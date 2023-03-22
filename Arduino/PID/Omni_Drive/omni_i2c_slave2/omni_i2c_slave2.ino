//I2C Slave2 (address 10) reciever code for HC-SR04 Stops the Gearmotor Through I2C

#include <Wire.h>  // Arduino library that enables I2C functionality
char SerialData1[3];
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
// List of the variables that will be recieved via I2C ****** / byte I2C_OnOff;  //defining the variable that will be sent
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

int pwr1, pwr2;

SimplePID pid1, pid2;

const int inPin = 8;

// Setup loop *********** /
void setup() {
  pinMode(inPin, INPUT);
  Wire.begin(10);                // Join I2C bus as the slave with address 1
  Wire.onReceive(receiveEvent);  // When the data transmition is detected call receiveEvent function
  Wire.onRequest(requestEvent);
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

  pid1.setParams(1.8, 0, 0.0000005, 255);       //Change kP,kD,kI,umax only here
  pid2.setParams(1.8, 0, 0.0000005, 255);       //Change kP,kD,kI,umax only here
}

// Main loop *********** /
void loop() {
  delay(100);
  //Serial.println("Input Low");
  // String rxString = "";
  // String strArr[6];
  // if (Serial.available()) {
  //   //Keep looping until there is something in the buffer.
  //   while (Serial.available()) {
  //     //Delay to allow byte to arrive in input buffer.
  //     delay(2);
  //     //Read a single character from the buffer.
  //     char ch = Serial.read();
  //     //Append that single character to a string.
  //     rxString += ch;
  //   }
  //   int stringStart = 0;
  //   int arrayIndex = 0;
  //   for (int i = 0; i < rxString.length(); i++) {
  //     //Get character and check if it's our "special" character.
  //     if (rxString.charAt(i) == ',') {
  //       //Clear previous values from array.
  //       strArr[arrayIndex] = "";
  //       //Save substring into array.
  //       strArr[arrayIndex] = rxString.substring(stringStart, i);
  //       //Set new string starting point.
  //       stringStart = (i + 1);
  //       arrayIndex++;
  //     }
  //   }
  //   //Put values from the array into the variables.
  //   String kP1 = strArr[0];
  //   String kD1 = strArr[1];
  //   String kI1 = strArr[2];
  //   String kP2 = strArr[3];
  //   String kD2 = strArr[4];
  //   String kI2 = strArr[5];

  //   //Convert string to int.
  //   int kP1_value = kP1.toInt();
  //   int kD1_value = kD1.toInt();
  //   int kI1_value = kI1.toInt();
  //   int kP2_value = kP2.toInt();
  //   int kD2_value = kD2.toInt();
  //   int kI2_value = kI2.toInt();

  //   pid1.setParams(kP1_value, kD1_value, kI1_value, 255);
  //   pid2.setParams(kP2_value, kD2_value, kI2_value, 255);
  // }

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
// time difference
// currT = micros();
// deltaT = ((float)(currT - prevT));
// if (currT - prevT > 1000000) {
//   readspeed();
//   prevT = currT;
// }

// if (stopp == false) {
//   target1=15;
//   target2=15;
// } else if (stopp == true) {
//   target1=0;
//   target2=0;
//   stp();
// }

// pid1.evalu(s1, target1, deltaT, pwr1);
// pid2.evalu(s2, target2, deltaT, pwr2);

// newcount1 = ctr1;

// newcount2 = ctr2;



// Function / Event call ********* /
void receiveEvent() {
  argument = Wire.read();  // Reads the data sent via I2C
  Serial.print("Argument: ");
  Serial.println(argument);
  if (argument == 'f') {  //  Forward
    stopp = false;
    target1 = 25;
    target2 = 25;
    fwd();
  } else if (argument == 'b') {  //  Backward
    stopp = false;
    target1 = 25;
    target2 = 25;
    bkw();
  } else if (argument == 'l') {  //  Left
    stopp = false;
    target1 = 25;
    target2 = 25;
    lt();
  } else if (argument == 'r') {  //  Right
    stopp = false;
    target1 = 25;
    target2 = 25;
    rt();
  } else if (argument == 's') {  //  Stop
    stopp = true;
    target1 = 0;
    target2 = 0;
  } else if (argument == 'c') {  //  Clock
    stopp = false;
    target1 = 15;
    target2 = 15;
    cw();
  } else if (argument == 'a') {  //  Anti-Clock
    stopp = false;
    target1 = 15;
    target2 = 15;
    ccw();
  }
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
  Serial.println("Bot moving left");
  m1_cwMotor();
  m2_cwMotor();
}
void cw() {
  Serial.println("Bot moving clockwise");
  m1_cwMotor();
  m2_cwMotor();
}
void ccw() {
  Serial.println("Bot moving counter-clockwise");
  m1_ccwMotor();
  m2_ccwMotor();
}
void stp() {
  Serial.println("Bot stop");
  m1_stopMotor();
  m2_stopMotor();
}

//LOW LEVEL MOTOR FUNCTIONS

void m1_cwMotor() {
  Serial.println("Motor A clockwise");
  digitalWrite(m1_dir, HIGH);
  //analogWrite(m1_pwm, pwr1);
}
void m1_ccwMotor() {
  Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir, LOW);
  //analogWrite(m1_pwm, pwr1);
}
void m2_cwMotor() {
  Serial.println("Motor B clockwise");
  digitalWrite(m2_dir, HIGH);
  //analogWrite(m2_pwm, pwr2);
}
void m2_ccwMotor() {
  Serial.println("Motor B counter-clockwise");
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