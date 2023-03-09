//I2C Slave2 (address 10) reciever code for HC-SR04 Stops the Gearmotor Through I2C/
//List of the lubraries included in the project******/
#include <Wire.h>  // Arduino library that enables I2C functionality
char SerialData1[3];
class SimplePID {
private:
  float kp, kd, ki, umax;  // Parameters
  float eprev, eintegral;  // Storage

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
  void evalu(int value, int target, float deltaT, int &pwr, int &dir) {
    // error
    float e = target - (value * 4.333333333333);

    // derivative
    float dedt = (e - eprev) / (deltaT);

    // integral
    eintegral = eintegral + e * deltaT;

    // control signal
    float u = (value * 4.333333333) + (kp * e + kd * dedt + ki * eintegral);

    // motor power
    pwr = (int)fabs(u);
    if (pwr > umax) {
      pwr = umax;
    }

    // motor direction
    dir = 1;
    if (u < 0) {
      dir = -1;
    }

    // store previous error
    eprev = e;
  }
};


// Pin definition *********** /
  //int DO_Blink = 13;
// List of the variables that will be recieved via I2C ****** / byte I2C_OnOff;  //defining the variable that will be sent

int PIN_INPUT1 = 3;
int PIN_INPUT2 = 2;
volatile byte state1 = LOW;
volatile byte state2 = LOW;

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

int M1_PWM, M2_PWM;
int target1, target2;

long prevT = 0;
int m1_dirflag, m2_dirflag;

//motors
int pwr1, pwr2, dir1, dir2;

SimplePID pid1, pid2;

// Setup loop *********** / 
void setup() {
  // pinMode(DO_Blink, OUTPUT);                    // Sets the DO_Blink as output
  Wire.begin(10);           // Join I2C bus as the slave with address 1
  Wire.onReceive(BlinkLAD);  // When the data transmition is detected call receiveEvent function
  Wire.onRequest(requestEvent);
  Serial.begin(9600);

  pid1.setParams(1, 0, 0, 255);
  pid2.setParams(1, 0, 0, 255);


  attachInterrupt(digitalPinToInterrupt(PIN_INPUT1), interrupt_routine1, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT2), interrupt_routine2, RISING);

  //Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
}

// Main loop *********** / 
void loop() {
  delay(100);
  // time difference
  currT = micros();
  deltaT = ((float)(currT - prevT));
  if (currT - prevT > 1000000) {
    readspeed();
    prevT = currT;
  }

  newcount1 = ctr1;

  newcount2 = ctr2;
}


// Function / Event call ********* / 
void BlinkLAD() {
  target1 = (int)Wire.read();  // Reads the data sent via I2C
  Serial.print(target1);
  target2 = target1;
  pid1.evalu(s1, target1, deltaT, pwr1, dir1);
  pid2.evalu(s2, target2, deltaT, pwr2, dir2);
  setMotor1(dir1, pwr1, m1_pwm, m1_dir);
  setMotor2(dir2, pwr2, m2_pwm, m2_dir);
}

void requestEvent() {

  String temp_str = String(s1);
  temp_str.toCharArray(SerialData1, 5);
  Wire.write(SerialData1);
}


void interrupt_routine1() {
  ctr1++;
}

void interrupt_routine2() {
  ctr2++;
}

void readspeed() {
  s1 = (newcount1 - oldcount1);
  Serial.print("IR1 = ");
  Serial.print(s1);
  Serial.print(" PWM1 = ");
  Serial.print(pwr1);
  Serial.print(" Target = ");
  Serial.println(target1);
  oldcount1 = newcount1;

  s2 = (newcount2 - oldcount2);
  Serial.print("IR2 = ");
  Serial.print(s2);
  Serial.print(" PWM2 = ");
  Serial.print(pwr2);
  Serial.print(" Target = ");
  Serial.println(target2);
  oldcount2 = newcount2;
}


void setMotor1(int dir, int pwmVal, int pwm, int dir1) {
  analogWrite(pwm, pwmVal);
  //digitalWrite(dir1,HIGH);
  if (dir == 1) {
    // Serial.println("dir1 high");
    // Serial.println(dir1);
    digitalWrite(dir1, HIGH);
    m1_dirflag = 1;
  } else if (dir == 0) {
    // Serial.println("dir1 low");
    // Serial.println(dir1);
    digitalWrite(dir1, LOW);
    m1_dirflag = 0;
  } else {
    analogWrite(pwm, 0);
  }
}

void setMotor2(int dir, int pwmVal, int pwm, int dir2) {
  analogWrite(pwm, pwmVal);
  // digitalWrite(dir2,HIGH);
  if (dir == 1) {
    // Serial.println("dir2 high");
    // Serial.println(dir1);
    digitalWrite(dir1, HIGH);
    m2_dirflag = 1;
  } else if (dir == 0) {
    // Serial.println("dir2 low");
    // Serial.println(dir1);
    digitalWrite(dir1, LOW);
    m2_dirflag = 0;
  } else {
    analogWrite(pwm, 0);
  }
}