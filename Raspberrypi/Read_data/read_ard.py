import serial
import time
import pandas as pd
import csv
ser=serial.Serial(port='COM10',baudrate=9600)
print("Connected to Arduino port")
filename="analog-data.csv"
file=open(filename,"a")
print("Created file")

samples = 100 #how many samples to collect
print_labels = False
line = 0 #start at 0 because our header is 0 (not real data)
encoder_data = [] #store data

# collect the samples
while line <= samples:
    getData=ser.readline()
    dataString = getData.decode('utf-8')
    data=dataString[0:][:-2]
    #data=dataString[0:]
    print(data)

    readings = data.split(",")
    print(readings)

    encoder_data.append(readings)
    print(encoder_data)

    line = line+1

# create the CSV
with open(filename, 'w', encoding='UTF8', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(encoder_data)

print("Data collection complete!")
file.close()

# def write_read():
#     time.sleep(0.05)
#     data=arduino.readline()
#     return data

# while True:
#     value = write_read()
#     value=value.decode('ascii')
#     print (value)

#     if "Speed1" in value:
#         num1=[int(x) for x in value.split() if x.isdigit()]
#         readings=value.split(",")
    
#     if "Speed2" in value:
#         num2=[int(x) for x in value.split() if x.isdigit()]
#         readings=value.split(",")

#     if "Speed3" in value:
#         num3=[int(x) for x in value.split() if x.isdigit()]
#         readings=value.split(",")

#     if "Speed4" in value:
#         num4=[int(x) for x in value.split() if x.isdigit()]
#         readings=value.split(",")

#     dict={'1':num1,'2':num2,'3':num3,'4':num4}
#     df=pd.DataFrame(dict)