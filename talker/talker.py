import serial
import time
import threading

ERROR = 1
PING  = 2

class Axon2(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.setDaemon(True)
        self.serial_mutex = threading.Lock()
        self.ser = serial.Serial()
        self.ser.setTimeout(0.015)
        self.ser.baudrate = 115200
        self.ser.port = 0
        self.run4ever = True
        self.ser.open()
		
    def stop(self):
        self.run4ever = False
        
    def __del__(self):
        self.close()

    def isOpen(self):
        return self.ser.isOpen()
        
    def close(self):
        if self.ser:
            self.ser.flushInput()
            self.ser.flushOutput()
            self.ser.close()

    def write_serial(self, data):
        self.serial_mutex.acquire()
        self.ser.flushInput()
        self.ser.flushOutput()
        for c in data:
            self.ser.write(c)
        self.serial_mutex.release()
        
    def read_serial(self):
        self.serial_mutex.acquire()
        count = self.ser.inWaiting()
        str = self.ser.read(count)
        self.serial_mutex.release()
        return str

    def ping(self):
        packet = [PING, 5, 'h', 'e', 'l', 'l', 'o']
        self.write_serial(packet)
        
    def run(self):
        while self.run4ever == True:
            str = self.read_serial()
            if len(str) > 0:
                print str
        
def main():
    try:
        test = Axon2()
        test.start()
        test.ping()
        while True: time.sleep(100)
    except (KeyboardInterrupt, SystemExit):
        print '\n! Received keyboard interrupt, quitting threads.\n'
    
main()

