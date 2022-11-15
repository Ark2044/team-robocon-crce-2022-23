import tkinter as tk
from tkinter import *
import time
# import serial
# import threading
# import continuous_threading

# ser = serial.Serial('COM10',9600)
# val = 0
# index=[]
# ser.close()
# ser.open()

# def readserial():
#     global val
#     ser_bytes = ser.readline()
#     ser_bytes = ser_bytes.decode("utf-8")
#     print(ser_bytes.rstrip())
#     val = ser_bytes
#     index.append(val)

#     if len(index) == 1:
#         display = tk.Label(root,text=index[0]).place(x=50,y=10)

#     if len(index) == 7:
#         index.clear()

#     time.sleep(0.1)

# t1 = continuous_threading.PeriodicThread(0.1, readserial)

root = tk.Tk()
root.state('zoomed')

# w = tk.Label(root,text="Speed:").place(x=10,y=10)
# t1.start()

def which_button(button_press):
    print(button_press)

# labelframe1 = LabelFrame(root, text="P")  
# labelframe1.pack(fill="both", expand="yes")
# rootlabel = Label(labelframe1, text="Contents")  
# rootlabel.pack()  

#Stop
button = tk.Button(root, text="Stop",command=lambda m="L1 Up": which_button(m))
button.place(x=250, y=10)

#Battery
B = tk.Label(root,text="Battery", font = ('calibri', 20, 'bold'),background = 'red',foreground = 'white').place(x=1000,y=0)

#Left 1
L1 = tk.Label(root,text="LF", font = ('calibri', 20, 'bold'),background = 'red',foreground = 'white').place(x=20,y=30)

w = tk.Label(root,text="Speed:").place(x=20,y=70)

#Up button
button = tk.Button(root, text="Up",command=lambda m="L1 Up": which_button(m))
button.place(x=20, y=90)
#Down button
button = tk.Button(root, text="Down",command=lambda m="L1 Down": which_button(m))
button.place(x=20, y=120)

#Left 2
L2 = tk.Label(root,text="LR", font = ('calibri', 20, 'bold'),background = 'red',foreground = 'white').place(x=20,y=160)

w = tk.Label(root,text="Speed:").place(x=20,y=200)

#Up button
button = tk.Button(root, text="Up",command=lambda m="L2 Up": which_button(m))
button.place(x=20, y=225)
#Down button
button = tk.Button(root, text="Down",command=lambda m="L2 Down": which_button(m))
button.place(x=20, y=250)

#Right 1
R1 = tk.Label(root,text="RF", font = ('calibri', 20, 'bold'),background = 'blue',foreground = 'white').place(x=500,y=30)

w = tk.Label(root,text="Speed:").place(x=500,y=70)

#Up button
button = tk.Button(root, text="Up",command=lambda m="R1 Up": which_button(m))
button.place(x=500, y=90)
#Down button
button = tk.Button(root, text="Down",command=lambda m="R1 Down": which_button(m))
button.place(x=500, y=120)

#Right 2
R2 = tk.Label(root,text="RR", font = ('calibri', 20, 'bold'),background = 'blue',foreground = 'white').place(x=500,y=160)

w = tk.Label(root,text="Speed:").place(x=500,y=200)

#Up button
button = tk.Button(root, text="Up",command=lambda m="R2 Up": which_button(m))
button.place(x=500, y=225)
#Down button
button = tk.Button(root, text="Down",command=lambda m="R2 Down": which_button(m))
button.place(x=500, y=250)



root.mainloop()