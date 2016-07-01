import serial
import re
import sys
from time import sleep

class EvoController:
    """EvoFab Controller Class"""
    def __init__(self,serialPort):
        self.pattern = re.compile("\A(\+|\-)\d\d\d(\+|\-)\d\d\d\Z")
        try:
            self.ser = serial.Serial(
            port = serialPort,
            baudrate =9600,
            parity = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            bytesize = serial.EIGHTBITS)
            sleep(1)
        except serial.SerialException:
            import sys
            print "Error connecting"
            sys.exit(0)

    def flush(self):
        try:
            self.ser.flushInput()
            self.ser.flushOutput()
        except Exception, e:
            pass

    def extrude(self):
        self.flush()
        try:
            self.ser.write('e')
        except Exception, e:
            return False
        return True

    def pause(self):
        self.flush()
        try:
            self.ser.write('p')
        except Exception, e:
            return False
        return True

    def home(self):
        self.flush()
        try:
            self.ser.write('h')
            ret = self.ser.read(1)
            if ret == 'h':
                return True
        except Exception, e:
            return False
        return False

    def changeVelocity(self,velocity):
        self.flush()
        if self.pattern.match(velocity):
            try:
                self.ser.write(velocity)
                return True
            except Exception, e:
                print e
                return False
        else:
            print "pattern does not match"
        return False

    def disable(self):
        self.flush()
        try:
            self.ser.write('d')
            return True
        except Exception, e:
            return False

    def testHome(self):
        self.flush()
        try:
            self.ser.write('t')
            ret = self.ser.read(1)
            if ret == 't':
                return True
        except Exception, e:
            return False
        return False

    def close(self):
        self.ser.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()


class EvoArray:
    """EvoFab Array Class"""
    def __init__(self,serialPort):
        self.pattern = re.compile("\A(\+|\-)\d\d\d(\+|\-)\d\d\d\Z")
        try:
            self.ser = serial.Serial(
            port = serialPort,
            baudrate =9600,
            parity = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            bytesize = serial.EIGHTBITS)
            sleep(1)
        except serial.SerialException:
            import os
            print "Error connecting"
            sys.exit(0)

    def flush(self):
        try:
            self.ser.flushInput()
            self.ser.flushOutput()
        except Exception, e:
            pass

    def getNext(self):
        self.flush()
        line =  self.ser.readline()
        while not (len(line) == 35 and line[0] == '(' and line[-3] == ')'):
            line = self.ser.readline()
        code = "x =" + line
        exec code
        return x

    def close(self):
        self.ser.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
