// RR Integrated I2C
#include <TimerOne.h>

#include <PS4BT.h>  //PS4 Bluetooth
#include <usbhub.h> //Comment this for PS4 USB

// #include <PS4USB.h>      //PS4 Usb

#include <Servo.h>
#include <Wire.h> //Arduino library that enables I2C functionality

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// cytron - shooter
const int m1_dir = 29; // left motor with behind the shooter - pov
const int m1_pwm = 4;
const int m2_dir = 27; // right motor with behind the shooter - pov
const int m2_pwm = 3;

const int ir_1 = 18; // left motor
const int ir_2 = 19; // right motor

long long currT = 0;

// slot sensor - shooter
int ctr1 = 0;   // to calculate ticks of motor1
int ctr2 = 0;   // to calculate ticks of motor2
int speed1 = 0; // to store ticks of motor1
int speed2 = 0; // to store ticks of motor2
int target = 0; // Target ticks
int diff = 0;
int factor = 2;
int flag = 0;

// shooter - motor's pwm
int pwm1 = 0;
int pwm2 = 0;
int pwm_shooter = 85;

// piston - shooter
const int relay_shooter = 10;
const int relay_shooter2 = 28;
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
const int relay_claw = 47;

// Automation pins
const int auto_pin1 = 41;
const int auto_pin2 = 42;
const int auto_pin3 = 43;

// I2C
char mega_data;

/*********** PS4 USB *********/
// Use this for PS4 USB and comment PS4 BT
// USB Usb;
// PS4USB PS4(&Usb);
/*********** PS4 USB *********/

/*********** PS4 BT *********/
// Use this for PS4 BT and comment PS4 USB
USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS4BT PS4(&Btd, PAIR);
/*********** PS4 USB *********/

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

/*********** slot sensor - shooter *********/
void interrupt_routine1()
{
  ctr1++;
}

void interrupt_routine2()
{
  ctr2++;
}
/*********** slot sensor - shooter *********/

/*********** shooter ************/
void readmotor()
{
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

int find_factor1()
{
  diff = abs(ctr1 - target);
  if (diff >= 15)
  {
    return 30;
  }
  else if (diff >= 10)
  {
    return 20;
  }
  else if (diff >= 5)
  {
    return 10;
  }
  else if (diff >= 3)
  {
    return 5;
  }
  else if (diff >= 2)
  {
    return 3;
  }
  else if (diff >= 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int find_factor2()
{
  diff = abs(ctr2 - target);
  if (diff >= 15)
  {
    return 30;
  }
  else if (diff >= 10)
  {
    return 20;
  }
  else if (diff >= 5)
  {
    return 10;
  }
  else if (diff >= 3)
  {
    return 5;
  }
  else if (diff >= 2)
  {
    return 3;
  }
  else if (diff >= 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
/*********** shooter ************/

/*********** Adjusting pwm of left motor1 ************/
void set_pwm1()
{
  factor = find_factor1();
  if (speed1 < target - 1 && pwm1 + factor <= 250)
  {
    pwm1 += factor;
  }
  else if (speed1 > target + 1 && pwm1 - factor >= 0)
  {
    Serial.println("Bye");
    pwm1 -= factor;
  }
  else if (target == 0)
  {
    pwm1 = 0;
  }
  else
  {
    pwm1 = pwm1;
  }
}
/*********** Adjusting pwm of left motor1 ************/

/*********** Adjusting pwm of right motor2 ************/
void set_pwm2()
{
  factor = find_factor2();
  if (speed2 < target - 1 && pwm2 + factor <= 250)
  {
    pwm2 += factor;
  }
  else if (speed2 > target + 1 && pwm2 - factor >= 0)
  {
    Serial.println("I'm dying");
    pwm2 -= factor;
  }
  else if (target == 0)
  {
    pwm2 = 0;
  }
  else
  {
    pwm2 = pwm2;
  }
}
/*********** Adjusting pwm of right motor2 ************/

/*********** claw ************/
void flip_anticlock()
{
  // analogWrite(enbf, 255);
  // digitalWrite(inf1, LOW);
  // digitalWrite(inf2, HIGH);
}
void flip_clock()
{
  // analogWrite(enbf, 255);
  // digitalWrite(inf1, HIGH);
  // digitalWrite(inf2, LOW);
}
/*********** claw ************/

/*********** Drive ************/
void I2C_func(char x)
{
  Wire.beginTransmission(9);  // Opening the transmission channel to device with the name 1
  Wire.write(x);              // Sending the desired information via I2C to the slave device
  Wire.endTransmission();     // Closing the transmission channel
  Wire.beginTransmission(10); // Opening the transmission channel to device with the name 2
  Wire.write(x);              // Sending the desired information via I2C to the slave device
  Wire.endTransmission();     // Closing the transmission channel
}
/*********** Drive ************/

void setup()
{
  // slot sensors for shooter
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);

  // cytron-shooter
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  digitalWrite(m1_dir, HIGH); // left
  digitalWrite(m2_dir, HIGH); // right

  analogWrite(m1_pwm, 0);
  analogWrite(m2_pwm, 0);

  // piston shooter
  pinMode(relay_shooter, OUTPUT);
  pinMode(relay_shooter2, OUTPUT);

  //  Timer1.initialize();
  //  Timer1.attachInterrupt(readmotor);

  // interrupt fo slot sensor - shooter
  attachInterrupt(digitalPinToInterrupt(ir_1), interrupt_routine1, FALLING);
  attachInterrupt(digitalPinToInterrupt(ir_2), interrupt_routine2, FALLING);

  // l298n - claw
  //  pinMode(inc1, OUTPUT);
  //  pinMode(inc2, OUTPUT);
  //  pinMode(enac, OUTPUT);
  //  digitalWrite(inc1, LOW);
  //  digitalWrite(inc2, LOW);
  //  analogWrite(enac, 255);

  // l298n - flipping
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

  // relay for claw
  pinMode(relay_claw, OUTPUT);

  // Automation
  //  pinMode(auto_pin1, INPUT);
  //  pinMode(auto_pin2, INPUT);
  //  pinMode(auto_pin3, INPUT);

  // I2C
  Wire.setClock(400000L);
  Wire.begin(); // join I2C bus (address here is optional for master)

  Serial.begin(115200);

#if !defined(MIPSEL)
  while (!Serial)
    ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1)
  {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ; // Halt
  }
  Serial.println(F("\r\nPS4 Bluetooth Library Started"));
}

void loop()
{
  Usb.Task();

  if (PS4.connected())
  {
    int right_x = map(PS4.getAnalogHat(RightHatX), 0, 255, -255, 255);
    int right_y = map(PS4.getAnalogHat(RightHatY), 0, 255, 255, -255);

    /*********** flywheel ************/

    if (PS4.getButtonPress(L3) && PS4.getButtonClick(TRIANGLE))
    {
      pwm_shooter += 5;
      if (pwm_shooter >= 255)
      {
        pwm_shooter = 255;
      }
      Serial.print("Shooter PWM");
      Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    if (PS4.getButtonPress(L3) && PS4.getButtonClick(CROSS))
    {
      pwm_shooter -= 5;
      if (pwm_shooter <= 0)
      {
        pwm_shooter = 0;
      }
      Serial.print("Shooter PWM");
      Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    if (PS4.getButtonPress(L3) && PS4.getButtonClick(SQUARE))
    {
      pwm_shooter += 1;
      if (pwm_shooter >= 255)
      {
        pwm_shooter = 255;
      }
      Serial.print("Shooter PWM");
      Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    if (PS4.getButtonPress(L3) && PS4.getButtonClick(CIRCLE))
    {
      pwm_shooter -= 1;
      if (pwm_shooter <= 0)
      {
        pwm_shooter = 0;
      }
      Serial.print("Shooter PWM");
      Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    if (PS4.getButtonPress(L3))
    {
      //      flag = 1;
      Serial.println("Start Flywheel");
      // target = 45;
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    if (PS4.getButtonClick(R3)) // shoot the ring using piston
    {
      Serial.print("Shoot Ring");
      digitalWrite(relay_shooter, HIGH);
      digitalWrite(relay_shooter2, HIGH);
      Serial.println("SHOT!!");
      delay(100);
    }
    else // retract piston
    {
      digitalWrite(relay_shooter, LOW);
      digitalWrite(relay_shooter, LOW);
      Serial.println("Retract Shooter Piston");
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

    if (PS4.getButtonPress(CROSS)) // stop the flywheel
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
    //   digitalWrite(relay_claw, LOW);
    // } else  //close claw
    // {
    //   Serial.println("Close Claw");
    //   digitalWrite(relay_claw, HIGH);
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

    if (PS4.getButtonPress(R2) && PS4.getButtonPress(UP))
    {
      Serial.println("Fast forward: PWM 100");
      mega_data = '9';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(R2) && PS4.getButtonPress(DOWN))
    {
      Serial.println("Fast backward: PWM 100");
      mega_data = '10';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(R2) && PS4.getButtonPress(LEFT))
    {
      Serial.println("Fast Left: PWM 100");
      mega_data = '11';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(R2) && PS4.getButtonPress(RIGHT))
    {
      Serial.println("Fast right: PWM 100");
      mega_data = '12';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L1) && PS4.getButtonPress(TRIANGLE))
    {
      Serial.println("Forward wheels: PWM 100");
      mega_data = '17';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L1) && PS4.getButtonPress(CROSS))
    {
      Serial.println("Backward wheels: PWM 100");
      mega_data = '18';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(LEFT) && PS4.getButtonPress(UP))
    {
      Serial.println("Diagonal FL & RR: PWM 100");
      mega_data = '19';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(RIGHT) && PS4.getButtonPress(DOWN))
    {
      Serial.println("Diagonal FR & RL: PWM 100");
      mega_data = '20';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(LEFT))
    {
      Serial.println("Slow left: PWM 45");
      mega_data = '3';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(RIGHT))
    {
      Serial.println("Slow right: PWM 45");
      mega_data = '4';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(UP))
    {
      Serial.println("Slow forward: PWM 45");
      mega_data = '1';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(DOWN))
    {
      Serial.println("Slow backward: PWM 45");
      mega_data = '2';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(R1))
    {
      Serial.println("Slow clock: PWM 20");
      mega_data = '13';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(L1))
    {
      Serial.println("Slow anti-clock: PWM 20");
      mega_data = '14';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(UP))
    {
      Serial.println("Forward: PWM 85");
      mega_data = '5';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(DOWN))
    {
      Serial.println("Backward: PWM 85");
      mega_data = '6';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(LEFT))
    {
      Serial.println("LEFT: PWM 85");
      mega_data = '7';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(RIGHT))
    {
      Serial.println("RIGHT: PWM 85");
      mega_data = '8';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(R1))
    {
      Serial.println("Clock: PWM 35");
      mega_data = '15';
      I2C_func(mega_data);
    }

    else if (PS4.getButtonPress(L1))
    {
      Serial.println("Anti-Clock: PWM 35");
      mega_data = '16';
      I2C_func(mega_data);
    }
    else
    {
      Serial.println("Stop");
      mega_data = '0';
      I2C_func(mega_data);
    }
    /*********** Drive ************/

    /*********** Automation ************/
    // if (PS4.getButtonPress(L2) && PS4.getButtonPress(TRIANGLE)) {
    //   if (digitalRead(auto_pin1) == HIGH && digitalRead(auto_pin2) == LOW && digitalRead(auto_pin3) == LOW) {
    //     Serial.println("Go Back");
    //     mega_data = '2';
    // I2C_func(mega_data);

    //   } else if (digitalRead(auto_pin1) == HIGH && digitalRead(auto_pin2) == HIGH && digitalRead(auto_pin3) == HIGH) {
    //     Serial.println("Go Forward");
    //     mega_data = '1';

    // I2C_func(mega_data);

    //   } else if (digitalRead(auto_pin1) == HIGH && digitalRead(auto_pin2) == HIGH && digitalRead(auto_pin3) == LOW) {
    //     Serial.println("Go Left");
    //     mega_data = '3';

    // I2C_func(mega_data);

    //   } else if (digitalRead(auto_pin1) == HIGH && digitalRead(auto_pin2) == LOW && digitalRead(auto_pin3) == HIGH) {
    //     Serial.println("Go Right");
    //     mega_data = '4';

    // I2C_func(mega_data);

    //   } else if (digitalRead(auto_pin1) == LOW && digitalRead(auto_pin2) == HIGH && digitalRead(auto_pin3) == HIGH) {
    //     Serial.println("Stop");
    //     mega_data = '0';

    // I2C_func(mega_data);

    //   } else if (digitalRead(auto_pin1) == LOW && digitalRead(auto_pin2) == LOW && digitalRead(auto_pin3) == LOW) {
    //     Serial.println("Do nothing");
    //     mega_data = '0';

    // I2C_func(mega_data);

    //   }
    // }
    /*********** Automation ************/
  }
}