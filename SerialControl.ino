import time
import mss
import numpy as np
import serial
import serial.tools.list_ports
from collections import deque

class AmbientLightController:
    def __init__(self):
        self.last_colors = deque(maxlen=3)
        self.ser = None
        self.screen_capture = mss.mss()
        
    def find_esp32_port(self):
        ports = serial.tools.list_ports.comports()
        for port in ports:
            if any(id in port.description for id in ["CP210", "CH340"]):
                return port.device
        return None

    def get_average_screen_color(self, monitor_number=2):
        try:
            screenshot = self.screen_capture.grab(self.screen_capture.monitors[monitor_number])
            img = np.array(screenshot)
            return np.mean(img, axis=(0, 1))[:3]
        except Exception as e:
            print(f"Screen capture error: {e}")
            return None

    @staticmethod
    def enhance_color(value, gamma=1.6, brightness=1.4, min_value=3):
        value = max(value, min_value)
        normalized = value / 255.0
        corrected = pow(normalized, 1/gamma) * brightness
        return min(255, max(0, int(corrected * 255)))

    def enhance_colors(self, r, g, b):
        # Initial enhancement with stronger gamma
        r = self.enhance_color(r)
        g = self.enhance_color(g)
        b = self.enhance_color(b)
        
        # Find dominant color
        max_val = max(r, g, b)
        
        if max_val > 20:
            # Stronger color separation
            boost_factor = 1.5
            reduce_factor = 0.5
            
            # Boost dominant color while reducing others
            if r == max_val:
                r = min(255, int(r * boost_factor))
                g = int(g * reduce_factor)
                b = int(b * reduce_factor)
            elif g == max_val:
                g = min(255, int(g * boost_factor))
                r = int(r * reduce_factor)
                b = int(b * reduce_factor)
            elif b == max_val:
                b = min(255, int(b * boost_factor))
                r = int(r * reduce_factor)
                g = int(g * reduce_factor)
        
        # Prevent white output
        if r > 200 and g > 200 and b > 200:
            min_color = min(r, g, b)
            if min_color == r: r = int(r * 0.4)
            if min_color == g: g = int(g * 0.4)
            if min_color == b: b = int(b * 0.4)
        
        # Final vibrancy boost
        vibrancy = 1.3
        r = min(255, int(r * vibrancy))
        g = min(255, int(g * vibrancy))
        b = min(255, int(b * vibrancy))
        
        return r, g, b

    def smooth_colors(self, r, g, b, smoothing=0.3):
        self.last_colors.append((r, g, b))
        if len(self.last_colors) < 2:
            return r, g, b

        avg_colors = np.mean(self.last_colors, axis=0)
        smooth_r = int(avg_colors[0] * (1 - smoothing) + r * smoothing)
        smooth_g = int(avg_colors[1] * (1 - smoothing) + g * smoothing)
        smooth_b = int(avg_colors[2] * (1 - smoothing) + b * smoothing)
        
        return smooth_r, smooth_g, smooth_b

    def run_color_test(self):
        test_colors = [
            (255, 0, 0),    # Red
            (0, 255, 0),    # Green
            (0, 0, 255),    # Blue
            (255, 255, 0),  # Yellow
            (255, 0, 255),  # Magenta
            (0, 255, 255),  # Cyan
            (255, 128, 0),  # Orange
            (128, 0, 255)   # Purple
        ]
        
        print("\nRunning color test sequence...")
        for r, g, b in test_colors:
            command = f"RGB:{r},{g},{b}\n"
            print(f"Testing color: {command.strip()}")
            self.ser.write(command.encode())
            time.sleep(0.5)

    def main_loop(self):
        try:
            port = self.find_esp32_port()
            if not port:
                print("ESP32 not found! Please check connection.")
                return

            print(f"ESP32 found on port: {port}")
            self.ser = serial.Serial(port, 115200, timeout=1)
            print("Connected to ESP32")
            
            time.sleep(1)
            print("Starting color sync. Press Ctrl+C to stop.")
            
            self.run_color_test()
            
            last_update = time.time()
            last_command = ""
            
            while True:
                current_time = time.time()
                if current_time - last_update < 0.016:
                    continue
                    
                colors = self.get_average_screen_color()
                if colors is None:
                    continue
                    
                b, g, r = colors
                r, g, b = self.enhance_colors(r, g, b)
                r, g, b = self.smooth_colors(r, g, b)
                
                command = f"RGB:{r},{g},{b}\n"
                if command != last_command:
                    self.ser.write(command.encode())
                    last_command = command
                    print(f"\rCurrent RGB: ({r}, {g}, {b})", end="", flush=True)
                
                last_update = current_time
                
        except KeyboardInterrupt:
            print("\nStopping ambient light sync...")
        except Exception as e:
            print(f"\nError: {e}")
        finally:
            if self.ser:
                self.ser.write(b"RGB:0,0,0\n")
                self.ser.close()
                print("\nSerial connection closed")
            self.screen_capture.close()

def main():
    controller = AmbientLightController()
    controller.main_loop()

if __name__ == "__main__":
    main()