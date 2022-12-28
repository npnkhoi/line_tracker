import serial

class Arduino():
    def __init__(self):
        # set up serial for communication with Arduino
        self.ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
        self.ser.reset_input_buffer()
    
    def write(self, s: str) -> None:
        self.ser.write((s + '\n').encode('utf-8'))