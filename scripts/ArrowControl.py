import curses
import serial
import getopt
import sys

helptext = 'ArrowControl.py -p port'

def displayInfo(connected, screen):
    screen.clear()
    if connected:
        screen.addstr(0,10,"Welcome to EvoFab Arrow Control. Status: Connected")
    else:
        screen.addstr(0,10,"Welcome to EvoFab Arrow Control. Status: Not Connected")
    screen.addstr(1,10,"Hit 'e' to extrude, and 'p' to pause extrusion.")
    screen.addstr(2,10,"Use the arrow keys to control the printer.")
    screen.addstr(3,10,"Type 'q' to quit.")
    screen.refresh()

if __name__ == "__main__":
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

        stdscr = curses.initscr()
        curses.cbreak()
        curses.noecho()
        stdscr.nodelay(1)
        stdscr.keypad(1)

        displayInfo(False,stdscr)

        if x == "S":
            displayInfo(True,stdscr)
            key = ''
            while key != ord('q'):
                key = stdscr.getch()
                if key == curses.KEY_UP:
                    ser.write("+000+020")
                elif key == curses.KEY_DOWN:
                    ser.write("+000-020")
                elif key == curses.KEY_LEFT:
                    ser.write("-020+000")
                elif key == curses.KEY_RIGHT:
                    ser.write("+020+000")
                elif key == ord('e'):
                    ser.write('e')
                elif key == ord('p'):
                    ser.write('p')
                elif key == ord('h'):
                    ser.write('h')
                else:
                    continue
                    #displayInfo(True,stdscr)
            ser.write("d")
            curses.endwin()
            sys.exit()
        else:
            print("Could not connect to EvoFab Printer.")
        curses.endwin()
    except OSError:
        print("Could not connect to EvoFab Printer, invalid serial port.")
        print("Useage:")
        print(helptext)
