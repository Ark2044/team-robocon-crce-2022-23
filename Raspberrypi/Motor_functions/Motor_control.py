#HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)

def fwd():
    m1ccwMotor()
    m2cwMotor()
    m3cwMotor()
    m4ccwMotor()
    calibrate()


def bkw():
    m1cwMotor()
    m2ccwMotor()
    m3ccwMotor()
    m4cwMotor()
    calibrate()


def rt():
    m1ccwMotor()
    m2ccwMotor()
    m3cwMotor()
    m4cwMotor()
    calibrate()
  
def lt():
    m1cwMotor()
    m2cwMotor()
    m3ccwMotor()
    m4ccwMotor()
    calibrate()


def cw():
    m1ccwMotor()
    m2cwMotor()
    m3ccwMotor()
    m4cwMotor()
    calibrate()


def ccw():
    m1cwMotor()
    m2ccwMotor()
    m3cwMotor()
    m4ccwMotor()
    calibrate()

def stp():
  p1.start(0)
  p2.start(0)
  p3.start(0)
  p4.start(0) 

    
#LOW LEVEL MOTOR FUNCTIONS

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