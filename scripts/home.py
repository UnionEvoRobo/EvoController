import serial
import getopt
import sys

helptext = 'home.py -p port'

try:
    opts, args = getopt.getopt(sys.argv[1:], "p:", ["port="])
except getopt.GetoptError:
    print("Useage:")
    print helptext
    sys.exit()

port = ""

for opt, arg in opts:
    if opt in ('-p', '--port'):
        port = arg

try:
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
except OSError:
    print("Could not connect to EvoFab Printer, invalid serial port.")
    print("Useage:")
    print(helptext)
