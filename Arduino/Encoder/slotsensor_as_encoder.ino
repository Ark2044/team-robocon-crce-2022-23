#include <TimerOne.h>
#define ir_1 50
int led_ctr_1 = 0;
int irStatus_1 = 0;
//volatile long long int e1Pos = 0;
//long newposition1;
long long current_millis = 0;
//long oldposition1 = 0;
//long s1;
void setup() {
  pinMode(ir_1, INPUT);

  //Timer1.initialize(1000000 / 2);
  //Timer1.attachInterrupt(readspeed);
  //attachInterrupt(digitalPinToInterrupt(3), doe1, RISING);
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  //newposition1 = e1Pos;
  int interval = 500;

  if (millis() - current_millis > interval) {
    Serial.print("IR1 ");
    Serial.println(led_ctr_1);
    led_ctr_1 = 0;
    current_millis = millis();
  }

  if (digitalRead(ir_1) == 1 && irStatus_1 == 0) {
    led_ctr_1++;
    //Serial.println(led_ctr_1);
  }
  irStatus_1 = digitalRead(ir_1);
  // put your main code here, to run repeatedly:
}

// void doe1() {
//   e1Pos++;
// }

// void readspeed() {
//   s1 = (newposition1 - oldposition1);
//   Serial.print("Speed1 = ");
//   Serial.println(s1);
//   oldposition1 = newposition1;
// }

