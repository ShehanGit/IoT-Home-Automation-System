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

def get_average_screen_color(monitor_number=1):
    with mss.mss() as sct:
        monitor = sct.monitors[monitor_number]
        screenshot = sct.grab(monitor)
        img = np.array(screenshot)
        avg_color = np.mean(img, axis=(0, 1))
        return avg_color[:3]

def enhance_color(value):
    # Reduce overall brightness and increase contrast
    gamma = 2.2  # Higher gamma for more contrast
    brightness_factor = 0.6  # Reduced brightness
    min_value = 10  # Lower minimum value
    
    # Apply minimum brightness
    value = max(value, min_value)
    
    # Apply gamma correction and brightness
    normalized = value / 255.0
    corrected = pow(normalized, 1/gamma) * brightness_factor
    
    return min(255, max(0, int(corrected * 255)))

def enhance_colors(r, g, b):
    # Enhance individual colors
    r = enhance_color(r)
    g = enhance_color(g)
    b = enhance_color(b)
    
    # Find dominant color
    max_val = max(r, g, b)
    if max_val > 0:
        # Reduce non-dominant colors more aggressively
        if r < max_val: r = int(r * 0.6)
        if g < max_val: g = int(g * 0.6)
        if b < max_val: b = int(b * 0.6)
        
        # Boost dominant color slightly
        if r == max_val: r = min(255, int(r * 1.1))
        if g == max_val: g = min(255, int(g * 1.1))
        if b == max_val: b = min(255, int(b * 1.1))
    
    # Prevent white output by ensuring at least one color is significantly lower
    if min(r, g, b) > 100:
        min_color = min(r, g, b)
        if min_color == r: r = int(r * 0.5)
        if min_color == g: g = int(g * 0.5)
        if min_color == b: b = int(b * 0.5)
    
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
        
        # Test with pure colors first
        test_colors = [
            (255, 0, 0),    # Red
            (0, 255, 0),    # Green
            (0, 0, 255),    # Blue
            (255, 255, 0),  # Yellow
            (255, 0, 255),  # Magenta
            (0, 255, 255),  # Cyan
            (255, 255, 255) # White
        ]
        
        print("\nRunning color test sequence...")
        for r, g, b in test_colors:
            command = f"RGB:{r},{g},{b}\n"
            print(f"Testing color: {command.strip()}")
            ser.write(command.encode())
            time.sleep(0.5)
        
        # Color smoothing variables
        last_r, last_g, last_b = 0, 0, 0
        smoothing = 0.8  # Higher smoothing for more stable colors
        
        while True:
            try:
                b, g, r = get_average_screen_color()
                r, g, b = enhance_colors(r, g, b)
                
                # Smooth color transitions
                r = int(r * smoothing + last_r * (1 - smoothing))
                g = int(g * smoothing + last_g * (1 - smoothing))
                b = int(b * smoothing + last_b * (1 - smoothing))
                
                last_r, last_g, last_b = r, g, b
                
                command = f"RGB:{r},{g},{b}\n"
                ser.write(command.encode())
                
                print(f"\rCurrent RGB: ({r}, {g}, {b})", end="")
                time.sleep(0.05)
                
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