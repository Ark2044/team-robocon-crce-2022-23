import tkinter as tk
from tkinter import *
import time
import serial
import threading
import continuous_threading
# import numpy as np
# from matplotlib import pyplot as plt
# from matplotlib.pyplot import figure as Figure
# from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
# from matplotlib import animation

# # Port
# ser = serial.Serial('/dev/ttyUSB0', 9600)
# val = 0
# index = []
# ser.close()
# ser.open()

# # Arduino values in array
# Encoder1 = []
# Encoder2 = []
# Encoder3 = []
# Encoder4 = []
# y = []

# Read data from arduino


# def readserial():
#     global val
#     ser_bytes = ser.readline()
#     ser_bytes = ser_bytes.decode("utf-8")
#     print(ser_bytes.rstrip())
#     val = ser_bytes
#     index.append(val)

#     if (len(index) == 1):
#         displayL1 = tk.Label(root, text=index[0]).place(x=70, y=70)
#     elif (len(index) == 2):
#         displayL2 = tk.Label(root, text=index[1]).place(x=70, y=200)
#     elif (len(index) == 3):
#         display3 = tk.Label(root, text=index[2]).place(x=550, y=200)
#     elif (len(index) == 4):
#         display4 = tk.Label(root, text=index[3]).place(x=550, y=70)

#     if len(index) == 4:

#         # Encoder1.append(index[0])
#         # Encoder2.append(index[1])
#         # Encoder3.append(index[2])
#         # Encoder4.append(index[3])
#         # anima = animation.FuncAnimation(plt.gcf(), draw_graph, interval=1500)
#         index.clear()

#     time.sleep(0.5)


# # Thread
# t1 = continuous_threading.PeriodicThread(0.5, readserial)

root = tk.Tk()
root.state('zoomed')


def fwd():
    ser.write(b'F')


def bkw():
    ser.write(b'R')


def rt():
    ser.write(b'r')


def lt():
    ser.write(b'l')


def cw():
    ser.write(b'W')


def ccw():
    ser.write(b'w')


def stp():
    ser.write(b's')


def m1u():
    ser.write(b'A')


def m1d():
    ser.write(b'a')


def m2u():
    ser.write(b'B')


def m2d():
    ser.write(b'b')


def m3u():
    ser.write(b'C')


def m3d():
    ser.write(b'c')


def m4u():
    ser.write(b'D')


def m4d():
    ser.write(b'd')

# Button


def which_button(button_press):
    print(button_press)


# Stop
button = tk.Button(root, text="Stop", borderwidth=10, bg="red",
                   command=lambda m="STOP": which_button(m))
button.place(x=250, y=10)

# Battery
B = tk.Label(root, text="Battery", font=('calibri', 20, 'bold'),
             background='green', foreground='white').place(x=1000, y=0)

# Left 1
L1 = tk.Label(root, text="LF", font=('calibri', 20, 'bold'),
              background='brown', foreground='white').place(x=20, y=30)

w = tk.Label(root, text="Speed:").place(x=20, y=70)

# Up button
button = tk.Button(root, text="Up", command=m1u)
button.place(x=20, y=90)

# Down button
button = tk.Button(root, text="Down",
                   command=m1d)
button.place(x=20, y=120)

# Left 2
L2 = tk.Label(root, text="LR", font=('calibri', 20, 'bold'),
              background='brown', foreground='white').place(x=20, y=160)

w = tk.Label(root, text="Speed:").place(x=20, y=200)

# Up button
button = tk.Button(root, text="Up", command=m2u)
button.place(x=20, y=225)

# Down button
button = tk.Button(root, text="Down",
                   command=m2d)
button.place(x=20, y=250)

# Right 1
R1 = tk.Label(root, text="RF", font=('calibri', 20, 'bold'),
              background='blue', foreground='white').place(x=500, y=30)

w = tk.Label(root, text="Speed:").place(x=500, y=70)

# Up button
button = tk.Button(root, text="Up", command=m4u)
button.place(x=500, y=90)

# Down button
button = tk.Button(root, text="Down",
                   command=m4d)
button.place(x=500, y=120)

# Right 2
R2 = tk.Label(root, text="RR", font=('calibri', 20, 'bold'),
              background='blue', foreground='white').place(x=500, y=160)

w = tk.Label(root, text="Speed:").place(x=500, y=200)

# Up button
button = tk.Button(root, text="Up", command=m3u)
button.place(x=500, y=225)

# Down button
button = tk.Button(root, text="Down",
                   command=m3d)
button.place(x=500, y=250)

# Forward
button = tk.Button(root, text="Forward",
                   command=fwd)
button.place(x=250, y=350)

# BACKWARD
button = tk.Button(root, text="Backward",
                   command=bkw)
button.place(x=250, y=450)

# RIGHT
button = tk.Button(root, text="Right",
                   command=rt)
button.place(x=280, y=400)

# LEFT
button = tk.Button(root, text="Left",
                   command=lt)
button.place(x=220, y=400)

# CLOCK
button = tk.Button(root, text="Clock",
                   command=cw)
button.place(x=250, y=500)

# ANTICLOCK
button = tk.Button(root, text="Anticlock",
                   command=ccw)
button.place(x=300, y=500)

# STOP
button = tk.Button(root, text="Stop",
                   command=stp)
button.place(x=400, y=500)

# # Graph
# count = 0


# def draw_graph(i):
#     global count
#     count += 1
#     y.append(count)

#     plt.plot(Encoder1, y)
#     plt.plot(Encoder2, y)
#     plt.plot(Encoder3, y)
#     plt.plot(Encoder4, y)


# plt.xlabel("I am x")
# plt.ylabel("I am y")
# # anima=animation.FuncAnimation(plt.gcf(),draw_graph,interval=1500)
# plt.grid(True)


# t1.start()

root.mainloop()
