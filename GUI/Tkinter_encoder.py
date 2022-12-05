import tkinter as tk
from tkinter import *
import time
import serial
import threading
import continuous_threading
import numpy as np
from matplotlib import pyplot as plt
from matplotlib.pyplot import figure as Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib import animation

# Port
ser = serial.Serial('COM10', 9600)
val = 0
index = []
ser.close()
ser.open()

# Arduino values in array
Encoder1 = []
Encoder2 = []
Encoder3 = []
Encoder4 = []
y = []

# Read data from arduino

def readserial():
    global val
    ser_bytes = ser.readline()
    ser_bytes = ser_bytes.decode("utf-8")
    print(ser_bytes.rstrip())
    val = ser_bytes
    index.append(val)

    if (len(index) == 1):
        displayL1 = tk.Label(root, text=index[0]).place(x=70, y=70)
    elif (len(index) == 2):
        displayL2 = tk.Label(root, text=index[1]).place(x=70, y=200)
    elif (len(index) == 3):
        display3 = tk.Label(root, text=index[2]).place(x=550, y=70)
    elif (len(index) == 4):
        display4 = tk.Label(root, text=index[3]).place(x=550, y=200)

    if len(index) == 4:

        # Encoder1.append(index[0])
        # Encoder2.append(index[1])
        # Encoder3.append(index[2])
        # Encoder4.append(index[3])
        # anima = animation.FuncAnimation(plt.gcf(), draw_graph, interval=1500)
        index.clear()

    time.sleep(0.5)


# Thread
t1 = continuous_threading.PeriodicThread(0.5, readserial)

root = tk.Tk()
root.state('zoomed')

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
button = tk.Button(root, text="Up", command=lambda m="L1 Up": which_button(m))
button.place(x=20, y=90)

# Down button
button = tk.Button(root, text="Down",
                   command=lambda m="L1 Down": which_button(m))
button.place(x=20, y=120)

# Left 2
L2 = tk.Label(root, text="LR", font=('calibri', 20, 'bold'),
              background='brown', foreground='white').place(x=20, y=160)

w = tk.Label(root, text="Speed:").place(x=20, y=200)

# Up button
button = tk.Button(root, text="Up", command=lambda m="L2 Up": which_button(m))
button.place(x=20, y=225)

# Down button
button = tk.Button(root, text="Down",
                   command=lambda m="L2 Down": which_button(m))
button.place(x=20, y=250)

# Right 1
R1 = tk.Label(root, text="RF", font=('calibri', 20, 'bold'),
              background='blue', foreground='white').place(x=500, y=30)

w = tk.Label(root, text="Speed:").place(x=500, y=70)

# Up button
button = tk.Button(root, text="Up", command=lambda m="R1 Up": which_button(m))
button.place(x=500, y=90)

# Down button
button = tk.Button(root, text="Down",
                   command=lambda m="R1 Down": which_button(m))
button.place(x=500, y=120)

# Right 2
R2 = tk.Label(root, text="RR", font=('calibri', 20, 'bold'),
              background='blue', foreground='white').place(x=500, y=160)

w = tk.Label(root, text="Speed:").place(x=500, y=200)

# Up button
button = tk.Button(root, text="Up", command=lambda m="R2 Up": which_button(m))
button.place(x=500, y=225)

# Down button
button = tk.Button(root, text="Down",
                   command=lambda m="R2 Down": which_button(m))
button.place(x=500, y=250)

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


t1.start()

root.mainloop()
