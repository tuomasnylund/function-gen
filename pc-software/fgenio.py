#!/usr/bin/env python3

import serial
import io


class FgenIO():
    def __init__(self):
        self.serial = serial.Serial()

    def open(self,devname):
        self.devname = devname
        self.serial.port = devname
        self.serial.baudrate = 57600
        self.serial.parity = 'N'
        self.serial.writeTimeout = 0

        try:
            self.serial.open()
            print("Connected to " + self.serial.portstr)
        except:
            print("Device not found")

    def close(self):
        self.serial.close()

    def setPhase(self, reg, phase):
        if reg == 1:
            self.serial.write("sp1 ".encode('utf=8'))
        else:
            self.serial.write("sp2 ".encode('utf=8'))

        self.serial.write(str(phase).encode('utf-8'))
        self.serial.write("\r\n".encode('utf-8'))

    def setPhaseOut(self, reg):
        self.serial.write("spo "  .encode('utf8'))
        self.serial.write(str(reg).encode('utf8'))
        self.serial.write("\r\n"  .encode('utf8'))

    def setFreqOut(self, reg):
        self.serial.write("sfo "  .encode('utf8'))
        self.serial.write(str(reg).encode('utf8'))
        self.serial.write("\r\n"  .encode('utf8'))

    def setFreq(self, reg, freq):
        if reg == 1:
            self.serial.write("sf1 ".encode('utf=8'))
        else:
            self.serial.write("sf2 ".encode('utf=8'))

        self.serial.write(str(freq).encode('utf-8'))
        self.serial.write("\r\n".encode('utf-8'))

    def setMode(self,mode):
        commands = {
                "Off"     :"so o\r\n".encode('utf-8'),
                "Sine"    :"so s\r\n".encode('utf-8'),
                "Triangle":"so t\r\n".encode('utf-8'),
                "Square"  :"so q\r\n".encode('utf-8')
                }
        self.serial.write(commands[mode])

        #print("writing",commands[mode])
        #print ("changing mode to ",mode)

    def on(self):
        sef.serial.write("P\r\n".encode('utf-8'))
