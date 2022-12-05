#include<TimerOne.h>
#define e1PinA  2

#define e2PinA  3

#define e3PinA  18

#define e4PinA  20

volatile long e1Pos=0,e2Pos=0,e3Pos=0,e4Pos=0;
long newposition1,newposition2,newposition3,newposition4;
long oldposition1= 0,oldposition2 = 0,oldposition3 = 0,oldposition4 = 0;

long s1,s2,s3,s4;

void setup()
{
  //Motor 1
  pinMode(e1PinA, INPUT);
  digitalWrite(e1PinA, HIGH);       // turn on pullup resistor

  //Motor 2
  pinMode(e2PinA, INPUT);
  digitalWrite(e2PinA, HIGH);       // turn on pullup resistor

  //Motor 3
  pinMode(e3PinA, INPUT);
  digitalWrite(e3PinA, HIGH);       // turn on pullup resistor

  //Motor 4
  pinMode(e4PinA, INPUT);
  digitalWrite(e4PinA, HIGH);       // turn on pullup resistor


  Timer1.initialize(1000000/2);
  Timer1.attachInterrupt(readspeed);

  attachInterrupt(digitalPinToInterrupt(2), doe1, RISING);  // encoder ON PIN 2
  attachInterrupt(digitalPinToInterrupt(3), doe2, RISING);
  attachInterrupt(digitalPinToInterrupt(18), doe3, RISING);
  attachInterrupt(digitalPinToInterrupt(20), doe4, RISING);
  Serial.begin (9600);
}

void loop()
{
  newposition1 = e1Pos;

  newposition2 = e2Pos;

  newposition3 = e3Pos;

  newposition4 = e4Pos;
  
  // //Serial.println("hello");
  // readspeed();
  // delay(10);
  }

void doe1()
{
  e1Pos++;
}

void doe2()
{
  e2Pos++;
}

void doe3()
{
  e3Pos++;
}

void doe4()
{
  e4Pos++;
} 


void readspeed()
{
  s1=(newposition1-oldposition1);
  Serial.print("Speed1 = ");
  Serial.println(s1);
  oldposition1=newposition1;
  
  s2=(newposition2-oldposition2);
  Serial.print("Speed2 = ");
  Serial.println(s2);
  oldposition2=newposition2;

  s3=(newposition3-oldposition3);
  Serial.print("speed3 = ");
  Serial.println(s3);
  oldposition3=newposition3;


  s4=(newposition4-oldposition4);
  Serial.print("speed4 = ");
  Serial.println(s4);
  oldposition4=newposition4;
}
