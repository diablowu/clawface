#!/usr/bin/env python3

import serial
import time
import sys
import random

PORT = "/dev/ttyUSB0"
BAUD = 115200

MESSAGES = [
    "HELLO WORLD",
    "ESP8266 READY", 
    "MINIMAX-M2.5",
    "TERMINAL MODE",
    "TEST MESSAGE",
    "1234567890",
    "ABCDEFGHIJ",
    "OPENCODE AI"
]

def send_at(ser, cmd):
    ser.write((cmd + "\r\n").encode())
    time.sleep(0.3)
    resp = ser.read(200).decode('utf-8', errors='replace').strip()
    if resp:
        print(f"  -> {resp}")
    return resp

def test_at(ser):
    print("=== Testing AT ===")
    resp = send_at(ser, "AT")
    return "OK" in resp

def test_clear(ser):
    print("=== Testing AT+CLEAR ===")
    resp = send_at(ser, "AT+CLEAR")
    return "OK" in resp

def test_text(ser, msg):
    print(f"=== Sending: {msg} ===")
    resp = send_at(ser, f"AT+TEXT={msg}")
    return "OK" in resp

def test_show(ser):
    print("=== Testing AT+SHOW ===")
    resp = send_at(ser, "AT+SHOW")
    return resp

def main():
    try:
        ser = serial.Serial(PORT, BAUD, timeout=2)
        time.sleep(0.5)
        print(f"Connected to {PORT} at {BAUD} baud\n")
        
        if len(sys.argv) > 1:
            cmd = sys.argv[1]
            if cmd == "-a" or cmd == "--at":
                test_at(ser)
            elif cmd == "-c" or cmd == "--clear":
                test_clear(ser)
            elif cmd == "-s" or cmd == "--show":
                test_show(ser)
            elif cmd in ["-t", "--text"]:
                msg = " ".join(sys.argv[2:]) if len(sys.argv) > 2 else "Test"
                test_text(ser, msg)
            elif cmd == "-n" and len(sys.argv) > 2:
                count = int(sys.argv[2])
                test_at(ser)
                test_clear(ser)
                for i in range(count):
                    msg = f"Test {i+1}"
                    test_text(ser, msg)
                    time.sleep(1)
                print(f"\nCompleted {count} messages")
            else:
                print("Usage: ./test_at.sh [options]")
                print("  -a, --at         Test AT command")
                print("  -c, --clear      Clear screen")
                print("  -s, --show       Show content")
                print("  -t, --text MSG   Send text message")
                print("  -n N             Run N tests")
        else:
            # Default: run full test
            test_at(ser)
            test_clear(ser)
            time.sleep(0.5)
            for i, msg in enumerate(MESSAGES[:3]):
                test_text(ser, msg)
                time.sleep(1)
            test_show(ser)
            
        ser.close()
        
    except serial.SerialException as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
