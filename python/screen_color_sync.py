import time
import mss
import numpy as np
import serial
import serial.tools.list_ports
from threading import Thread, Event
from queue import Queue

class ColorSync:
    def __init__(self):
        self.stop_event = Event()
        self.color_queue = Queue(maxsize=2)
        self.ser = None
        self.last_r = self.last_g = self.last_b = 0
        self.smoothing = 0.3
        self.gamma_table = np.array([min(255, int((i/255) ** (1/1.6) * 1.4 * 255)) for i in range(256)])

    def find_esp32_port(self):
        ports = serial.tools.list_ports.comports()
        for port in ports:
            if "CP210" in port.description or "CH340" in port.description:
                return port.device
        return None

    def get_average_screen_color(self):
        with mss.mss() as sct:
            # Capture the second monitor (change to 1 if you're using primary monitor)
            monitor = sct.monitors[1]
            screenshot = sct.grab(monitor)
            img = np.array(screenshot)[::4, ::4]
            return np.mean(img, axis=(0, 1))[:3]

    def enhance_colors(self, r, g, b):
        r = self.gamma_table[min(255, max(0, int(r)))]
        g = self.gamma_table[min(255, max(0, int(g)))]
        b = self.gamma_table[min(255, max(0, int(b)))]
        
        max_val = max(r, g, b)
        
        if max_val > 20:
            boost_factor = 1.5
            reduction_factor = 0.5
            
            if r == max_val:
                r = min(255, int(r * boost_factor))
                g = int(g * reduction_factor)
                b = int(b * reduction_factor)
            elif g == max_val:
                g = min(255, int(g * boost_factor))
                r = int(r * reduction_factor)
                b = int(b * reduction_factor)
            else:
                b = min(255, int(b * boost_factor))
                r = int(r * reduction_factor)
                g = int(g * reduction_factor)
        
        if r > 180 and g > 180 and b > 180:
            max_color = max(r, g, b)
            if max_color == r:
                g = int(g * 0.6)
                b = int(b * 0.6)
            elif max_color == g:
                r = int(r * 0.6)
                b = int(b * 0.6)
            else:
                r = int(r * 0.6)
                g = int(g * 0.6)
        
        final_boost = 1.3
        return (
            min(255, int(r * final_boost)),
            min(255, int(g * final_boost)),
            min(255, int(b * final_boost))
        )

    def color_capture_thread(self):
        while not self.stop_event.is_set():
            try:
                b, g, r = self.get_average_screen_color()
                r, g, b = self.enhance_colors(r, g, b)
                
                r = int(r * (1 - self.smoothing) + self.last_r * self.smoothing)
                g = int(g * (1 - self.smoothing) + self.last_g * self.smoothing)
                b = int(b * (1 - self.smoothing) + self.last_b * self.smoothing)
                
                self.last_r, self.last_g, self.last_b = r, g, b
                
                if not self.color_queue.full():
                    self.color_queue.put((r, g, b))
                
                time.sleep(0.033)
                
            except Exception as e:
                print(f"\nError in capture thread: {e}")
                time.sleep(1)

    def serial_write_thread(self):
        while not self.stop_event.is_set():
            try:
                r, g, b = self.color_queue.get(timeout=1)
                command = f"RGB:{r},{g},{b}\n"
                self.ser.write(command.encode())
                print(f"\rCurrent RGB: ({r}, {g}, {b})", end="")
                
            except Queue.Empty:
                continue
            except Exception as e:
                print(f"\nError in serial thread: {e}")
                time.sleep(1)

    def run_test_sequence(self):
        test_colors = [
            (255, 0, 0), (0, 255, 0), (0, 0, 255),
            (255, 255, 0), (255, 0, 255), (0, 255, 255),
            (255, 128, 0), (128, 0, 255)
        ]
        
        for r, g, b in test_colors:
            command = f"RGB:{r},{g},{b}\n"
            print(f"Testing color: {command.strip()}")
            self.ser.write(command.encode())
            time.sleep(1)

    def main(self):
        print("Starting Ambient Light Sync...")
        
        try:
            port = self.find_esp32_port()
            if not port:
                print("ESP32 not found! Please check connection.")
                return

            print(f"ESP32 found on port: {port}")
            self.ser = serial.Serial(port, 115200, timeout=1)
            print("Connected to ESP32")
            
            time.sleep(2)
            self.run_test_sequence()
            
            print("Starting color sync. Press Ctrl+C to stop.")
            
            capture_thread = Thread(target=self.color_capture_thread)
            serial_thread = Thread(target=self.serial_write_thread)
            
            capture_thread.start()
            serial_thread.start()
            
            while True:
                time.sleep(0.1)
                
        except KeyboardInterrupt:
            print("\nStopping ambient light sync...")
        except Exception as e:
            print(f"\nError: {e}")
        finally:
            self.stop_event.set()
            if self.ser:
                self.ser.write(b"RGB:0,0,0\n")
                self.ser.close()
                print("\nSerial connection closed")

if __name__ == "__main__":
    color_sync = ColorSync()
    color_sync.main()