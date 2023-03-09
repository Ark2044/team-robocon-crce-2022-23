import RPi.GPIO as GPIO
import time
import sys
import signal
#import pygame
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

dt1 = 27
ir1 = 4
speed_ir1=0
count1 =0
current_time=0
interval=1

def count_callback(channel):
    global speed_ir1
    speed_ir1+=1
    
def count_callback1(channel):
    global count1
    count1+=1

GPIO.setup(ir1,GPIO.IN)
GPIO.add_event_detect(
        ir1, GPIO.RISING, callback=count_callback, bouncetime=5)
GPIO.setup(dt1, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.add_event_detect(dt1, GPIO.RISING, callback=count_callback1,bouncetime=5)
#pygame.init()
#display=pygame.display.set_mode((300,300))
try: 
   while True:
       if((time.time() - current_time) > interval):
            current_time = time.time();
            print('Speed1 =',count1)
            print("Speed1_IR = ",speed_ir1)
            count1=0
            speed_ir1=0
#        for event in pygame.event.get():
                #if event.type == pygame.KEYDOWN:
#                    if event.key==pygame.K_f:
#                         print("key press")
#                         fwd()
#                 elif event.type == pygame.KEYUP:
#                     if event.key==pygame.K_f:
#                         print("key up")
#                         stp()

    #   if GPIO.input(ir1):
    #     speed_ir1+=1
    #     while GPIO.input(ir1):


except KeyboardInterrupt:
    GPIO.cleanup()