/*
I2C Pinouts

SDA -> A4
SCL -> A5
*/

//Import the library required 
#include <Wire.h>
#include <TimerOne.h>

#define e3PinA 3

#define e4PinA 2

//Slave Address for the Communication
#define SLAVE_ADDRESS 0x05

volatile long long int e3Pos = 0, e4Pos = 0;
long newposition3, newposition4;
long oldposition3 = 0, oldposition4 = 0;

long s3, s4;

char number[50];
int state = 0;

//Code Initialization
void setup() {
  //Motor 3
  pinMode(e3PinA, INPUT);
  digitalWrite(e3PinA, HIGH);  // turn on pullup resistor

  //Motor 4
  pinMode(e4PinA, INPUT);
  digitalWrite(e4PinA, HIGH);  // turn on pullup resistor

  Timer1.initialize(1000000 / 2);
  Timer1.attachInterrupt(readspeed);

  attachInterrupt(digitalPinToInterrupt(3), doe3, RISING);
  attachInterrupt(digitalPinToInterrupt(2), doe4, RISING);

  // initialize i2c as slave
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
 // define callbacks for i2c communication
// Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
  newposition3 = e3Pos;

  newposition4 = e4Pos;
} // end loop

// callback for received data
// void receiveData(int byteCount){
//   int i = 0;
//   while(Wire.available()) { 
//     number[i] = Wire.read();
//     i++;
//   }
//   number[i] = '\0';
//   Serial.print(number);
// }  // end while

// callback for sending data
void sendData(){
  Wire.write(s3);
  Write.write(s4);
}

void doe3() {
  e3Pos++;
}

void doe4() {
  e4Pos++;
}

void readspeed() {
  s3 = (newposition3 - oldposition3);
  Serial.print("speed3 = ");
  Serial.println(s3);
  oldposition3 = newposition3;

  s4 = (newposition4 - oldposition4);
  Serial.print("speed4 = ");
  Serial.println(s4);
  oldposition4 = newposition4;
}

//End of the program 