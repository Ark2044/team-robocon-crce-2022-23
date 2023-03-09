import RPi.GPIO as GPIO
import random
import numpy
from time import sleep
import sys
import signal
import time
import csv
import continuous_threading

filename = "encoder.csv"
file = open(filename, "a")
fields = ['Motor1', 'Motor2', 'Motor3', 'Motor4']
with open(filename, 'a', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(fields)
file.close()

encoder_data = []

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

dt4 = 13
dt3 = 15
dt2 = 29
dt1 = 31

count1 = 0
count2 = 0
count3 = 0
count4 = 0
current_time = 0
interval = 1

# Cytron 1
m1_dir = 22
m1_pwm = 33
m2_dir = 37
m2_pwm = 35

GPIO.setup(m1_pwm, GPIO.OUT)
GPIO.setup(m2_pwm, GPIO.OUT)
GPIO.setup(m1_dir, GPIO.OUT)
GPIO.setup(m2_dir, GPIO.OUT)

# cytron 2
m3_dir = 16
m3_pwm = 32
m4_dir = 18
m4_pwm = 12

GPIO.setup(m3_pwm, GPIO.OUT)
GPIO.setup(m4_pwm, GPIO.OUT)
GPIO.setup(m3_dir, GPIO.OUT)
GPIO.setup(m4_dir, GPIO.OUT)

freq1 = freq2 = freq3 = freq4 = 5000
duty1 = duty2 = duty3 = duty4 = 50
inc0 = dec0 = inc1 = dec1 = inc2 = dec2 = inc3 = dec3 = 0

func_bool = False

p1 = GPIO.PWM(m1_pwm, freq1)  # (pin,freq)
p2 = GPIO.PWM(m2_pwm, freq2)
p3 = GPIO.PWM(m3_pwm, freq3)
p4 = GPIO.PWM(m4_pwm, freq4)

p4.start(0)

def signal_handler(sig, frame):
    GPIO.cleanup()
    sys.exit(0)


def count_callback1(channel):
    global count1
    count1 += 1


def count_callback2(channel):
    global count2
    count2 += 1


def count_callback3(channel):
    global count3
    count3 += 1


def count_callback4(channel):
    global count4
    count4 += 1


GPIO.setup(dt1, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(dt2, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(dt3, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(dt4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.add_event_detect(dt1, GPIO.RISING, callback=count_callback1, bouncetime=5)
GPIO.add_event_detect(dt2, GPIO.RISING, callback=count_callback2, bouncetime=5)
GPIO.add_event_detect(dt3, GPIO.RISING, callback=count_callback3, bouncetime=5)
GPIO.add_event_detect(dt4, GPIO.RISING, callback=count_callback4, bouncetime=5)

def stp():
    p1.start(0)
    p2.start(0)
    p3.start(0)
    p4.start(0)


def calspeed():
    global current_time, count1, count2, count3, count4
    if ((time.time() - current_time) > interval):
        current_time = time.time()
        print('Speed1 =', count1)
        print('Speed2 =', count2)
        print('Speed3 =', count3)
        print('Speed4 =', count4)
        count1 = 0
        count2 = 0
        count3 = 0
        count4 = 0

#Thread
t1 = continuous_threading.PeriodicThread(0.1, calspeed)


sort_list = []

calib_bool = False
my_dict = {"0": [], "1": [], "2": [], "3": []}


def inc_dec(index, dir):
    global inc0, inc1, inc2, inc3, dec0, dec1, dec2, dec3, duty1, duty2, duty3, duty4
    if (index == 0 and dir > 0) and inc0 < 30 and count1 < 250:
        inc0 = inc0+1
        duty1 += inc0
        p1.ChangeDutyCycle(duty1)
        # print("inc0", inc0)

    if (index == 1 and dir > 0) and inc1 < 30 and count2 < 250:
        inc1 = inc1+1
        duty2 += inc1
        p2.ChangeDutyCycle(duty2)
        # print("inc1", inc1)

    if (index == 2 and dir > 0) and inc2 < 30 and count3 < 250:
        inc2 = inc2+1
        duty3 += inc2
        p3.ChangeDutyCycle(duty3)
        # print("inc2", inc2)

    if (index == 3 and dir > 0) and inc3 < 30 and count4 < 250:
        inc3 = inc3+1
        duty4 += inc3
        p4.ChangeDutyCycle(duty4)
        # print("inc3", inc3)

    if (index == 0 and dir < 0) and dec0 >= 0 and count1 < 250:
        dec0 = dec0+1
        duty1 -= dec0
        p1.ChangeDutyCycle(duty1)
        # print("dec0", dec0)

    if (index == 1 and dir < 0) and dec1 >= 0 and count2 < 250:
        dec1 = dec1+1
        duty2 -= dec1
        p2.ChangeDutyCycle(duty2)
        # print("dec1", dec1)

    if (index == 2 and dir < 0) and dec2 >= 0 and count3 < 250:
        dec2 = dec2+1
        duty3 -= dec2
        p3.ChangeDutyCycle(duty3)
        # print("dec2", dec2)

    if (index == 3 and dir < 0) and dec3 >= 0 and count4 < 250:
        dec3 = dec3+1
        duty4 -= dec3
        p4.ChangeDutyCycle(duty4)
        # print("dec3", dec3)


def calibrate():
    while (count1 != 0 or count2 != 0 or count3 != 0 or count4 != 0):
        global sort_list, calib_bool, file
        global inc0, inc1, inc2, inc3
        sort_list.append(count1)
        sort_list.append(count2)
        sort_list.append(count3)
        sort_list.append(count4)
        sort_list = numpy.array(sort_list)
        sort_index = numpy.argsort(sort_list)
        sort_list = sort_list.tolist()
        sort_index = sort_index.tolist()
        sort_list.sort()
        sort_index.reverse()
        sort_list.reverse()
        # print("diff", sort_list[0]-sort_list[1])
        # print("before breaking", sort_list)
        if (sort_list[0]-sort_list[1] > 5):
            index = sort_index[1]
            inc_dec(index, 1)
            # inc1=inc1+5
            sort_list = []
        elif (sort_list[0]-sort_list[2] > 5):
            index = sort_index[2]
            inc_dec(index, 1)
            sort_list = []
        elif (sort_list[0]-sort_list[3] > 5):
            index = sort_index[3]
            inc_dec(index, 1)
            sort_list = []
        elif (sort_list[0]-sort_list[1] < -5):
            index = sort_index[1]
            inc_dec(index, -1)
            # inc1=inc1+5
            sort_list = []
        elif (sort_list[0]-sort_list[2] < -5):
            index = sort_index[2]
            inc_dec(index, -1)
            sort_list = []
        elif (sort_list[0]-sort_list[3] < -5):
            index = sort_index[3]
            inc_dec(index, -1)
            sort_list = []
        else:
            calib_bool = True
            print('calibrated')
            encoder_data = ""
            data = [duty1, duty2, duty3, duty4]
            for i in data:
                encoder_data += str(i)
            # create the CSV
            with open(filename, 'a', newline='') as f:
                writer = csv.writer(f)
                writer.writerows(encoder_data)
            file.close()
            #stp()
            break
        # print(sort_list)
        # print(sort_index)


# HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)

def fwd():
    func_bool = True
    m1ccwMotor()
    m2ccwMotor()
    m3cwMotor()
    m4cwMotor()
    calibrate()


def bkw():
    func_bool = True
    m1cwMotor()
    m2cwMotor()
    m3ccwMotor()
    m4ccwMotor()
    calibrate()


def rt():
    m1ccwMotor()
    m2cwMotor()
    m3cwMotor()
    m4ccwMotor()


def lt():
    m1cwMotor()
    m2ccwMotor()
    m3ccwMotor()
    m4cwMotor()
    calibrate()


def cw():
    m1ccwMotor()
    m2ccwMotor()
    m3ccwMotor()
    m4ccwMotor()
    calibrate()


def ccw():
    m1cwMotor()
    m2cwMotor()
    m3cwMotor()
    m4cwMotor()
    calibrate()


# LOW LEVEL MOTOR FUNCTIONS

def m1cwMotor():
    GPIO.output(m1_dir, GPIO.HIGH)
    p1.start(duty1)  # (dutyCycle)


def m1ccwMotor():
    GPIO.output(m1_dir, GPIO.LOW)
    p1.start(duty1)


def m2cwMotor():
    GPIO.output(m2_dir, GPIO.HIGH)
    p2.start(duty2)


def m2ccwMotor():
    GPIO.output(m2_dir, GPIO.LOW)
    p2.start(duty2)


def m3cwMotor():
    GPIO.output(m3_dir, GPIO.HIGH)
    p3.start(duty3)


def m3ccwMotor():
    GPIO.output(m3_dir, GPIO.LOW)
    p3.start(duty3)


def m4cwMotor():
    GPIO.output(m4_dir, GPIO.HIGH)
    p4.start(duty4)


def m4ccwMotor():
    GPIO.output(m4_dir, GPIO.LOW)
    p4.start(duty4)

t1.start()
while True:
    try:
        while calib_bool == False:
            fwd()
            break
    except KeyboardInterrupt():
        stp()