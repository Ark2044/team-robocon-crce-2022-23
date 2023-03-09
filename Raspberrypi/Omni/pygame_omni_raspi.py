from RPi import GPIO
from time import sleep
import sys
import signal
import time
import pygame
import continuous_threading
from threading import Thread
import csv
sys.path.append('C:/Users/DELL/Robocon_Aarush/Raspberrypi/Motor_functions')
from Motor_control import *

filename = "encoder_readings.csv"
file = open(filename, "a")
fields = ['Time','Motor1', 'Motor2', 'Motor3', 'Motor4']
with open(filename, 'a', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(fields)
file.close()

GPIO.setmode(GPIO.BOARD)
GPIO.setwarnings(False)

dt4 = 13
dt3 = 15
dt2 = 29
dt1 = 31

count1 =0
count2 =0
count3 =0
count4 =0
current_time=0
interval=1
    
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
duty1 = duty2 = duty3 = duty4 = 75

p1 = GPIO.PWM(m1_pwm, freq1)  # (pin,freq)
p2 = GPIO.PWM(m2_pwm, freq2)
p3 = GPIO.PWM(m3_pwm, freq3)
p4 = GPIO.PWM(m4_pwm, freq4)

def signal_handler(sig, frame):
    GPIO.cleanup()
    sys.exit(0)

def count_callback1(channel):
    global count1
    count1+=1
    
def count_callback2(channel):
    global count2
    count2+=1
    
def count_callback3(channel):
    global count3
    count3+=1

def count_callback4(channel):
    global count4
    count4+=1


def calspeed():
    global current_time, count1, count2, count3, count4
    if ((time.time() - current_time) > interval):
        current_time = time.time()
        print('Speed1 =', count1)
        print('Speed2 =', count2)
        print('Speed3 =', count3)
        print('Speed4 =', count4)
        data = [[str(current_time),str(count1),str(count2),str(count3),str(count4)]]
        # create the CSV
        with open(filename, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerows(data)
        file.close()
        count1 = 0
        count2 = 0
        count3 = 0
        count4 = 0

#Thread
t1 = continuous_threading.PeriodicThread(0.1, calspeed)

GPIO.setup(dt1, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(dt2, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(dt3, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(dt4, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.add_event_detect(dt1, GPIO.RISING, callback=count_callback1,bouncetime=5)
GPIO.add_event_detect(dt2, GPIO.RISING, callback=count_callback2,bouncetime=5)
GPIO.add_event_detect(dt3, GPIO.RISING, callback=count_callback3,bouncetime=5)
GPIO.add_event_detect(dt4, GPIO.RISING, callback=count_callback4,bouncetime=5)
run_bool = False
pygame.init()
display=pygame.display.set_mode((300,300))
t1.start()

if __name__ == '__main__':
    try:
        while True:
            signal.signal(signal.SIGINT, signal_handler)
            #calibrate()
            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    if event.key==pygame.K_w:
                        print("efgw")
                        fwd()
                    if event.key==pygame.K_a:
                        lt()
                    if event.key==pygame.K_s:
                        bkw()
                    if event.key==pygame.K_d:
                        rt()
                    
                elif event.type == pygame.KEYUP:
                    if event.key==pygame.K_w:
                        stp()
                    if event.key==pygame.K_a:
                        stp()
                    if event.key==pygame.K_s:
                        stp()
                    if event.key==pygame.K_d:
                        stp()
                        
    except:
        stp()
