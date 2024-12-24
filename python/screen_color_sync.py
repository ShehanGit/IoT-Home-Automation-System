import time
import mss
import numpy as np
import serial
import serial.tools.list_ports

def find_esp32_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if "CP210" in port.description or "CH340" in port.description:
            return port.device
    return None

def get_average_screen_color(monitor_number=2):
    with mss.mss() as sct:
        monitor = sct.monitors[monitor_number]
        screenshot = sct.grab(monitor)
        img = np.array(screenshot)
        avg_color = np.mean(img, axis=(0, 1))
        return avg_color[:3]

def enhance_color(value):
    gamma = 1.6        # Lower gamma for more punchy colors
    saturation = 1.4   # Higher saturation
    min_value = 5      # Minimum brightness threshold
    
    value = max(value, min_value)
    normalized = value / 255.0
    corrected = pow(normalized, 1/gamma) * saturation
    
    return min(255, max(0, int(corrected * 255)))

def enhance_colors(r, g, b):
    # Initial color enhancement
    r = enhance_color(r)
    g = enhance_color(g)
    b = enhance_color(b)
    
    # Find dominant color
    max_val = max(r, g, b)
    
    if max_val > 20:  # Only process if there's significant color
        # Boost dominant color more aggressively
        boost_factor = 1.5
        reduction_factor = 0.5
        
        # Boost dominant and reduce others
        if r == max_val:
            r = min(255, int(r * boost_factor))
            g = int(g * reduction_factor)
            b = int(b * reduction_factor)
        elif g == max_val:
            g = min(255, int(g * boost_factor))
            r = int(r * reduction_factor)
            b = int(b * reduction_factor)
        elif b == max_val:
            b = min(255, int(b * boost_factor))
            r = int(r * reduction_factor)
            g = int(g * reduction_factor)
        
        # Handle mixed colors (when two colors are close)
        if abs(r - g) < 30 and r > 100 and g > 100:  # Yellow
            r = min(255, int(r * 1.2))
            g = min(255, int(g * 1.2))
            b = int(b * 0.3)
        elif abs(r - b) < 30 and r > 100 and b > 100:  # Magenta
            r = min(255, int(r * 1.2))
            b = min(255, int(b * 1.2))
            g = int(g * 0.3)
        elif abs(g - b) < 30 and g > 100 and b > 100:  # Cyan
            g = min(255, int(g * 1.2))
            b = min(255, int(b * 1.2))
            r = int(r * 0.3)
    
    # Prevent white output
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
    
    # Final brightness boost for vibrant colors
    final_boost = 1.3
    r = min(255, int(r * final_boost))
    g = min(255, int(g * final_boost))
    b = min(255, int(b * final_boost))
    
    return r, g, b

def main():
    print("Starting Ambient Light Sync...")
    
    try:
        port = find_esp32_port()
        if not port:
            print("ESP32 not found! Please check connection.")
            return

        print(f"ESP32 found on port: {port}")
        ser = serial.Serial(port, 115200, timeout=1)
        print("Connected to ESP32")
        
        time.sleep(2)
        print("Starting color sync. Press Ctrl+C to stop.")
        
        # Test sequence with vibrant colors
        test_colors = [
            (255, 0, 0),    # Pure Red
            (0, 255, 0),    # Pure Green
            (0, 0, 255),    # Pure Blue
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
            ser.write(command.encode())
            time.sleep(1)
        
        # Color smoothing variables
        last_r, last_g, last_b = 0, 0, 0
        smoothing = 0.3  # Fast response time
        
        while True:
            try:
                b, g, r = get_average_screen_color()
                r, g, b = enhance_colors(r, g, b)
                
                # Smooth color transitions
                r = int(r * (1 - smoothing) + last_r * smoothing)
                g = int(g * (1 - smoothing) + last_g * smoothing)
                b = int(b * (1 - smoothing) + last_b * smoothing)
                
                last_r, last_g, last_b = r, g, b
                
                command = f"RGB:{r},{g},{b}\n"
                ser.write(command.encode())
                
                print(f"\rCurrent RGB: ({r}, {g}, {b})", end="")
                time.sleep(0.016)  # ~60fps update rate
                
            except Exception as e:
                print(f"\nError during color processing: {e}")
                time.sleep(1)
                
    except KeyboardInterrupt:
        print("\nStopping ambient light sync...")
    except Exception as e:
        print(f"\nError: {e}")
    finally:
        if 'ser' in locals():
            ser.write(b"RGB:0,0,0\n")
            ser.close()
            print("\nSerial connection closed")

if __name__ == "__main__":
    main()