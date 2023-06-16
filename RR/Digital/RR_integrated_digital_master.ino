//RR Integrated digital communication master

#include <PS4BT.h>   //PS4 Bluetooth
#include <usbhub.h>  //Comment this for PS4 USB

//#include <PS4USB.h>   //PS4 Usb

#include <Servo.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//laser
int laser_diode = 25;

//cytron - shooter
const int m1_dir = 29;  //left motor with behind the shooter - pov
const int m1_pwm = 4;
const int m2_dir = 27;  //right motor with behind the shooter - pov
const int m2_pwm = 3;

long long currT = 0;

//shooter - motor's pwm
int pwm_shooter = 85;  //near type 2 pole

//piston - shooter
const int relay_shooter = 28;

// servos for the claw
Servo servo_right;
Servo servo_left;
const int s1 = 11;
const int s2 = 12;

float servo_right_var = 67.20;
float servo_left_var = 173.90;

float hold_servo_right = 67.20;
float hold_servo_left = 173.90;

float final_right_servo = 146.10;
float final_left_servo = 70;

float release_servo_right = 146.10;
float release_servo_left = 105.90;

// flipping motor using l298n
const int inf1 = 47;
const int inf2 = 31;
const int enaf = 7;

const int inf3 = 49;
const int inf4 = 33;
const int enbf = 8;

bool init_flip = 1;

// limit switch for flipping
const int limit_switch_up = 30;
// const int limit_switch_down = 1000;

// relay claw
const int relay_claw = 10;

//Automation pins
// const int auto_pin1 = 41;
// const int auto_pin2 = 42;
// const int auto_pin3 = 43;

//communication pins
//RR
const int com_pin4 = 6;
const int com_pin3 = 41;
const int com_pin2 = 39;
const int com_pin1 = 37;
const int com_pin0 = 35;

//ER
// const int com_pin4=6;
// const int com_pin3=43;
// const int com_pin2=41;
// const int com_pin1=39;
// const int com_pin0=37;

/** PS4 USB **/
//Use this for PS4 USB and comment PS4 BT
//USB Usb;
//PS4USB PS4(&Usb);
/** PS4 USB **/

/** PS4 BT **/
// Use this for PS4 BT and comment PS4 USB
USB Usb;
BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so
PS4BT PS4(&Btd, PAIR);
/** PS4 BT **/

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

/** claw ***/
void flip_up() {
  if (digitalRead(limit_switch_up) == 1 && init_flip == 1)  // run the following when claw completes 180 degrees
  {
    Serial.println("stopping process");
    analogWrite(enaf, 0);
    analogWrite(enbf, 0);

    digitalWrite(inf1, LOW);  // shut the flipping motor
    digitalWrite(inf2, LOW);
    digitalWrite(inf3, LOW);  // shut the flipping motor
    digitalWrite(inf4, LOW);
    // delay(500);
    // digitalWrite(relay_claw, HIGH);
    // servo_right.write(120);
    // servo_left.write(0);
    // delay(1000);
    // servo_right.write(35);
    // servo_left.write(70);
    // delay(500);
    // digitalWrite(relay_claw, LOW);
    init_flip = 0;
  }

  else if (digitalRead(limit_switch_up) == 0 && init_flip == 0) {
    Serial.println("limit switch up low");
    init_flip = 1;
  }

  else if (init_flip == 0) {
    analogWrite(enaf, 0);
    analogWrite(enbf, 0);
    digitalWrite(inf1, LOW);  // shut the flipping motor
    digitalWrite(inf2, LOW);
    digitalWrite(inf3, LOW);  // shut the flipping motor
    digitalWrite(inf4, LOW);
  }

  else {
    analogWrite(enaf, 115);
    analogWrite(enbf, 115);
    digitalWrite(inf1, HIGH);
    digitalWrite(inf2, LOW);
    digitalWrite(inf3, HIGH);
    digitalWrite(inf4, LOW);
  }
}

void flip_down() {
  //  if (digitalRead(limit_switch_down) == 1) {
  //    analogWrite(enbf, 0);
  //    digitalWrite(inf1, LOW);
  //    digitalWrite(inf2, LOW);
  //  }
  //
  //  else {
  analogWrite(enaf, 115);
  digitalWrite(inf1, LOW);
  digitalWrite(inf2, HIGH);
  analogWrite(enbf, 115);
  digitalWrite(inf3, HIGH);
  digitalWrite(inf4, LOW);
  //  }
}
/** claw ***/

void setup() {
  servo_right.attach(s1);
  servo_left.attach(s2);
  // servo_left.write(servo_left_var);
  // servo_right.write(servo_right_var);
  servo_left.write(hold_servo_left);
  servo_right.write(hold_servo_right);

  //laser
  pinMode(laser_diode, OUTPUT);
  digitalWrite(laser_diode, HIGH);

  //cytron-shooter
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  digitalWrite(m1_dir, LOW);   // left
  digitalWrite(m2_dir, HIGH);  // right

  analogWrite(m1_pwm, 0);
  analogWrite(m2_pwm, 0);

  //piston shooter
  pinMode(relay_shooter, OUTPUT);


  //l298n - flipping
  pinMode(enaf, OUTPUT);
  pinMode(inf1, OUTPUT);
  pinMode(inf2, OUTPUT);
  pinMode(inf3, OUTPUT);
  pinMode(inf4, OUTPUT);
  pinMode(enbf, OUTPUT);

  digitalWrite(inf1, LOW);
  digitalWrite(inf2, LOW);
  digitalWrite(inf3, LOW);
  digitalWrite(inf4, LOW);

  analogWrite(enaf, 0);
  analogWrite(enbf, 0);

  // limit switches for flipping
  pinMode(limit_switch_up, INPUT);
  // pinMode(limit_switch_down, INPUT);

  //relay for claw
  pinMode(relay_claw, OUTPUT);

  //Automation
  // pinMode(auto_pin1, INPUT);
  // pinMode(auto_pin2, INPUT);
  // pinMode(auto_pin3, INPUT);

  //Communication pins
  pinMode(com_pin4, OUTPUT);
  pinMode(com_pin3, OUTPUT);
  pinMode(com_pin2, OUTPUT);
  pinMode(com_pin1, OUTPUT);
  pinMode(com_pin0, OUTPUT);

  digitalWrite(com_pin4, HIGH);
  digitalWrite(com_pin3, HIGH);
  digitalWrite(com_pin2, HIGH);
  digitalWrite(com_pin1, HIGH);
  digitalWrite(com_pin0, HIGH);

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
  Serial.println(F("\r\nPS4 Bluetooth Library Started"));
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    Serial.print("Limit Switch: ");
    Serial.println(digitalRead(limit_switch_up));
    int right_x = map(PS4.getAnalogHat(RightHatX), 0, 255, -255, 255);
    int right_y = map(PS4.getAnalogHat(RightHatY), 0, 255, 255, -255);

    /** flywheel ***/
    if (PS4.getButtonPress(SHARE) && PS4.getButtonPress(L3) && !(servo_left_var > 179) && !(servo_left_var < 0)) {
      servo_left_var += 0.1;
      if (servo_left_var >= 180) {
        servo_left_var = 180;
      }
      Serial.print("Servo left Var: ");
      Serial.println(servo_left_var);
      servo_left.write(servo_left_var);
    }

    else if (PS4.getButtonPress(OPTIONS) && PS4.getButtonPress(L3) && !(servo_left_var > 179) && !(servo_left_var < 0)) {
      servo_left_var -= 0.1;
      if (servo_left_var <= 0) {
        servo_left_var = 0;
      }
      Serial.print("Servo left Var: ");
      Serial.println(servo_left_var);
      servo_left.write(servo_left_var);
    }

    if (PS4.getButtonPress(L3) && PS4.getButtonClick(R2)) {
      if (pwm_shooter >= 255) {
        pwm_shooter = 255;
      } else {
        pwm_shooter += 5;
      }
      // Serial.print("Shooter PWM");
      // Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    else if (PS4.getButtonPress(L3) && PS4.getButtonClick(L2)) {
      if (pwm_shooter <= 0) {
        pwm_shooter = 0;
      } else {
        pwm_shooter -= 5;
      }
      // Serial.print("Shooter PWM");
      // Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    else if (PS4.getButtonPress(L3) && PS4.getButtonClick(CROSS))  //stop the flywheel
    {
      // Serial.println("Shooter stop");
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 0);
    }

    else if (PS4.getButtonPress(L3) && PS4.getButtonClick(SQUARE)) {
      pwm_shooter = 85;  // Near type 2 pole
      // Serial.print("Shooter PWM");
      // Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    else if (PS4.getButtonPress(L3) && PS4.getButtonClick(CIRCLE)) {
      pwm_shooter = 141;  // Far type 2 pole
      // Serial.print("Shooter PWM");
      // Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    else if (PS4.getButtonPress(L3) && PS4.getButtonClick(TRIANGLE)) {
      pwm_shooter = 225;  // Center pole
      // Serial.print("Shooter PWM");
      // Serial.println(pwm_shooter);
      analogWrite(m1_pwm, pwm_shooter);
      analogWrite(m2_pwm, pwm_shooter);
    }

    // //flip the claw
    if (PS4.getButtonPress(L2) && PS4.getButtonPress(SQUARE) && !(PS4.getButtonPress(L3))) {
      Serial.println("Flip Claw Up");
      flip_up();
    }

    else if (PS4.getButtonPress(SQUARE) && !(PS4.getButtonPress(L3))) {
      Serial.println("flip claw down");
      flip_down();
    }

    else {
      // Serial.println("Stop Claw");
      analogWrite(enbf, 0);
      digitalWrite(inf1, LOW);
      digitalWrite(inf2, LOW);
    }
    /** claw ***/


    if (PS4.getButtonPress(SHARE) && PS4.getButtonPress(R3) && !(servo_right_var > 179) && !(servo_right_var < 0)) {
      servo_right_var += 0.1;
      if (servo_right_var >= 180) {
        servo_right_var = 0;
      }
      Serial.print("Servo right Var: ");
      Serial.println(servo_right_var);
      servo_right.write(servo_right_var);

    }

    else if (PS4.getButtonPress(OPTIONS) && PS4.getButtonPress(R3) && !(servo_right_var > 179) && !(servo_right_var < 0)) {
      servo_right_var -= 0.1;
      if (servo_right_var <= 0) {
        servo_right_var = 0;
      }
      Serial.print("Servo right Var: ");
      Serial.println(servo_right_var);
      servo_right.write(servo_right_var);
    }

    else if (PS4.getButtonClick(R3))  //shoot the ring using piston
    {
      // Serial.print("Shoot Ring");
      digitalWrite(relay_shooter, HIGH);
      delay(500);
      // Serial.println("SHOT!!");
      digitalWrite(relay_shooter, LOW);

      // digitalWrite(relay_claw, HIGH);
      digitalWrite(relay_claw, LOW);

      delay(200);
      // final_right_servo = servo_right_var - 90;
      // final_left_servo = servo_left_var + 90;
      // servo_right.write(final_right_servo);
      // servo_left.write(final_left_servo);
      servo_right.write(release_servo_right);
      servo_left.write(release_servo_left);
      delay(1000);

      // final_right_servo = servo_right_var;
      // final_left_servo = servo_left_var;
      // servo_right.write(final_right_servo);
      // servo_left.write(final_left_servo);
      servo_right.write(hold_servo_right);
      servo_left.write(hold_servo_left);
      delay(500);

      digitalWrite(relay_claw, HIGH);
      // digitalWrite(relay_claw, LOW);

      //servoright.write(0);
      //servoleft.write(100);
    }

    else  //retract piston
    {
      // Serial.println("Retract Shooter Piston");
      // digitalWrite(relay_claw, LOW);
      digitalWrite(relay_claw, HIGH);

      servo_left.write(servo_left_var);
      servo_right.write(servo_right_var);
    }


    /** flywheel ***/

    if (PS4.getButtonPress(TOUCHPAD) && PS4.getButtonPress(SHARE)) {
      digitalWrite(relay_claw, HIGH);
      // delay(100);
    }

    if (PS4.getButtonPress(TOUCHPAD) && PS4.getButtonPress(OPTIONS)) {
      digitalWrite(relay_claw, LOW);
      // delay(100);
    }
    /** claw ***/


    /** Drive ***/

    if (PS4.getButtonPress(R2) && PS4.getButtonPress(UP)) {
      // Serial.println("Fast forward (9): PWM 130");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(R2) && PS4.getButtonPress(DOWN)) {
      // Serial.println("Fast backward (10): PWM 130");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(R2) && PS4.getButtonPress(LEFT)) {
      // Serial.println("Fast Left (11): PWM 130");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(R2) && PS4.getButtonPress(RIGHT)) {
      // Serial.println("Fast right (12): PWM 130");
      digitalWrite(com_pin0, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin4, LOW);
    }

    else if (PS4.getButtonPress(L1) && PS4.getButtonPress(TRIANGLE)) {
      // Serial.println("Forward wheels (17): PWM 130");
      digitalWrite(com_pin4, HIGH);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(L1) && PS4.getButtonPress(CROSS)) {
      // Serial.println("Backward wheels (18): PWM 130");
      digitalWrite(com_pin4, HIGH);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(LEFT) && PS4.getButtonPress(UP)) {
      // Serial.println("Diagonal FR & RL (20): PWM 130");
      digitalWrite(com_pin4, HIGH);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(RIGHT) && PS4.getButtonPress(UP)) {
      // Serial.println("Diagonal FL & RR (19): PWM 130");
      digitalWrite(com_pin4, HIGH);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(UP)) {
      // Serial.println("Slow forward (1): PWM 45");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(DOWN)) {
      // Serial.println("Slow backward (2): PWM 45");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(LEFT)) {
      // Serial.println("Slow left (3): PWM 45");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(RIGHT)) {
      // Serial.println("Slow right (4): PWM 45");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(L1)) {
      // Serial.println("Slow anti-clock (14): PWM 18");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(L2) && PS4.getButtonPress(R1)) {
      // Serial.println("Slow clock (13): PWM 18");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, HIGH);
    }

    /** Picking:Reverse Directions ***/
    else if (PS4.getButtonPress(TOUCHPAD) && PS4.getButtonPress(UP)) {
      // Serial.println("Backward (6): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(TOUCHPAD) && PS4.getButtonPress(DOWN)) {
      // Serial.println("Forward (5): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(TOUCHPAD) && PS4.getButtonPress(LEFT)) {
      // Serial.println("RIGHT (8): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(TOUCHPAD) && PS4.getButtonPress(RIGHT)) {
      // Serial.println("LEFT (7): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }
    /** Picking:Reverse Directions ***/

    else if (PS4.getButtonPress(UP)) {
      // Serial.println("Forward (5): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(DOWN)) {
      // Serial.println("Backward (6): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(LEFT)) {
      // Serial.println("LEFT (7): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(RIGHT)) {
      // Serial.println("RIGHT (8): PWM 85");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, LOW);
    }

    else if (PS4.getButtonPress(R1)) {
      // Serial.println("Clock (15): PWM 35");
      digitalWrite(com_pin4, LOW);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }

    else if (PS4.getButtonPress(L1)) {
      // Serial.println("Anti-Clock (16): PWM 35");
      digitalWrite(com_pin4, HIGH);
      digitalWrite(com_pin3, LOW);
      digitalWrite(com_pin2, LOW);
      digitalWrite(com_pin1, LOW);
      digitalWrite(com_pin0, LOW);
    } else {
      // Serial.println("Stop (31)");
      digitalWrite(com_pin4, HIGH);
      digitalWrite(com_pin3, HIGH);
      digitalWrite(com_pin2, HIGH);
      digitalWrite(com_pin1, HIGH);
      digitalWrite(com_pin0, HIGH);
    }
    /** Drive ***/

    /** Automation ***/

    /** Automation ***/
  }
}
