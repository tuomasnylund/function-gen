#!/usr/bin/env python3

import io

from tkinter import *
from tkinter import ttk

from fgenio import Fgen


#class FgenInterface(Frame):
#    def __init__(self):
#        Frame.__init__(self)
#        self.master.title("Function Generator")
#        self.fgenio = FgenIO()
#        self.grid()


class digit(Frame):
    def __init__(self,root):
        Frame.__init__(self)
        self.root = root

        self.var = IntVar()
        self.var.set(0)

        Button(self, command=self.up).pack()
        self.num = ttk.Label(self,textvariable=self.var).pack()
        Button(self,command=self.down).pack()

    def up(self):
        self.var.set('1')
    def down(self):
        self.var.set('2')



class FgenFselect(Frame):
    def __init__(self,root,name):
        Frame.__init__(self)
        self.root = root
        self.grid()

        #variables
        self.frange = StringVar()
        self.frange.set("Hz")
        self.fval = DoubleVar()

        #label
        ttk.Label(self,text=name).grid(column=0,row=0)

        #scale
        self.scale = Scale(self, variable=self.fval, from_=0, to=1000,showvalue=0, resolution=0.1,orient=HORIZONTAL)
        self.scale.grid(column=1,row=0)
        self.scale.bind(sequence='<4>',func=self.Mouse)
        self.scale.bind(sequence='<5>',func=self.Mouse)

        #entry
        self.entry = ttk.Entry(self,textvariable=self.fval,width=6).grid(column=2,row=0)

        #menu
        self.menu = OptionMenu(self,self.frange,"Hz","kHz","MHz", command=self.RangeChange).grid(column=3,row=0)

    def RangeChange(self,event):
        print (self.frange.get())

    def Mouse(self,event):
        if event.type == '4':
            if event.num == 4:
                self.fval.set(self.fval.get()+0.1)
                print("scrollup!")
            elif event.num == 5:
                self.fval.set(self.fval.get()-0.1)
                print("scrolldown!")

        
        
fgen = Fgen()
fgen.open("/dev/ttyACM1")

root = Tk()

root.grid()

digit(root).grid(column=0,row=0)
digit(root).grid(column=1,row=0)
digit(root).grid(column=2,row=0)
digit(root).grid(column=3,row=0)

#fsel1 = FgenFselect(root,"f1")
#fsel1.pack()

#fsel2 = FgenFselect(root,"f2")
#fsel2.pack()

root.mainloop()
