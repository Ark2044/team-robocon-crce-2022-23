#include <PS4USB.h>

#define m1_dir 22
#define m2_dir 24
#define m1_pwm 6
#define m2_pwm 5

int M1_PWM = 150;
int M2_PWM = 150;

#define ir_1 2
#define ir_2 3

int led_ctr_1 = 0;
int led_ctr_2 = 0;
int irStatus_1 = 0;
int irStatus_2 = 0;
long long current_millis = 0;

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS4USB PS4(&Usb);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
#if !defined(_MIPSEL_)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  pinMode(ir_1,INPUT);

  pinMode(ir_2,INPUT);
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
  digitalWrite(m1_dir, HIGH);
  digitalWrite(m2_dir, LOW);
  Serial.print(F("\r\nPS4 USB Library Started"));
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    int interval = 500;
    
    if (millis() - current_millis > interval){
        Serial.print("IR1 ");

        Serial.println(led_ctr_1);
        Serial.print("IR2 ");

        Serial.println(led_ctr_2);
        led_ctr_1 = 0;
        led_ctr_2 = 0;
        current_millis = millis();
    }
    
    if (PS4.getButtonClick(CIRCLE)) {
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      Serial.println("Start");
    }
    
    if (PS4.getButtonClick(CROSS)) {
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 0);
      Serial.println("Stop");
    }

    if (PS4.getButtonClick(UP)) {
      M1_PWM+=20;
      M2_PWM+=20;
      Serial.print("PWM - ");
      Serial.println(M1_PWM);
    }
    
    if (PS4.getButtonClick(DOWN)) {
      M1_PWM-=20;
      M2_PWM-=20;
      Serial.print("PWM - ");
      Serial.println(M1_PWM);
    }
    
    if (digitalRead(ir_1) == 1 && irStatus_1 == 0){
      led_ctr_1++;
//      Serial.println(led_ctr_1);
    }

    if (digitalRead(ir_2) == 1 && irStatus_2 == 0){
      led_ctr_2++;
//      Serial.println(led_ctr_2);
    }

//    if (PS4.getButtonClick(TRIANGLE)) {
//      led_ctr=0;
//      Serial.println(led_ctr);
//    }
    
    irStatus_1 = digitalRead(ir_1);
    irStatus_2 = digitalRead(ir_2);
  }
}