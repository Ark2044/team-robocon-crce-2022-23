//RR Integrated 2.0
#include <TimerOne.h>
#include <mcp2515.h>

#include <PS4BT.h>  //PS4 Bluetooth
//#include <PS4USB.h>      //PS4 Usb

#include <usbhub.h>  //Comment this for PS4 USB
#include <Servo.h>
#include <Wire.h>  //Arduino library that enables I2C functionality

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//cytron - shooter
const int m1_dir = 29;  //left motor with behind the shooter - pov
const int m1_pwm = 4;
const int m2_dir = 27;  //right motor with behind the shooter - pov
const int m2_pwm = 3;

const int ir_1 = 18;  //left motor
const int ir_2 = 19;  //right motor

long long currT = 0;

//slot sensor - shooter
int ctr1 = 0;    //to calculate ticks of motor1
int ctr2 = 0;    //to calculate ticks of motor2
int speed1 = 0;  //to store ticks of motor1
int speed2 = 0;  //to store ticks of motor2
int target = 0;  //Target ticks
int diff = 0;
int factor = 2;
int flag = 0;

//shooter - motor's pwm
int pwm1 = 0;
int pwm2 = 0;

//piston - shooter
const int relay = 2;

// servos for the claw
Servo servo1;
Servo servo2;
const int s1 = 11;
const int s2 = 12;

// flipping motor using l298n
const int inf1 = 35;
const int inf2 = 37;
const int enbf = 4;

// limit switch for flipping
// const int limit_switch1 = 39;
// const int limit_switch2 = 40;

// relay claw
const int relay_pin = 47;

//Automation pins
// const auto_pin1 = ;
// const auto_pin2 = ;
// const auto_pin3 = ;

//Canbus
struct can_frame canMsg1;
struct can_frame canMsg2;
byte mega_data;
// Define the CS pin each CAN bus shield/module
MCP2515 mcp2515(53);


//Use this for PS4 USB and comment PS4 BT
//USB Usb;
//PS4USB PS4(&Usb);

// Use this (USB Usb to PS4BT PS4(&Btd, PAIR)) for PS4 BT and comment PS4 USB
USB Usb;
BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so
                // // You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;


/*********** slot sensor - shooter *********/
void interrupt_routine1() {
  ctr1++;
}

void interrupt_routine2() {
  ctr2++;
}
/*********** slot sensor - shooter *********/

/*********** shooter ************/
void readmotor() {
  //    Serial.print("Time");
  //    Serial.println(millis());
  speed1 = ctr1;
  speed2 = ctr2;
  Serial.print("Motor1 ");
  Serial.print(speed1);
  Serial.print("  PWM1: ");
  Serial.println(pwm1);
  Serial.print("Motor2 ");
  Serial.print(speed2);
  Serial.print("  PWM2: ");
  Serial.println(pwm2);

  set_pwm1();
  set_pwm2();

  analogWrite(m1_pwm, pwm1);
  analogWrite(m2_pwm, pwm2);
  ctr1 = 0;
  ctr2 = 0;
}

int find_factor1() {
  diff = abs(ctr1 - target);
  if (diff >= 15) {
    return 30;
  } else if (diff >= 10) {
    return 20;
  } else if (diff >= 5) {
    return 10;
  } else if (diff >= 3) {
    return 5;
  } else if (diff >= 2) {
    return 3;
  } else if (diff >= 1) {
    return 1;
  } else {
    return 0;
  }
}

int find_factor2() {
  diff = abs(ctr2 - target);
  if (diff >= 15) {
    return 30;
  } else if (diff >= 10) {
    return 20;
  } else if (diff >= 5) {
    return 10;
  } else if (diff >= 3) {
    return 5;
  } else if (diff >= 2) {
    return 3;
  } else if (diff >= 1) {
    return 1;
  } else {
    return 0;
  }
}
/*********** shooter ************/

/*********** shooter adjusting pwm of motor1 ************/
void set_pwm1() {
  factor = find_factor1();
  if (speed1 < target - 1 && pwm1 + factor <= 250) {
    pwm1 += factor;
  } else if (speed1 > target + 1 && pwm1 - factor >= 0) {
    Serial.println("Bye");
    pwm1 -= factor;
  } else if (target == 0) {
    pwm1 = 0;
  } else {
    pwm1 = pwm1;
  }
}
/*********** shooter adjusting pwm of motor1 ************/

/*********** shooter adjusting pwm of motor2 ************/
void set_pwm2() {
  factor = find_factor2();
  if (speed2 < target - 1 && pwm2 + factor <= 250) {
    pwm2 += factor;
  } else if (speed2 > target + 1 && pwm2 - factor >= 0) {
    Serial.println("I'm dying");
    pwm2 -= factor;
  } else if (target == 0) {
    pwm2 = 0;
  } else {
    pwm2 = pwm2;
  }
}
/*********** shooter adjusting pwm of motor2 ************/

/*********** claw ************/
void flip_anticlock() {
  // analogWrite(enbf, 255);
  // digitalWrite(inf1, LOW);
  // digitalWrite(inf2, HIGH);
}
void flip_clock() {
  // analogWrite(enbf, 255);
  // digitalWrite(inf1, HIGH);
  // digitalWrite(inf2, LOW);
}
/*********** claw ************/

void setup() {
  //slot sensors for shooter
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);

  //cytron-shooter
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  digitalWrite(m1_dir, LOW);
  digitalWrite(m2_dir, HIGH);

  analogWrite(m1_pwm, 0);
  analogWrite(m2_pwm, 0);

  //piston shooter
  pinMode(relay, OUTPUT);

//  Timer1.initialize();
//  Timer1.attachInterrupt(readmotor);

  //interrupt fo slot sensor - shooter
  attachInterrupt(digitalPinToInterrupt(ir_1), interrupt_routine1, FALLING);
  attachInterrupt(digitalPinToInterrupt(ir_2), interrupt_routine2, FALLING);

  //l298n - claw
  // pinMode(inc1, OUTPUT);
  // pinMode(inc2, OUTPUT);
  // pinMode(enac, OUTPUT);
  // digitalWrite(inc1, LOW);
  // digitalWrite(inc2, LOW);
  // analogWrite(enac, 255);

  //l298n - flipping
  pinMode(inf1, OUTPUT);
  pinMode(inf2, OUTPUT);
  pinMode(enbf, OUTPUT);
  // digitalWrite(inf1, LOW);
  // digitalWrite(inf2, LOW);
  // analogWrite(enbf, 255);

  // limit switches for flipping
  // pinMode(limit_switch1, INPUT);
  // pinMode(limit_switch2, INPUT);
  // limit switch for cascading lift
  // pinMode(limit_switch3, INPUT);

  //relay for piston
  pinMode(relay_pin, OUTPUT);

  //Automation
  // pinMode(auto_pin1, INPUT);
  // pinMode(auto_pin2, INPUT);
  // pinMode(auto_pin3, INPUT);

  Serial.begin(115200);

  //CANBUS
  canMsg1.can_id = 0x0F6;
  canMsg1.can_dlc = 1;

  // canMsg2.can_id = 0x036;
  // canMsg2.can_dlc = 1;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

#if !defined(MIPSEL)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.println(F("\r\nPS4 Bluetooth Library Started"));
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    int right_x = map(PS4.getAnalogHat(RightHatX), 0, 255, -255, 255);
    int right_y = map(PS4.getAnalogHat(RightHatY), 0, 255, 255, -255);

    /*********** flywheel ************/
    if (PS4.getButtonPress(L3)) {
//      flag = 1;
      Serial.print("Start Flywheel");
      //target = 45;
      analogWrite(m1_pwm, 135);
      analogWrite(m2_pwm, 135);
    }

    if (PS4.getButtonPress(R3))  //shoot the ring using piston
    {
      Serial.print("Shoot Ring");
      digitalWrite(relay, HIGH);
      Serial.println("SHOT!!");

    } else  //retract piston
    {
      Serial.println("Retract Piston");
      digitalWrite(relay, LOW);
    }

    // if (PS4.getButtonPress(R2))  //servo
    // {
    //   servo1.attach(s1);
    //   servo2.attach(s2);
    //   for (int i = 0; i <= 65; i++) {
    //     Serial.println(i);
    //     servo1.writeMicroseconds(2000);
    //     Serial.println(i);
    //     servo2.writeMicroseconds(1000);
    //   }
    //   servo1.detach();
    //   servo2.detach();
    // }

    if (PS4.getButtonPress(CROSS))  //stop the flywheel
    {
      Serial.println("Shooter stop");
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 0);
    }
    /*********** flywheel ************/

    /*********** claw ************/
    // if (PS4.getButtonPress(CIRCLE))  //open claw
    // {
    //   Serial.print("Open Claw");
    //   digitalWrite(relay_pin, LOW);
    // } else  //close claw
    // {
    //   Serial.println("Close Claw");
    //   digitalWrite(relay_pin, HIGH);
    // }

    // //flip the claw
    // if (PS4.getButtonPress(L2) && PS4.getButtonPress(SQUARE))  //clock
    // {
    //   Serial.println("flip claw anticlockwise");
    //   flip_clock();
    // }

    // else if (PS4.getButtonPress(SQUARE))  //anticlock
    // {
    //   Serial.println("Flip Claw Clockwise");
    //   flip_anticlock();
    //   //      if (digitalRead(limit_switch2) == HIGH and digitalRead(limit_switch1 == HIGH))  // run the following when claw completes 180 degrees
    //   //      {
    //   //        digitalWrite(inf1, LOW);  // shut the flipping motor
    //   //        digitalWrite(inf2, LOW);
    //   //      }
    // } else {
    //   // Serial.println("Stop Claw");
    //   digitalWrite(inf1, LOW);
    //   digitalWrite(inf2, LOW);
    // }
    /*********** claw ************/

    /*********** Drive ************/

    // Target 25

    if (PS4.getButtonPress(L2) && PS4.getButtonPress(UP)) {
      Serial.println("Forward: target 25");
      mega_data = 'F';
      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(DOWN)) {
      Serial.println("Backward: target 25");
      mega_data = 'B';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(LEFT)) {
      Serial.println("LEFT: : target 25");
      mega_data = 'L';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(RIGHT)) {
      Serial.println("RIGHT: : target 25");
      mega_data = 'R';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    //Target 20

    //Serial.println("In");
    else if (PS4.getButtonPress(UP)) {
      Serial.println("UP: target 20");
      mega_data = 'f';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(DOWN)) {
      Serial.println("DOWN: target 20");
      mega_data = 'b';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(LEFT)) {
      Serial.println("LEFT: target 20");
      mega_data = 'l';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(RIGHT)) {
      Serial.println("RIGHT: target 20");
      mega_data = 'r';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(L1)) {
      Serial.println("Anticlockwise: target 15");
      mega_data = 'a';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }

    else if (PS4.getButtonPress(R1)) {
      Serial.println("Clockwise: target 15");
      mega_data = 'c';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    } else {
      Serial.println("Stop");
      mega_data = 's';

      canMsg1.data[0] = mega_data;
      mcp2515.sendMessage(&canMsg1);
      // canMsg2.data[0] = mega_data;
      // mcp2515.sendMessage(&canMsg2);
    }
    /*********** Drive ************/

    /*********** Automation ************/
    // if (PS4.getButtonPress(L2) && PS4.getButtonPress(TRIANGLE)) {
    //   if (digitalRead(auto_pin1) ==  && digitalRead(auto_pin2) == HIGH) {
    //     Serial.println("Turn Right");
    //   } else if (digitalRead(auto_pin1) == HIGH && digitalRead(auto_pin2) == LOW) {
    //     Serial.println("Turn Left");
    //   } else if (digitalRead(auto_pin1) == HIGH && digitalRead(auto_pin2) == HIGH) {
    //     Serial.println("Go Straight");
    //   } else if (digitalRead(auto_pin1) == LOW && digitalRead(auto_pin2) == LOW) {
    //     Serial.println("Do nothing");
    //   }
    // }

    /*********** Automation ************/
  }
}