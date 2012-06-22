#!/usr/bin/env python3

import io

from tkinter import *
#from tkinter import ttk
from fgenio import Fgen


class FgenFselect(Frame):
    def __init__(self,root):
        Frame.__init__(self,bg="black")
        self.root = root
        self.grid()

        self.var = IntVar()
        self.var.set(0)

        self.num = []

        for i in range(0,10):
            Button(self, relief="flat",highlightbackground="black",bg="black", command= lambda i=i: self.btnCallback(i,1)).grid(column=i,row=0)
            self.num.append(Label(self,textvariable=self.var,bg="black",fg="white").grid(column=i,row=1))
            Button(self, relief="flat",highlightbackground="black",bg="black", command= lambda i=i: self.btnCallback(i,-1)).grid(column=i,row=2)

    def btnCallback(self,exp,change):
        print (change)
        print (exp)


    def set(val):
        self.var = val

    #def __init__(self,root):
    #    Frame.__init__(self)
    #    self.root = root
#
#        self.grid()
#
#        Segment(self,0).grid(column=0,row=0)
        #Segment(self,0).grid(column=1,row=0)
        #Segment(self,0).grid(column=2,row=0)
        #Segment(self,0).grid(column=3,row=0)

    #def change(self,exp,val):
    #    print ("omg")


        
        
#fgen = Fgen()
#fgen.open("/dev/ttyACM1")

root = Tk()

fsel = FgenFselect(root)
fsel.pack()

root.mainloop()
