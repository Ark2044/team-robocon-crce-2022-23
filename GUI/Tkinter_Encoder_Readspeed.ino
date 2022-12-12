#include <TimerOne.h>
//Encoder
#define e1PinA 3

#define e2PinA 2

#define e3PinA 18

#define e4PinA 20

volatile long e1Pos = 0, e2Pos = 0, e3Pos = 0, e4Pos = 0;
long newposition1, newposition2, newposition3, newposition4;
long oldposition1 = 0, oldposition2 = 0, oldposition3 = 0, oldposition4 = 0;

long s1, s2, s3, s4;

//Cytron
#define m4_dir 53
#define m1_dir 51
#define m4_pwm 4
#define m1_pwm 5

#define m2_dir 47
#define m3_dir 49
#define m2_pwm 7
#define m3_pwm 6

int M1_PWM = 20;
int M2_PWM = 20;
int M3_PWM = 20;
int M4_PWM = 20;

char userInput;
// int cwSpeed = 150;
// int ccwSpeed = -150;

void setup() {
  //Encoder
  //Motor 1
  pinMode(e1PinA, INPUT);
  digitalWrite(e1PinA, HIGH);  // turn on pullup resistor

  //Motor 2
  pinMode(e2PinA, INPUT);
  digitalWrite(e2PinA, HIGH);  // turn on pullup resistor

  //Motor 3
  pinMode(e3PinA, INPUT);
  digitalWrite(e3PinA, HIGH);  // turn on pullup resistor

  //Motor 4
  pinMode(e4PinA, INPUT);
  digitalWrite(e4PinA, HIGH);  // turn on pullup resistor


  Timer1.initialize(1000000 / 2);
  Timer1.attachInterrupt(readspeed);

  attachInterrupt(digitalPinToInterrupt(2), doe1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), doe2, RISING);
  attachInterrupt(digitalPinToInterrupt(18), doe3, RISING);
  attachInterrupt(digitalPinToInterrupt(20), doe4, RISING);

  //Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
  pinMode(m3_dir, OUTPUT);
  pinMode(m3_pwm, OUTPUT);
  pinMode(m4_dir, OUTPUT);
  pinMode(m4_pwm, OUTPUT);
  digitalWrite(m1_dir, HIGH);
  digitalWrite(m2_dir, LOW);
  digitalWrite(m3_dir, HIGH);
  digitalWrite(m4_dir, LOW);

  Serial.begin(9600);
  Serial.println("Omni-Bot Control Begin");
  Serial.println("enter: f, b, r, l, c, a, s");  //FORWARD,BACKWARD,RIGHT,LEFT,CLOCK,ANTICLOCK,STOP
}

void loop() {
  userInput = Serial.read();  //read serial input
  // cwSpeed = speedIn;
  // ccwSpeed = -speedIn;

  if (userInput == 'f') fwd();
  else if (userInput == 'b') bkw();
  else if (userInput == 'r') rt();
  else if (userInput == 'l') lt();
  else if (userInput == 'c') cw();
  else if (userInput == 'a') ccw();
  else if (userInput == 's') stp();

  newposition1 = e1Pos;

  newposition2 = e2Pos;

  newposition3 = e3Pos;

  newposition4 = e4Pos;
}

void doe1() {
  e1Pos++;
}

void doe2() {
  e2Pos++;
}

void doe3() {
  e3Pos++;
}

void doe4() {
  e4Pos++;
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

  s3 = (newposition3 - oldposition3);
  Serial.print("speed3 = ");
  Serial.println(s3);
  oldposition3 = newposition3;


  s4 = (newposition4 - oldposition4);
  Serial.print("speed4 = ");
  Serial.println(s4);
  oldposition4 = newposition4;
}

//HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)

void fwd() {
  Serial.println("Bot moving forward");
  accwMotor();
  bccwMotor();
  ccwMotor();
  dcwMotor();
}
void bkw() {
  Serial.println("Bot moving backwards");
  acwMotor();
  bcwMotor();
  cccwMotor();
  dccwMotor();
}
void rt() {
  Serial.println("Bot moving right");
  accwMotor();
  bcwMotor();
  ccwMotor();
  dccwMotor();
}
void lt() {
  Serial.println("Bot moving left");
  acwMotor();
  bccwMotor();
  cccwMotor();
  dcwMotor();
}
void cw() {
  Serial.println("Bot moving clockwise");
  accwMotor();
  bccwMotor();
  cccwMotor();
  dccwMotor();
}
void ccw() {
  Serial.println("Bot moving counter-clockwise");
  acwMotor();
  bcwMotor();
  ccwMotor();
  dcwMotor();
}
void stp() {
  Serial.println("Bot stop");
  astopMotor();
  bstopMotor();
  cstopMotor();
  dstopMotor();
}

//LOW LEVEL MOTOR FUNCTIONS

void acwMotor() {
  Serial.println("Motor A clockwise");
  digitalWrite(m1_dir,HIGH);
  analogWrite(m1_pwm, M1_PWM);
}
void accwMotor() {
  Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir,LOW);
  analogWrite(m1_pwm, M1_PWM);
}
void bcwMotor() {
  Serial.println("Motor B clockwise");
  digitalWrite(m2_dir,HIGH);
  analogWrite(m2_pwm, M2_PWM);
}
void bccwMotor() {
  Serial.println("Motor B counter-clockwise");
  digitalWrite(m2_dir,LOW);
  analogWrite(m2_pwm, M2_PWM);
}
void ccwMotor() {
  Serial.println("Motor C clockwise");
  digitalWrite(m3_dir,HIGH);
  analogWrite(m3_pwm, M3_PWM);
}
void cccwMotor() {
  Serial.println("Motor C counter-clockwise");
  digitalWrite(m3_dir,LOW);
  analogWrite(m3_pwm, M3_PWM);
}
void dcwMotor() {
  Serial.println("Motor D clockwise");
  digitalWrite(m4_dir,HIGH);
  analogWrite(m4_pwm, M4_PWM);
}
void dccwMotor() {
  Serial.println("Motor D counter-clockwise");
  digitalWrite(m4_dir,LOW);
  analogWrite(m4_pwm, M4_PWM);
}
void astopMotor() {
  Serial.println("Motor A stop");
  analogWrite(m1_pwm, 0);
}
void bstopMotor() {
  Serial.println("Motor B stop");
  analogWrite(m2_pwm, 0);
}
void cstopMotor() {
  Serial.println("Motor C stop");
  analogWrite(m3_pwm, 0);
}
void dstopMotor() {
  Serial.println("Motor D stop");
  analogWrite(m4_pwm, 0);
}
