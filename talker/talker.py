import serial
import time
import threading
import struct

ERROR = 1
PING  = 2

class Parser(object):
    SET_POS, GET_POS, GET_STATUS, STOP_MOTION = range(1, 5)
    LENS = [0, 5, 2, 2, 2]
    def __init__(self):
        self.buffer = ""
        self.messages = []
    
    def appendData(self, data):
        self.buffer += data
        self.parse()
        
    def messageParsed(self):
        tmp = self.messages
        self.messages = []
        return tmp
        
    def parse(self):
        while len(self.buffer)>2:
            cmd = struct.unpack("b", self.buffer[0])[0]
            if cmd == 0 or cmd > 4:
                #print "failed"
                self.buffer = ""
                break
            size = self.LENS[cmd]
            msg = []
            msg.append( cmd )
            for i in range(size):
                msg.append( struct.unpack("b", self.buffer[i+1])[0] )
            self.messages.append(msg)
            self.buffer = self.buffer[size+1:len(self.buffer)]

class Axon2(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.callbacks = []
        self.parser = Parser()
        self.setDaemon(True)
        self.serial_mutex = threading.Lock()
        self.ser = serial.Serial()
        self.ser.setTimeout(15)
        self.ser.bytesize = 8
        self.ser.parity = 'N'
        self.ser.baudrate = 115200
        self.ser.port = 0
        self.ser.stopbits = 1
        self.run4ever = True
        self.ser.open()
        self.start()
		
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
            time.sleep(0.1)
            self.ser.write(c)
        self.serial_mutex.release()
        
    def read_serial(self):
        self.serial_mutex.acquire()
        count = self.ser.inWaiting()
        str = self.ser.read(count)
        self.serial_mutex.release()
        return str
        
    def addCallback(self, clb):
        self.serial_mutex.acquire()
        self.callbacks.append(clb)
        self.serial_mutex.release()
        
    def run(self):
        while self.run4ever == True:
            str = self.read_serial()
            if len(str) > 0:
                #print str
                self.serial_mutex.acquire()
                self.parser.appendData(str)
                self.serial_mutex.release()
                messages = self.parser.messageParsed()
                for msg in messages:
                    for c in self.callbacks:
                        c.onReceive(msg)
        
class Motor(object):
    def __init__(self, id, serial):
        self.id = int(id)
        self.real = 0
        self.status = 0
        self.serial = serial
        self.c = threading.Condition()
        self.serial.addCallback(self)
    
    def onReceive(self, msg):
        if len(msg) > 0:
            if Parser.GET_POS == int(msg[0]):
                if self.id == int(msg[1]):
                    print "receive pos"
                    self.real = int(msg[2])
                    self.c.acquire()
                    self.c.notify()
                    self.c.release()
            if Parser.GET_STATUS == int(msg[0]):
                if self.id == int(msg[1]):
                    print "receive status"
                    self.status = int(msg[2])
                    self.c.acquire()
                    self.c.notify()
                    self.c.release()
        
    def setPosition(self, pos, tps):
        packet = struct.pack('bbbBB', Parser.SET_POS, self.id, pos, tps/256, tps%256)
        self.serial.write_serial(packet)
        
    def getPosition(self):
        packet = struct.pack('bb', Parser.GET_POS, self.id)
        self.c.acquire()
        self.serial.write_serial(packet)
        #self.c.wait(1)
        self.c.release()
        return self.real
        
    def getStatus(self):
        packet = struct.pack('bb', Parser.GET_STATUS, self.id)
        self.c.acquire()
        print "send status"
        print struct.unpack("bb", packet)
        self.serial.write_serial(packet)
        ##self.c.wait(1)
        self.c.release()
        return self.status
        
    def stop(self):
        packet = struct.pack('bb', Parser.STOP_MOTION, self.id)
        self.serial.write_serial(packet)
        
def main():
    try:
        test = Axon2()
        m = Motor(0, test)
        time.sleep(1)   
        while True:
            pos = raw_input('pos')
            tps = raw_input('tps')
            m.setPosition(int(pos),int(tps))
            for i in range(10):
                m.getPosition()
                time.sleep(0.1)
                print m.real
                
            #print "position : %d" % m.getPosition()
            #print "status : %d" % m.getStatus()
            #raw_input('attend position')
            #print "position : %d" % m.getPosition()
            #print "status : %d" % m.getStatus()
    except (KeyboardInterrupt, SystemExit):
        print '\n! Received keyboard interrupt, quitting threads.\n'
        
main()
