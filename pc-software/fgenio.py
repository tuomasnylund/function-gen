#!/usr/bin/env python3

import serial
import io


class Fgen():
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

    def setFreq(self, freq):
        self.serial.write("f".encode('utf-8'))
        self.serial.write(str(freq).encode('utf-8'))
        self.serial.write("\r\n".encode('utf-8'))

    def on(self):
        sef.serial.write("P\r\n".encode('utf-8'))
