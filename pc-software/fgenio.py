#!/usr/bin/env python3

import serial
import io


class FgenIO():
    def __init__(self):
        self.serial = serial.Serial()

    def openIO(self,devname):
        self.devname = devname
        self.serial.port = devname
        self.serial.baudrate = 57600
        self.serial.parity = 'N'
        self.serial.writeTimeout = 0

        try:
            self.serial.open()
            print("Connected to " + fgen.portstr)
        except:
            print("Device not found")

    def closeIO(self):
        self.serial.close()

