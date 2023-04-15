//I2C Slave2 (address 10) reciever code for HC-SR04 Stops the Gearmotor Through I2C
//BR->m1 BL->m2
#include <Wire.h>  // Arduino library that enables I2C functionality
char SerialData1[3];
class SimplePID {
private:
  float kp, kd, ki, umax;  // Parameters
  float eprev, integral;  // Storage
  float e, u;
public:
  // Constructor
  SimplePID()
    : kp(1), kd(0), ki(0), umax(255), eprev(0.0), integral(0.0) {}

  // A function to set the parameters
  void setParams(float kpIn, float kdIn, float kiIn, float umaxIn) {
    kp = kpIn;
    kd = kdIn;
    ki = kiIn;
    umax = umaxIn;
  }

  // A function to compute the control signal
  void evalu(int value, int target, int &pwr, int pwm_pin) {
    // error
    e = (target - value);
    //Serial.print("value = ");
    //Serial.print(value);
    //Serial.print(" e = ");
    //Serial.print(e);

    // derivative
    float derivative = (e - eprev);
    //Serial.println("e dt");
    //Serial.println(dedt);
    // integral
    integral = integral + e ;
    //Serial.println("e int");
    //Serial.println(eintegral);
    //Serial.println("k1");
    //Serial.println(ki);
    // control signal
    // if (target == 0) {
    //   u = (0.1 * e + kd * derivative);
    //   e = 0;
    //   dedt = 0;
    //   eintegral = 0;
    // } else {
    //   u = (kp * e + kd * dedt + ki * eintegral);
    // }
    u = (kp * e + kd * derivative+ ki * integral);

    //Serial.print(" u = ");
    //Serial.print(u);

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

    // Delay to allow motor to adjust
    delay(10);
  }
};


// Pin definition *********** /
// List of the variables that will be recieved via I2C ****** / byte I2C_OnOff;  //defining the variable that will be sent
char argument;

int PIN_INPUT1 = 3;
int PIN_INPUT2 = 2;

//IR
volatile long int ctr1 = 0, ctr2 = 0;
long int newcount1 = 0, newcount2 = 0;
int s1 = 0, s2 = 0;

long currT,prevT = 0;

//CYTRON
const int m1_dir = 5;
const int m1_pwm = 6;

const int m2_dir = 7;
const int m2_pwm = 9;

int target1=0, target2=0;

int pwr1, pwr2;

SimplePID pid1, pid2;

const int inPin = 8;

// Setup loop *********** /
void setup() {
  Serial.begin(9600);
  pinMode(inPin, INPUT);
  Wire.begin(10);                // Join I2C bus as the slave with address 1
  Wire.onReceive(receiveEvent);  // When the data transmition is detected call receiveEvent function
  Wire.onRequest(requestEvent);
  

  pinMode(PIN_INPUT1, INPUT);
  pinMode(PIN_INPUT2, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_INPUT1), interrupt_routine1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT2), interrupt_routine2, FALLING);

  //Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  //BR->1 BL->2

  pid1.setParams(0.5, 0.1, 0.3, 200);  //Change kP,kD,kI,umax only here
  pid2.setParams(0.5, 0.1, 0.3, 200);  //Change kP,kD,kI,umax only here
  pid1.evalu(s1, target1,pwr1,m1_pwm);
  pid2.evalu(s2, target2,pwr2,m2_pwm);
}

// Main loop *********** /
void loop() {
  // delay(25);
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

  if (currT - prevT > 1000000) {
    noInterrupts();
    newcount1 = ctr1;
    newcount2 = ctr2;
    readspeed();
    prevT = currT;
    ctr1 = 0;
    ctr2 = 0;
    interrupts();
  }

  pid1.evalu(s1, target1, pwr1, m1_pwm);
  pid2.evalu(s2, target2, pwr2, m2_pwm);
}




// Function / Event call ********* /
void receiveEvent() {
  argument = Wire.read();  // Reads the data sent via I2C
  Serial.print("Argument: ");
  Serial.println(argument);
  if (argument == 'f') {  // Forward
    target1 = 20;
    target2 = 20;
    fwd();
  } else if (argument == 'b') {  // Backward
    target1 = 20;
    target2 = 20;
    bkw();
  } else if (argument == 'l') {  // Left
    target1 = 20;
    target2 = 20;
    lt();
  } else if (argument == 'r') {  // Right
    target1 = 20;
    target2 = 20;
    rt();
  } else if (argument == 's') {  //  Stop
    target1 = 0;
    target2 = 0;
    stp();
  } else if (argument == 'c') {  //  Clock
    target1 = 15;
    target2 = 15;
    cw();
  } else if (argument == 'a') {  //  Anti-Clock
    target1 = 15;
    target2 = 15;
    ccw();
  }
  else if (argument == 'F') {  // Forward
    target1 = 30;
    target2 = 30;
    fwd();
  } else if (argument == 'B') {  // Backward
    target1 = 30;
    target2 = 30;
    bkw();
  } else if (argument == 'L') {  // Left
    target1 = 30;
    target2 = 30;
    lt();
  } else if (argument == 'R') {  // Right
    target1 = 30;
    target2 = 30;
    rt();
  }
}

void requestEvent() {
  String str1 = "BR = " + String(s1) + ", BL = " + String(s2);
  // Serial.println(str1);
  str1.toCharArray(SerialData1, 15);
  Wire.write(SerialData1);
}



void interrupt_routine1() {
  ctr1++;
}

void interrupt_routine2() {
  ctr2++;
}

void readspeed() {
  s1 = newcount1;
  Serial.print("BR = ");
  Serial.print(s1);
  // Serial.print(",");
  Serial.print(" PWM1 = ");
  Serial.print(pwr1);
  Serial.print(" Target = ");
  Serial.println(target1);

  s2 = newcount2;
  Serial.print("BL = ");
  Serial.print(s2);
  Serial.print(" PWM2 = ");
  Serial.print(pwr2);
  Serial.print(" Target = ");
  Serial.println(target2);
}


//HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)
//BR->m1 BL->m2
void fwd() {
  //Serial.println("Bot moving forward");
  m1_cwMotor();
  m2_ccwMotor();
}
void bkw() {
  //Serial.println("Bot moving backwards");
  m1_cwMotor();
  m2_cwMotor();
}
void rt() {
  //Serial.println("Bot moving right");
  m1_ccwMotor();
  m2_cwMotor();
}
void lt() {
  //Serial.println("Bot moving left");
  m1_ccwMotor();
  m2_ccwMotor();
}
void cw() {
  //Serial.println("Bot moving clockwise");
  m1_ccwMotor();
  m2_ccwMotor();
}
void ccw() {
  //Serial.println("Bot moving counter-clockwise");
  m1_ccwMotor();
  m2_cwMotor();
}
void stp() {
  //Serial.println("Bot stop");
  m1_stopMotor();
  m2_stopMotor();
}

//LOW LEVEL MOTOR FUNCTIONS

void m1_cwMotor() {
  // Serial.println("Motor A clockwise");
  digitalWrite(m1_dir, HIGH);
  // analogWrite(m1_pwm, 50);
}
void m1_ccwMotor() {
  // Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir, LOW);
  // analogWrite(m1_pwm, 50);
}
void m2_cwMotor() {
  // Serial.println("Motor B clockwise");
  digitalWrite(m2_dir, HIGH);
  // analogWrite(m2_pwm, 50);
}
void m2_ccwMotor() {
  // Serial.println("Motor B counter-clockwise");
  digitalWrite(m2_dir, LOW);
  // analogWrite(m2_pwm, 50);
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