import serial
import re
import sys
from time import sleep


class EvoError(Exception):
    def __init__(self,value):
        self.value = value
    def __str__(self):
        return repr(self.value)

class EvoController:
    """EvoFab Controller Class"""
    def __init__(self,serialPort):
        self.pattern = re.compile("\A(\+|\-)\d\d\d(\+|\-)\d\d\d\Z")
        try:
            self.ser = serial.Serial(
            port = serialPort,
            baudrate = 9600,
            parity = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            bytesize = serial.EIGHTBITS)
            sleep(1)
        except serial.SerialException:
            print "Error connecting"
            sys.exit(0)

    def flush(self):
        try:
            self.ser.flushInput()
            self.ser.flushOutput()
        except Exception, e:
            raise EvoError("Error Flushing Serial")

    def extrude(self):
        self.flush()
        try:
            self.ser.write('e')
        except Exception, e:
            raise EvoError("Error Issuing Extrude Command")
        return True

    def pause(self):
        self.flush()
        try:
            self.ser.write('p')
        except Exception, e:
            raise EvoError("Error Issuing Pause Command")
        return True

    def home(self):
        self.flush()
        try:
            self.ser.write('h')
            ret = self.ser.read(1)
            if ret == 'h':
                return True
        except Exception, e:
            raise EvoError("Error sending or recieving home command")
        raise EvoError("Controller Error, expected home aknoledgement, got" + ret)

    def changeVelocity(self,velocity):
        self.flush()
        if self.pattern.match(velocity):
            try:
                self.ser.write(velocity)
                return True
            except Exception, e:
                print e
                raise EvoError("Error sending velocity command to printer")
        else:
            raise EvoError("Incorrect velocity inputed to changeVelocity")

    def disable(self):
        self.flush()
        try:
            self.ser.write('d')
            return True
        except Exception, e:
            EvoError("Error issuing disable command")

    def testHome(self):
        self.flush()
        try:
            self.ser.write('t')
            ret = self.ser.read(1)
            if ret == 't':
                return True
        except Exception, e:
            raise EvoError("Serial Error while testHoming")
        raise EvoError("Controller error while testHoming, expected responce 't', recieved " + ret)

    def close(self):
        self.disable()
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

class EvoConveyor:
    """EvoFab Conveyor Bed Class"""
    def __init__(self,serialPort,runTime=10):
        self.runTime = runTime
        try:
            self.ser = serial.Serial(
            port = serialPort,
            baudrate =9600,
            parity = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            bytesize = serial.EIGHTBITS)
            sleep(1)
            self.ser.write("GO\n")
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

    def run(self):
        self.flush()
        self.ser.write("R3200\n")
        sleep(self.runTime)
        self.ser.write("B0\n")
        return

    def close(self):
        self.ser.write("B0\n")
        self.ser.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()


class EvoZAxis:
    """EvoFab Temporary Z-Axis Controller Class"""
    def __init__(self,serialPort):
        try:
            self.ser = serial.Serial(
            port = serialPort,
            baudrate = 9600,
            parity = serial.PARITY_NONE,
            stopbits = serial.STOPBITS_ONE,
            bytesize = serial.EIGHTBITS)
            sleep(1)
        except serial.SerialException:
            print "Error connecting"
            sys.exit(0)

    def flush(self):
        try:
            self.ser.flushInput()
            self.ser.flushOutput()
        except Exception, e:
            raise EvoError("Error Flushing Serial")

    def up(self):
        self.flush()
        try:
            self.ser.write('u')
            ret = self.ser.read(1)
            if ret == 'u':
                return True
        except Exception, e:
            raise EvoError("Error sending or recieving up command")
        raise EvoError("Z-Axis Error, expected up aknoledgement, got" + ret)

    def down(self):
        self.flush()
        try:
            self.ser.write('d')
            ret = self.ser.read(1)
            if ret == 'd':
                return True
        except Exception, e:
            raise EvoError("Error sending or recieving down command")
        raise EvoError("Z-Axis Error, expected down aknoledgement, got" + ret)

    def close(self):
        self.ser.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
