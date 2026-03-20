#!/usr/bin/env python3

import serial
import time
import sys

PORT = "/dev/ttyUSB0"
BAUD = 115200

def send_at(ser, cmd):
    ser.write((cmd + "\r\n").encode())
    time.sleep(0.3)
    resp = ser.read(50).decode('utf-8', errors='replace').strip()
    return resp

def test_line_length(ser, count):
    msg = "A" * count
    print(f"Testing {count} chars: {msg}")
    resp = send_at(ser, f"AT+TEXT={msg}")
    print(f"  Response: {resp}")
    time.sleep(1.5)

def main():
    try:
        ser = serial.Serial(PORT, BAUD, timeout=2)
        time.sleep(0.5)
        print(f"Connected to {PORT}\n")
        
        start = int(sys.argv[1]) if len(sys.argv) > 1 else 10
        end = int(sys.argv[2]) if len(sys.argv) > 2 else 30
        
        send_at(ser, "AT+CLEAR")
        print(f"Testing line lengths from {start} to {end}\n")
        
        for i in range(start, end + 1):
            test_line_length(ser, i)
        
        ser.close()
        print("\nDone! Check screen for max chars per line.")
        
    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
