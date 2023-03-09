/*
 Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif 
#include <SPI.h>

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
PS4USB PS4(&Usb);  // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode


// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

#include <TimerOne.h>
//Encoder
#define e1PinA 3

#define e2PinA 2

#define e3PinA 18

#define e4PinA 20

volatile long long int e1Pos = 0, e2Pos = 0, e3Pos = 0, e4Pos = 0;
long newposition1, newposition2, newposition3, newposition4;
long oldposition1 = 0, oldposition2 = 0, oldposition3 = 0, oldposition4 = 0;

long s1, s2, s3, s4;

//Cytron
#define m4_dir 53
#define m1_dir 51
#define m4_pwm 4
#define m1_pwm 5

#define m2_dir 47
#define m3_dir 49
#define m2_pwm 7
#define m3_pwm 6

int M1_PWM = 20;
int M2_PWM = 20;
int M3_PWM = 20;
int M4_PWM = 20;

void setup() {
  //Encoder
  //Motor 1
  pinMode(e1PinA, INPUT);
  digitalWrite(e1PinA, HIGH);  // turn on pullup resistor

  //Motor 2
  pinMode(e2PinA, INPUT);
  digitalWrite(e2PinA, HIGH);  // turn on pullup resistor

  //Motor 3
  pinMode(e3PinA, INPUT);
  digitalWrite(e3PinA, HIGH);  // turn on pullup resistor

  //Motor 4
  pinMode(e4PinA, INPUT);
  digitalWrite(e4PinA, HIGH);  // turn on pullup resistor


  Timer1.initialize(1000000 / 2);
  Timer1.attachInterrupt(readspeed);

  attachInterrupt(digitalPinToInterrupt(3), doe1, RISING);
  attachInterrupt(digitalPinToInterrupt(2), doe2, RISING);
  attachInterrupt(digitalPinToInterrupt(18), doe3, RISING);
  attachInterrupt(digitalPinToInterrupt(20), doe4, RISING);

  //Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
  pinMode(m3_dir, OUTPUT);
  pinMode(m3_pwm, OUTPUT);
  pinMode(m4_dir, OUTPUT);
  pinMode(m4_pwm, OUTPUT);
  digitalWrite(m1_dir, HIGH);
  digitalWrite(m2_dir, LOW);
  digitalWrite(m3_dir, HIGH);
  digitalWrite(m4_dir, LOW);
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    //Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.println(F("\r\nPS4 USB Library Started"));
}
void loop() {
  Usb.Task();

  if (PS4.connected()) {

    if (PS4.getButtonClick(L1)) {
      m1u();
      m2u();
    }

    // if (PS4.getButtonClick(L1)) {
    //   m1d();
    // }

    if (PS4.getButtonClick(L2)) {
      m1d();
      m2d();
    }

    // if (PS4.getButtonClick(L2) && PS4.getButtonClick(DOWN)) {
    //   m2d();
    // }

    if (PS4.getButtonClick(R1)) {
      m3u();
      m4u();
    }

    // if (PS4.getButtonClick(R1) && PS4.getButtonClick(DOWN)) {
    //   m4d();
    // }

    // if (PS4.getButtonClick(R2) && PS4.getButtonClick(UP)) {
    //   m3u();
    // }

    if (PS4.getButtonClick(R2)) {
      m3d();
      m4d();
    }

    if (PS4.getButtonClick(CIRCLE)) {
      cw();
    }

    if (PS4.getButtonClick(SQUARE)) {
      ccw();
    }

    if (PS4.getButtonClick(CROSS)) {
      stp();
    }

    if (PS4.getButtonClick(UP)) {
      fwd();
    }
    if (PS4.getButtonClick(RIGHT)) {
      rt();
    }
    if (PS4.getButtonClick(DOWN)) {
      bkw();
    }
    if (PS4.getButtonClick(LEFT)) {
      lt();
    }

    if (PS4.getButtonClick(SHARE))
      Serial.print(F("\r\nShare"));
  }
  newposition1 = e1Pos;

  newposition2 = e2Pos;

  newposition3 = e3Pos;

  newposition4 = e4Pos;
}

void doe1() {
  e1Pos++;
}

void doe2() {
  e2Pos++;
}

void doe3() {
  e3Pos++;
}

void doe4() {
  e4Pos++;
}


void readspeed() {
  s1 = (newposition1 - oldposition1);
  Serial.print("Speed1 = ");
  Serial.println(s1);
  oldposition1 = newposition1;

  s2 = (newposition2 - oldposition2);
  Serial.print("Speed2 = ");
  Serial.println(s2);
  oldposition2 = newposition2;

  s3 = (newposition3 - oldposition3);
  Serial.print("speed3 = ");
  Serial.println(s3);
  oldposition3 = newposition3;

  s4 = (newposition4 - oldposition4);
  Serial.print("speed4 = ");
  Serial.println(s4);
  oldposition4 = newposition4;
}

//HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)

void fwd() {
  //Serial.println("Bot moving forward");
  acwMotor();
  bcwMotor();
  ccwMotor();
  dcwMotor();
}
void bkw() {
  //Serial.println("Bot moving backwards");
  accwMotor();
  bccwMotor();
  cccwMotor();
  dccwMotor();
}
void rt() {
  //Serial.println("Bot moving right");
  acwMotor();
  bccwMotor();
  ccwMotor();
  dccwMotor();
}
void lt() {
  //Serial.println("Bot moving left");
  accwMotor();
  bccwMotor();
  cccwMotor();
  dcwMotor();
}
void cw() {
  //Serial.println("Bot moving clockwise");
  accwMotor();
  bccwMotor();
  cccwMotor();
  dccwMotor();
}
void ccw() {
  //Serial.println("Bot moving counter-clockwise");
  acwMotor();
  bcwMotor();
  ccwMotor();
  dcwMotor();
}
void stp() {
  //Serial.println("Bot stop");
  astopMotor();
  bstopMotor();
  cstopMotor();
  dstopMotor();
}

//LOW LEVEL MOTOR FUNCTIONS

void acwMotor() {
  //Serial.println("Motor A clockwise");
  digitalWrite(m1_dir, HIGH);
  analogWrite(m1_pwm, M1_PWM);
}
void accwMotor() {
  //Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir, LOW);
  analogWrite(m1_pwm, M1_PWM);
}
void bcwMotor() {
  //Serial.println("Motor B clockwise");
  digitalWrite(m2_dir, HIGH);
  analogWrite(m2_pwm, M2_PWM);
}
void bccwMotor() {
  //.println("Motor B counter-clockwise");
  digitalWrite(m2_dir, LOW);
  analogWrite(m2_pwm, M2_PWM);
}
void ccwMotor() {
  //Serial.println("Motor C clockwise");
  digitalWrite(m3_dir, HIGH);
  analogWrite(m3_pwm, M3_PWM);
}
void cccwMotor() {
  //Serial.println("Motor C counter-clockwise");
  digitalWrite(m3_dir, LOW);
  analogWrite(m3_pwm, M3_PWM);
}
void dcwMotor() {
  //Serial.println("Motor D clockwise");
  digitalWrite(m4_dir, HIGH);
  analogWrite(m4_pwm, M4_PWM);
}
void dccwMotor() {
  //Serial.println("Motor D counter-clockwise");
  digitalWrite(m4_dir, LOW);
  analogWrite(m4_pwm, M4_PWM);
}
void astopMotor() {
  //Serial.println("Motor A stop");
  analogWrite(m1_pwm, 0);
}
void bstopMotor() {
  //Serial.println("Motor B stop");
  analogWrite(m2_pwm, 0);
}
void cstopMotor() {
  //Serial.println("Motor C stop");
  analogWrite(m3_pwm, 0);
}
void dstopMotor() {
  //Serial.println("Motor D stop");
  analogWrite(m4_pwm, 0);
}

//PWM functions
void m1u() {
  M1_PWM += 10;
  analogWrite(m1_pwm, M1_PWM);
}

void m1d() {
  M1_PWM -= 10;
  analogWrite(m1_pwm, M1_PWM);
}

void m2u() {
  M2_PWM += 10;
  analogWrite(m2_pwm, M2_PWM);
}

void m2d() {
  M2_PWM -= 10;
  analogWrite(m2_pwm, M2_PWM);
}

void m3u() {
  M3_PWM += 10;
  analogWrite(m3_pwm, M3_PWM);
}

void m3d() {
  M3_PWM -= 10;
  analogWrite(m3_pwm, M3_PWM);
}

void m4u() {
  M4_PWM += 10;
  analogWrite(m4_pwm, M4_PWM);
}

void m4d() {
  M4_PWM -= 10;
  analogWrite(m4_pwm, M4_PWM);
}