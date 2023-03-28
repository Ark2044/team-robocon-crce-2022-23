//I2C Slave2 (address 10) reciever code

#include <Wire.h>  // Arduino library that enables I2C functionality
class SimplePID {
private:
  float kp, kd, ki, umax;  // Parameters
  float eprev, eintegral;  // Storage
  float e,u;
public:
  // Constructor
  SimplePID()
    : kp(1), kd(0), ki(0), umax(255), eprev(0.0), eintegral(0.0) {}

  // A function to set the parameters
  void setParams(float kpIn, float kdIn, float kiIn, float umaxIn) {
    kp = kpIn;
    kd = kdIn;
    ki = kiIn;
    umax = umaxIn;
  }

  // A function to compute the control signal
  void evalu(int value, int target, float deltaT, int &pwr, int pwm_pin) {
    // error
    e = target - value;
    // Serial.print("value = ");
    // Serial.print(value);
    // Serial.print(" e = ");
    // Serial.print(e);

    // derivative
    float dedt = (e - eprev) / (deltaT);

    // integral
    eintegral = eintegral + e * deltaT;

    // control signal
    if(target==0)
    {
      u=(0.1*e+kd*dedt);      
    }
    else{
     u = (kp * e + kd * dedt + ki * eintegral);
    }

    // Serial.print(" u = ");
    // Serial.print(u);

    // motor power
    pwr = (int)fabs(u);
    //pwr = abs(pwr);

    if (pwr > umax) {
      pwr = umax;
    }
    // Serial.print(" pwr = ");
    // Serial.println(pwr);

    analogWrite(pwm_pin, pwr);

    // store previous error
    eprev = e;
  }
};


// Pin definition *********** /
// List of the variables that will be recieved via I2C 
char argument;
int PIN_INPUT1 = 3;
int PIN_INPUT2 = 2;
volatile byte state1 = LOW;
volatile byte state2 = LOW;
bool stopp = false;

//IR
volatile long long int ctr1 = 0, ctr2 = 0;
long newcount1, newcount2;
long oldcount1 = 0, oldcount2 = 0;
int s1 = 0, s2 = 0;

long currT;
float deltaT;

//CYTRON
const int m1_dir = 5;
const int m1_pwm = 6;

const int m2_dir = 7;
const int m2_pwm = 9;

int target1, target2;

long prevT = 0;

//Motor
int pwr1, pwr2;

SimplePID pid1, pid2;


// Setup loop *********** /
void setup() {
  pinMode(inPin, INPUT);
  Wire.begin(10);                // Join I2C bus as the slave with address 1
  Wire.onReceive(receiveEvent);  // When the data transmition is detected call receiveEvent function

  Serial.begin(9600);

  pinMode(PIN_INPUT1, INPUT_PULLUP);
  pinMode(PIN_INPUT2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_INPUT1), interrupt_routine1, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT2), interrupt_routine2, RISING);

  //Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  //BR->1 BL->2

  pid1.setParams(1.8, 0, 0.0000005, 255);       //Change kP,kD,kI,umax only here
  pid2.setParams(1.8, 0, 0.0000005, 255);       //Change kP,kD,kI,umax only here
}

// Main loop *********** /
void loop() {
  delay(100);

  currT = micros();
  deltaT = ((float)(currT - prevT));
  if (currT - prevT > 1000000) {
    noInterrupts();
    readspeed();
    prevT = currT;
    interrupts();
  }

  pid1.evalu(s1, target1, deltaT, pwr1,m1_pwm);
  pid2.evalu(s2, target2, deltaT, pwr2,m2_pwm);

  newcount1 = ctr1;

  newcount2 = ctr2;
}

// Function / Event call ********* /
void receiveEvent() {
  argument = Wire.read();  // Reads the data sent via I2C
  // Serial.print("Argument: ");
  // Serial.println(argument);
  if (argument == 'f') {  //  Forward
    target1 = 15;
    target2 = 15;
    fwd();
  } else if (argument == 'b') {  //  Backward
    target1 = 15;
    target2 = 15;
    bkw();
  } else if (argument == 'l') {  //  Left
    target1 = 15;
    target2 = 15;
    lt();
  } else if (argument == 'r') {  //  Right
    target1 = 15;
    target2 = 15;
    rt();
  } else if (argument == 's') {  //  Stop
    target1 = 0;
    target2 = 0;
  } else if (argument == 'c') {  //  Clock
    target1 = 15;
    target2 = 15;
    cw();
  } else if (argument == 'a') {  //  Anti-Clock
    target1 = 15;
    target2 = 15;
    ccw();
  }
}

void interrupt_routine1() {
  ctr1++;
}

void interrupt_routine2() {
  ctr2++;
}

void readspeed() {
  s1 = (newcount1 - oldcount1);
  Serial.print("BR = ");
  Serial.print(s1);
  Serial.print(",");
  // Serial.print(" PWM1 = ");
  // Serial.print(pwr1);
  // Serial.print(" Target = ");
  // Serial.println(target1);
  oldcount1 = newcount1;

  s2 = (newcount2 - oldcount2);
  Serial.print("BL = ");
  Serial.println(s2);
  // Serial.print(" PWM2 = ");
  // Serial.print(pwr2);
  // Serial.print(" Target = ");
  // Serial.println(target2);
  oldcount2 = newcount2;
}


//HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)

void fwd() {
  //Serial.println("Bot moving forward");
  m1_ccwMotor();
  m2_cwMotor();
}
void bkw() {
  //Serial.println("Bot moving backwards");
  m1_cwMotor();
  m2_ccwMotor();
}
void rt() {
  //Serial.println("Bot moving right");
  m1_ccwMotor();
  m2_ccwMotor();
}
void lt() {
  //Serial.println("Bot moving left");
  m1_cwMotor();
  m2_cwMotor();
}
void cw() {
  //Serial.println("Bot moving clockwise");
  m1_cwMotor();
  m2_cwMotor();
}
void ccw() {
  //Serial.println("Bot moving counter-clockwise");
  m1_ccwMotor();
  m2_ccwMotor();
}
void stp() {
  //Serial.println("Bot stop");
  m1_stopMotor();
  m2_stopMotor();
}

//LOW LEVEL MOTOR FUNCTIONS

void m1_cwMotor() {
  //Serial.println("Motor A clockwise");
  digitalWrite(m1_dir, HIGH);
  //analogWrite(m1_pwm, pwr1);
}
void m1_ccwMotor() {
  //Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir, LOW);
  //analogWrite(m1_pwm, pwr1);
}
void m2_cwMotor() {
  //Serial.println("Motor B clockwise");
  digitalWrite(m2_dir, HIGH);
  //analogWrite(m2_pwm, pwr2);
}
void m2_ccwMotor() {
  //Serial.println("Motor B counter-clockwise");
  digitalWrite(m2_dir, LOW);
  //analogWrite(m2_pwm, pwr2);
}

void m1_stopMotor() {
  // Serial.println("Motor A stop");
  // analogWrite(m1_pwm, 0);
  // analogWrite(m1_pwm, pwr1);
}
void m2_stopMotor() {
  // Serial.println("Motor B stop");
  // analogWrite(m2_pwm, 0);
  // analogWrite(m1_pwm, pwr2);
}