import serial
import sys

port = sys.argv[1]
ser = serial.Serial(port)
x = ser.read()
if x == "S":
    print("Connected. Now Homing...")
    ser.write("h")
    y = ""
    while(y != "h"):
        y = ser.read()
    print("Homeing Successful!")
else:
    print("Could not connect to EvoFab Printer.")
