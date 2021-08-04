from tkinter import *
from tkinter.font import Font
from time import sleep
#import serial
#import time
#import sys 

raiz=Tk()
raiz.title("Interfaz USART")
#raiz.resizable(1,1)
raiz.iconbitmap("jm.ico")
#raiz.geometry("650x500")
raiz.config(bg="gray")
miFrame=Frame(raiz, width=500, height=300)
miFrame.pack()
miFrame.config(bg="orange")
#miFrame.config(width="500",height="400")
miFrame.config(bd=15)
miFrame.config(relief="sunken")
miFrame.config(cursor="pirate")
miImagen=PhotoImage(file="")

cont=IntVar()
pot1=IntVar()
pot2=IntVar()



#miLabel= Label(miFrame,text="Comunicacion UART",fg="blue",bg="orange",font=("Comic Sans MS", 18)).place(x="150",y="10")
titulo=Label(miFrame, text="Comunicacion USART",bg="orange", font=("Alternate Gothic",18))
titulo.place(x=120, y=10)


Contador=Entry(miFrame, textvariable = cont)
Contador.config(bg="light gray", justify="center")
Contador.place(x=150, y=110)
nombreLabel=Label(miFrame, text="Contador:", bg="orange", font=("Alternate Gothic",12))
nombreLabel.place(x=10, y=110)

Pot1=Entry(miFrame, textvariable=pot1)

Pot1.place(x=50, y=210)
Pot1.config(bg="light gray", justify="center")
nombreLabel=Label(miFrame, text="Potenciómetro 1:",bg="orange", font=("Alternate Gothic",12))
nombreLabel.place(x=50, y=180)
VLabel=Label(miFrame, text="V",bg="orange", font=("Alternate Gothic",12))
VLabel.place(x=180, y=205)

Pot2=Entry(miFrame, textvariable=pot2)
Pot2.place(x=280, y=210)
Pot2.config(bg="light gray", justify="center")
nombreLabel=Label(miFrame, text="Potenciómetro 2:",bg="orange", font=("Alternate Gothic",12))
nombreLabel.place(x=280, y=180)
VLabel=Label(miFrame, text="V",bg="orange", font=("Alternate Gothic",12))
VLabel.place(x=410, y=205)


def codigoBoton():
	cont.set(0)

def codigoConectar():
	pot1.set(2.2)
	pot2.set(1.5)

botonEnvio=Button(raiz, text="ENVIAR", command=codigoBoton)
botonEnvio.place(x= 400, y= 120)

botonEnvio=Button(raiz, text="CONECTAR", command=codigoConectar)
botonEnvio.place(x= 30, y= 70)

raiz.mainloop()
