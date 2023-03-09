/*
  I2C Pinouts

  SDA -> A4
  SCL -> A5
*/
//Import the library required
#include <Wire.h>
#include <TimerOne.h>

#define e1PinA 3

#define e2PinA 2

//Slave Address for the Communication
#define SLAVE_ADDRESS 0x04

volatile long long int e1Pos = 0, e2Pos = 0;
long newposition1, newposition2;
long oldposition1 = 0, oldposition2 = 0;

long s1, s2;

char number[50];
int state = 0;

//Code Initialization
void setup() {
  //Encoder
  //Motor 1
  pinMode(e1PinA, INPUT);
  digitalWrite(e1PinA, HIGH);  // turn on pullup resistor

  //Motor 2
  pinMode(e2PinA, INPUT);
  digitalWrite(e2PinA, HIGH);  // turn on pullup resistor

  Timer1.initialize(1000000 / 2);
  Timer1.attachInterrupt(readspeed);

  attachInterrupt(digitalPinToInterrupt(3), doe1, RISING);
  attachInterrupt(digitalPinToInterrupt(2), doe2, RISING);

  // initialize i2c as slave
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  // Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
  newposition1 = e1Pos;

  newposition2 = e2Pos;
} // end loop

// callback for received data
// void receiveData(int byteCount) {
//   int i = 0;
//   while (Wire.available()) {
//     number[i] = Wire.read();
//     i++;
//   }
//   number[i] = '\0';
//   Serial.print(number);
// }  // end while

// callback for sending data
void sendData() {
  Wire.write(char(s1));
  Wire.write(char(s2));
}

void doe1() {
  e1Pos++;
}

void doe2() {
  e2Pos++;
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
}

//End of the program