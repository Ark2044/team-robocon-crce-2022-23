#Read data from Slave1 and Slave2
import serial
import time
starttime = time.time()
#Filename
filename="omni_readings.csv"
#Open the file in append mode
file=open(filename,"a")
print("Created file")

# Define serial port names and baud rates
port1 = '/dev/ttyUSB0'
port2 = '/dev/ttyUSB1'
baudrate = 9600

# Open serial connections
ser1 = serial.Serial(port1, baudrate)
ser2 = serial.Serial(port2, baudrate)

while True:
    if(time.time()-starttime>=1):
        # Read data from port 1
        starttime=time.time()
        if ser1.in_waiting > 0:
            data1 = ser1.readline()
            print("Port 1:", data1)
            file.write(data1.decode('utf-8'))

        # Read data from port 2
        if ser2.in_waiting > 0:
            data2 = ser2.readline()
            print("Port 2:", data2)
            file.write(data2.decode('utf-8'))
