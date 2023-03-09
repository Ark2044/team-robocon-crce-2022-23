from RPi import GPIO
import continuous_threading
from time import sleep
import time
import signal
import sys

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

dt1 = 13
dt2 = 15
dt3 = 29
dt4 = 31

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

p1 = GPIO.PWM(m1_pwm, freq1)  # (pin,freq)
p2 = GPIO.PWM(m2_pwm, freq2)
p3 = GPIO.PWM(m3_pwm, freq3)
p4 = GPIO.PWM(m4_pwm, freq4)

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

m1cwMotor()
sleep(2)
m1ccwMotor()
sleep(2)
m2cwMotor()
sleep(2)
m2ccwMotor()
sleep(2)
m3cwMotor()
sleep(2)
m3ccwMotor()
sleep(2)
m4cwMotor()
sleep(2)
m4ccwMotor()
sleep(2)