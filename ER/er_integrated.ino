#include <TimerOne.h>
#include <PS4BT.h>  //PS4 Bluetooth
// #include <PS4USB.h>      //PS4 Usb

#include <usbhub.h>  //Comment this for PS4 USB
#include <Servo.h>
#include <Wire.h>  //Arduino library that enables I2C functionality

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//flags
bool f_flag = false;
bool b_flag = false;
bool l_flag = false;
bool r_flag = false;
bool s_flag = false;
bool c_flag = false;
bool a_flag = false;

//cytron - shooter
int m1_dir = 25;  //right motor with behind the shooter - pov
int m2_dir = 23;  //left motor with behind the shooter - pov
int m1_pwm = 9;
int m2_pwm = 8;
int ir_1 = 18;  //left motor
int ir_2 = 19;  //right motor

//y-axis of shooter -- GLA
int enA = 4;
int in1 = 29;
int in2 = 27;

char r;  //to read character input from serial monitor
long long currT = 0;

//slot sensor - shooter
int ctr1 = 0;    //to calculate ticks of motor1
int ctr2 = 0;    //to calculate ticks of motor2
int speed1 = 0;  //to store ticks of motor1
int speed2 = 0;  //to store ticks of motor2
int target = 0;  //Target ticks
int diff = 0;
int factor = 2;

//shooter - motor's pwm
int pwm1 = 0;
int pwm2 = 0;

//piston - shooter
int relay = 47;

// servos for the claw
Servo servo1;
Servo servo2;
int pos1 = 0;
int pos2 = 180;

// cascading lift motor
int inc1 = 33;
int inc2 = 31;
int enac = 5;

// flipping motor
int inf1 = 35;
int inf2 = 37;
int enbf = 4;

// limit switch for flipping
int limit_switch1 = 39;
int limit_switch2 = 40;

//limit switch for cascading lift
int limit_switch3 = 41;

// relay
int relay_pin = 2;

//I2C
String readString;  // defining the string

// List of the variables that will be sent via I2C** /
byte I2C_OnOff;  //defining the variable that will be sent

long long int prevT1 = 0, currT1, prevT2 = 0, currT2;


//Use this for PS4 USB and comment PS4 BT
// USB Usb;
// PS4USB PS4(&Usb);


// Use this (USB Usb to PS4BT PS4(&Btd, PAIR)) for PS4 BT and comment PS4 USB
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so

// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);


//bool printAngle, printTouch;
//uint8_t oldL2Value, oldR2Value;

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  //
  //slot sensors for shooter
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);
  //cytron-shooter
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
  digitalWrite(m1_dir, LOW);
  digitalWrite(m2_dir, LOW);
  //piston shooter
  pinMode(relay, OUTPUT);

  Timer1.initialize();
  Timer1.attachInterrupt(readmotor);
  //interrupt fo slot sensor - shooter
  attachInterrupt(digitalPinToInterrupt(ir_1), interrupt_routine1, RISING);
  attachInterrupt(digitalPinToInterrupt(ir_2), interrupt_routine2, RISING);

  //L298N - yaxis
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(enA, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  //l298n - claw
  pinMode(inc1, OUTPUT);
  pinMode(inc2, OUTPUT);
  pinMode(enac, OUTPUT);
  digitalWrite(inc1, LOW);
  digitalWrite(inc2, LOW);
  analogWrite(enac, 255);

  //l298n - flipping
  pinMode(inf1, OUTPUT);
  pinMode(inf2, OUTPUT);
  pinMode(enbf, OUTPUT);
  digitalWrite(inf1, LOW);
  digitalWrite(inf2, LOW);
  analogWrite(enbf, 255);

  // limit switches for flipping
  pinMode(limit_switch1, INPUT);
  pinMode(limit_switch2, INPUT);
  // limit switch for cascading lift
  pinMode(limit_switch3, INPUT);

  //relay for piston
  pinMode(relay_pin, OUTPUT);

  //Motors off initially


  //I2C
  Wire.begin();  // join I2C bus (address here is optional for master)

  Serial.begin(115200);

#if !defined(MIPSEL)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {

    // currT1 = micros();
    // currT2 = micros();

    //shooter
    if (PS4.getAnalogHat(RightHatY) < 255 || PS4.getAnalogHat(RightHatY) > 210)  //shooter up by 1 degree - test ek baar
    {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      delay(500);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }

    //shooter
    else if (PS4.getAnalogHat(RightHatY) > -255 || PS4.getAnalogHat(RightHatY) < -210)  //shooter down by 1 degree - test ek baar
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      delay(500);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }


    // shooter
    // else if (PS4.getButtonClick(L3))  //start stop flywheel
    // {
    //   Serial.print(F("\r\nL3"));
    //   if (target == 0) {
    //     target = 45;
    //   } else {
    //     //  target = 0;
    //     pwm1 = 0;
    //     pwm2 = 0;
    //     analogWrite(m1_pwm, 0);
    //     analogWrite(m2_pwm, 0);
    //   }
    // }
    // else if(PS4.getButtonClick(CROSS))
    // {
    //    target = 0;
    //         pwm1=0;
    //         pwm2=0;
    //         analogWrite(m1_pwm,0);
    //         analogWrite(m2_pwm,0);

    // }
    if (PS4.getButtonClick(R3))  //shoot the ring using piston
    {
      Serial.print(F("\r\nR3"));
      digitalWrite(relay, HIGH);
      // delay(500);
      digitalWrite(relay, LOW);
      Serial.println("SHOT!!");
      // moves the servos on the claw
      for (pos1 = 0, pos2 = 180; pos1 <= 180, pos2 >= 0; pos1 += 1, pos2 -= 1) {
        servo1.write(pos1);
        servo2.write(pos2);
        // delay(15);
      }
    }

    //claw
    if (PS4.getButtonClick(CIRCLE))  //open and close claw
    {
      Serial.print(F("\r\nCircle"));
      if (relay_pin == 1) {
        digitalWrite(relay_pin, LOW);
      } else if (relay_pin == 0) {
        digitalWrite(relay_pin, HIGH);
      }
    }

    else if (PS4.getButtonPress(L1) && PS4.getButtonPress(TRIANGLE))  //lift down
    {
      cascading_lift_clock();
      analogWrite(enac, 255);
      digitalWrite(inc1, HIGH);
      digitalWrite(inc2, LOW);
    }

    else if (PS4.getButtonPress(TRIANGLE))  //lift up
    {
      Serial.print(F("\r\nTriangle"));
      cascading_lift_anticlock();
      digitalWrite(inc1, LOW);
      digitalWrite(inc2, HIGH);
      if (digitalRead(limit_switch3) == HIGH)  // shut the motor when cascading lift reaches  max hieght
      {
        digitalWrite(inc1, LOW);
        digitalWrite(inc2, LOW);
      }

      else {
        digitalWrite(inc1, LOW);
        digitalWrite(inc2, LOW);
      }
    }

    //flip the claw
    else if (PS4.getButtonPress(L1) && PS4.getButtonPress(SQUARE))  //clock
    {
      flip_clock();
      analogWrite(enbf, 255);
      digitalWrite(inf1, HIGH);
      digitalWrite(inf2, LOW);
    }

    else if (PS4.getButtonPress(SQUARE))  //anticlock
    {
      Serial.print(F("\r\nSquare"));
      flip_anticlock();

      digitalWrite(inf1, LOW);
      digitalWrite(inf2, HIGH);
      if (digitalRead(limit_switch2) == HIGH and digitalRead(limit_switch1 == HIGH))  // run the following when claw completes 180 degrees
      {
        digitalWrite(inf1, LOW);  // shut the flipping motor
        digitalWrite(inf2, LOW);
      } else {
        digitalWrite(inf1, LOW);
        digitalWrite(inf2, LOW);
      }
    }

    // Target 35

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(UP)) {
      //              Serial.println("UP1");
      // Serial2.write("f");
      // delay(100);
      if (!f_flag)  //send only once string f
      {
        Serial.println("UP1");
        I2C_OnOff = 'F';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        f_flag = true;
        // delay(100);
      }

      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(DOWN)) {
      // Serial.println("DOWN1");
      // Serial2.write("b");
      // delay(100);
      if (!b_flag) {
        Serial.println("DOWN1");
        I2C_OnOff = 'B';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        b_flag = true;
        // delay(100);
      }
      f_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(LEFT)) {
      // Serial.println("LEFT");
      // Serial2.write("l");
      // delay(100);
      if (!l_flag) {
        Serial.println("LEFT1");
        I2C_OnOff = 'L';


        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        l_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(RIGHT)) {
      // Serial.println("RIGHT");
      // Serial2.write("r");
      // delay(100);
      if (!r_flag) {
        Serial.println("RIGHT1");
        I2C_OnOff = 'R';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        r_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(L1)) {
      // Serial.println("L1");
      // Serial2.write("a");
      // delay(100);
      if (!a_flag) {
        Serial.println("L1-1");
        I2C_OnOff = 'A';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        a_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(R1)) {
      // Serial.println("R1");
      // Serial2.write("c");
      // delay(100);
      if (!c_flag) {
        Serial.println("R1-1");
        I2C_OnOff = 'C';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        c_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      a_flag = false;
    }

    //Target 25

    //Serial.println("In");
    else if (PS4.getButtonPress(UP)) {
      Serial.println("UP");
      // Serial2.write("f");
      // delay(100);
      if (!f_flag) {  //send only once string f
        // Serial.println("UP");
        I2C_OnOff = 'f';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        f_flag = true;
        // delay(100);
      }

      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(DOWN)) {
      Serial.println("DOWN");
      // Serial2.write("b");
      // delay(100);
      if (!b_flag) {
        // Serial.println("DOWN");
        I2C_OnOff = 'b';


        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        b_flag = true;
        // delay(100);
      }
      f_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(LEFT)) {
      Serial.println("LEFT");
      // Serial2.write("l");
      // delay(100);
      if (!l_flag) {
        // Serial.println("LEFT");
        I2C_OnOff = 'l';


        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        l_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(RIGHT)) {
      Serial.println("RIGHT");
      // Serial2.write("r");
      // delay(100);
      if (!r_flag) {
        // Serial.println("RIGHT");
        I2C_OnOff = 'r';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        r_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(L1)) {
      Serial.println("L1");
      // Serial2.write("a");
      // delay(100);
      if (!a_flag) {
        // Serial.println("L1");
        I2C_OnOff = 'a';


        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        a_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
    }

    else if (PS4.getButtonPress(R1)) {
      Serial.println("R1");
      // Serial2.write("c");
      // delay(100);
      if (!c_flag) {
        // Serial.println("R1");
        I2C_OnOff = 'c';


        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        c_flag = true;
        // delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      a_flag = false;
    } else {
      Serial.println("Stop");
      // Serial2.write("s");
      // delay(100);
      if (!s_flag) {
        // Serial.println("Stop");
        I2C_OnOff = 's';

        Wire.beginTransmission(9);   // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        Wire.beginTransmission(10);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closing the transmission channel
        s_flag = true;
        // delay(100);
      }
      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      c_flag = false;
      a_flag = false;
    }
  }
}

void interrupt_routine1()  //slot sensor - shooter
{
  ctr1++;
}

void interrupt_routine2()  //slot sensor - shooter
{
  ctr2++;
}

void readmotor()  // shooter
{
  //    Serial.print("Time");
  //    Serial.println(millis());
  speed1 = ctr1;
  speed2 = ctr2;
  Serial.print("Motor1 ");
  Serial.println(speed1);
  Serial.print("Motor2 ");
  Serial.println(speed2);
  Serial.print("PWM1: ");
  Serial.println(pwm1);
  Serial.print("PWM2: ");
  Serial.println(pwm2);

  set_pwm1();
  set_pwm2();

  analogWrite(m1_pwm, pwm1);
  analogWrite(m2_pwm, pwm2);
  ctr1 = 0;
  ctr2 = 0;
}

int find_factor1()  //shooter
{
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

int find_factor2()  //shooter
{
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

void set_pwm1()  //shooter
{
  factor = find_factor1();
  //adjusting pwm of motor1
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

void set_pwm2()  // shooter
{
  factor = find_factor2();
  //adjusting pwm of motor2
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

void cascading_lift_clock()  // Downward MOtion
{
  analogWrite(enac, 255);
  digitalWrite(inc1, HIGH);
  digitalWrite(inc2, LOW);
}
void cascading_lift_anticlock()  // upward motion
{
  analogWrite(enac, 255);
  digitalWrite(inc1, LOW);
  digitalWrite(inc2, HIGH);
}

void flip_anticlock() {
  analogWrite(enbf, 255);
  digitalWrite(inf1, LOW);
  digitalWrite(inf2, HIGH);
}
void flip_clock() {
  analogWrite(enbf, 255);
  digitalWrite(inf1, HIGH);
  digitalWrite(inf2, LOW);
}