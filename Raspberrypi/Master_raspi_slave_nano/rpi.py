#RPi Pinouts

#I2C Pins 
#GPIO2 -> SDA
#GPIO3 -> SCL

#Import the Library Required 
import smbus
import time

# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
#Slave Address 1
address_1 = 0x04

#Slave Address 2
address_2 = 0x05

# def writeNumber(value):
#     bus.write_byte(address, value)
#     bus.write_byte(address_2, value)
#     # bus.write_byte_data(address, 0, value)
#     return -1

def readNumber1():
    # number = bus.read_byte(address)
    number = bus.read_byte_data(address_1, 1)
    return number

def readNumber2():
    number = bus.read_byte_data(address_2, 1)
    return number
    
while True:
    readNumber1()
    readNumber2()
	# #Receives the data from the User
    # data = raw_input("Enter the data to be sent : ")
    # data_list = list(data)
    # for i in data_list:
    # 	#Sends to the Slaves 
    #     writeNumber(int(ord(i)))
    #     time.sleep(.1)

    # writeNumber(int(0x0A))

#End of the Script
