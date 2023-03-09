#include <PS3USB.h>

#define m1_dir 26
#define m2_dir 24
#define m1_pwm 12
#define m2_pwm 13

int M1_PWM = 200;
int M2_PWM = 200;

#define ir_1 18
#define ir_2 19

int led_ctr_1 = 0;
int led_ctr_2 = 0;
int irStatus_1 = 0;
int irStatus_2 = 0;
long long current_millis = 0;

int speed_1,speed_2;

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS3USB PS3(&Usb);


// bool printAngle, printTouch;
// uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
  pinMode(ir_1, INPUT);
  pinMode(ir_2, INPUT);
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
  digitalWrite(m1_dir, LOW);
  digitalWrite(m2_dir, LOW);
  #if !defined(_MIPSEL_)
  while (!Serial);  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);  // Halt
  }
  Serial.print(F("\r\nPS4 USB Library Started"));
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected) {
    int interval = 1000;

    if (millis() - current_millis > interval) {
      Serial.print("IR1 ");
      Serial.println(led_ctr_1);

      Serial.print("IR2 ");
      Serial.println(led_ctr_2);

      speed_1=led_ctr1;
      speed_2=led_ctr2;

      led_ctr_1 = 0;
      led_ctr_2 = 0;
      current_millis = millis();
    }

    if (digitalRead(ir_1) == 1 && irStatus_1 == 0) {
      led_ctr_1++;
      //Serial.println(led_ctr_1);
    }

    if (digitalRead(ir_2) == 1 && irStatus_2 == 0) {
      led_ctr_2++;
      //Serial.println(led_ctr_2);
    }
    irStatus_1 = digitalRead(ir_1);
    irStatus_2 = digitalRead(ir_2);    

    if (PS3.getButtonClick(CIRCLE)) {
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      speedcontrol();
      //Serial.println("Start");

      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(CROSS)) {
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 0);
      //Serial.println("Stop");

      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(UP)) {
      if(M1_PWM>=255 || M2_PWM>=255)
      {
        M1_PWM=255;
        M2_PWM=255;
      }
      else
      {
        M1_PWM += 1;
        M2_PWM += 1;
      }
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      speedcontrol();
      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(DOWN)) {
      if(M1_PWM<=0 || M2_PWM<0)
      {
        M1_PWM=0;
        M2_PWM=0;
      }
      else
      {
        M1_PWM -= 1;
        M2_PWM -= 1;
      }
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      speedcontrol();
      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(L1)) {
      if(M1_PWM>=255)
      {
        M1_PWM=255;
      }
      else
      {
        M1_PWM += 20;
      }
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      speedcontrol();
      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(L2)) {
      if(M1_PWM<=0)
      {
        M1_PWM=0;
      }
      else
      {
        M1_PWM -= 20;
      }
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      speedcontrol();
      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(R1)) {
      if(M2_PWM>=255)
      {
        M2_PWM=255;
      }
      else
      {
        M2_PWM += 20;
      }
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);   
      speedcontrol();   
      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }

    if (PS3.getButtonClick(R2)) {
      if(M2_PWM<=0)
      {
        M2_PWM=0;
      }
      else
      {
        M2_PWM -= 20;
      }
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      speedcontrol();
      Serial.print("PWM1: ");
      Serial.print(M1_PWM);
      Serial.print(" PWM2: ");
      Serial.println(M2_PWM);
    }
  }
}

void speedcontrol() {
  if (speed_1 > speed_2 && M1_PWM > 200)
    M1_PWM -= 10;
  else if (speed_1 > speed_2)
    M2_PWM += 10;
  if (speed_1 < speed_2 && M2_PWM > 200)
    M2_PWM -= 10;
  else if (speed_1 < speed_2)
    M1_PWM += 10;
}