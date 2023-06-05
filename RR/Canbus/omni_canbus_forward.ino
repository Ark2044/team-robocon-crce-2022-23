// Omni Canbus Forward reciever code
// FR->m2 FL->m1
#include <SPI.h>
#include <mcp2515.h>

// Define the CS pin for the CAN bus shield/module
#define CS_PIN 10

// Define the CAN message ID to read
struct can_frame canMsg;

// Create an MCP_CAN object for the CAN bus shield/module
MCP2515 mcp2515(CS_PIN);

// Pin definition *********** /
// List of the variables that will be recieved via I2C ****** / byte I2C_OnOff;  //defining the variable that will be sent
char argument;
int PIN_INPUT1 = 3;
int PIN_INPUT2 = 2;

// IR
volatile long int ctr1 = 0, ctr2 = 0;
long int newcount1 = 0, newcount2 = 0;
int s1 = 0, s2 = 0;

long currT;
float deltaT;

// CYTRON
const int m1_dir = 5;
const int m1_pwm = 6;

const int m2_dir = 7;
const int m2_pwm = 9;

int target1 = 0, target2 = 0;

long prevT = 0;

// motors
int pwr1, pwr2;

class SimplePID
{
private:
  float kp, kd, ki, umax; // Parameters
  float eprev, eintegral; // Storage
  float e, u;

public:
  // Constructor
  SimplePID()
      : kp(1), kd(0), ki(0), umax(255), eprev(0.0), eintegral(0.0) {}

  // A function to set the parameters
  void setParams(float kpIn, float kdIn, float kiIn, float umaxIn)
  {
    kp = kpIn;
    kd = kdIn;
    ki = kiIn;
    umax = umaxIn;
  }

  // A function to compute the control signal
  void evalu(int value, int target, float deltaT, int &pwr, int pwm_pin)
  {
    // error
    e = target - value;
    // Serial.print("value = ");
    // Serial.print(value);
    // Serial.print(" e = ");
    // Serial.print(e);

    // derivative
    float dedt = (e - eprev) / (deltaT);
    // Serial.println("e dt");
    // Serial.println(dedt);
    //  integral
    eintegral = eintegral + e * deltaT;
    // Serial.println("e int");
    // Serial.println(eintegral);
    // Serial.println("k1");
    // Serial.println(ki);
    //  control signal
    if (target == 0)
    {
      u = (0.1 * e + kd * dedt);
      e = 0;
      dedt = 0;
      eintegral = 0;
    }
    else
    {
      u = (kp * e + kd * dedt + ki * eintegral);
    }

    // Serial.print(" u = ");
    // Serial.print(u);

    // motor power
    pwr = (int)fabs(u);
    // pwr = abs(pwr);

    if (pwr > umax)
    {
      pwr = umax;
    }

    // Serial.print(" pwr = ");
    // Serial.println(pwr);
    analogWrite(pwm_pin, pwr);

    // store previous error
    eprev = e;
  }
};

SimplePID pid1, pid2;

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_INPUT1, INPUT);
  pinMode(PIN_INPUT2, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_INPUT1), interrupt_routine1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT2), interrupt_routine2, FALLING);

  // Cytron
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);

  // FR->m2  FL->m1

  pid1.setParams(1.7, 0, 0.0000005, 255); // Change kP,kD,kI,umax only here
  pid2.setParams(1.9, 0, 0.0000005, 255); // Change kP,kD,kI,umax only here
  // pid1.evalu(s1, target1, deltaT, pwr1, m1_pwm);
  // pid2.evalu(s2, target2, deltaT, pwr2, m2_pwm);

  // Initialize the CAN bus shield/module
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop()
{
  currT = micros();
  deltaT = ((float)(currT - prevT));
  if (currT - prevT > 300000)
  {
    noInterrupts();
    newcount1 = ctr1;
    newcount2 = ctr2;
    readspeed();
    prevT = currT;
    ctr1 = 0;
    ctr2 = 0;
    interrupts();
  }
  // analogWrite(m1_pwm, 50);
  // analogWrite(m2_pwm, 50);
  // pid1.evalu(s1, target1, deltaT, pwr1, m1_pwm);
  // pid2.evalu(s2, target2, deltaT, pwr2, m2_pwm);

  // Check for incoming messages
  // Check if the message ID matches the ID we want to read
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)
  {
    argument = canMsg.data[0];
    Serial.print("Argument: ");
    Serial.println(argument);

    if (argument == '1')
    { // Slow Forward
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 45);
      analogWrite(m2_pwm, 45);
    }
    else if (argument == '2')
    { // Slow Backward
      target1 = 20;
      target2 = 20;
      bkw();
      analogWrite(m1_pwm, 45);
      analogWrite(m2_pwm, 45);
    }
    else if (argument == '3')
    { // Slow Left
      target1 = 20;
      target2 = 20;
      lt();
      analogWrite(m1_pwm, 45);
      analogWrite(m2_pwm, 45);
    }
    else if (argument == '4')
    { // Slow Right
      target1 = 20;
      target2 = 20;
      rt();
      analogWrite(m1_pwm, 45);
      analogWrite(m2_pwm, 45);
    }
    else if (argument == '5')
    { // Forward
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 85);
      analogWrite(m2_pwm, 85);
    }
    else if (argument == '6')
    { // Backward
      target1 = 20;
      target2 = 20;
      bkw();
      analogWrite(m1_pwm, 85);
      analogWrite(m2_pwm, 85);
    }
    else if (argument == '7')
    { // Left
      target1 = 20;
      target2 = 20;
      lt();
      analogWrite(m1_pwm, 85);
      analogWrite(m2_pwm, 85);
    }
    else if (argument == '8')
    { // Right
      target1 = 20;
      target2 = 20;
      rt();
      analogWrite(m1_pwm, 85);
      analogWrite(m2_pwm, 85);
    }
    else if (argument == '9')
    { // Fast Forward
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 100);
      analogWrite(m2_pwm, 100);
    }
    else if (argument == '10')
    { // Fast Backward
      target1 = 20;
      target2 = 20;
      bkw();
      analogWrite(m1_pwm, 100);
      analogWrite(m2_pwm, 100);
    }
    else if (argument == '11')
    { // Fast Left
      target1 = 20;
      target2 = 20;
      lt();
      analogWrite(m1_pwm, 100);
      analogWrite(m2_pwm, 100);
    }
    else if (argument == '12')
    { // Fast Right
      target1 = 20;
      target2 = 20;
      rt();
      analogWrite(m1_pwm, 100);
      analogWrite(m2_pwm, 100);
    }
    else if (argument == '13')
    { // Slow Clock
      target1 = 20;
      target2 = 20;
      cw();
      analogWrite(m1_pwm, 20);
      analogWrite(m2_pwm, 20);
    }
    else if (argument == '14')
    { // Slow Anti Clock
      target1 = 20;
      target2 = 20;
      ccw();
      analogWrite(m1_pwm, 20);
      analogWrite(m2_pwm, 20);
    }
    else if (argument == '15')
    { // Clock
      target1 = 20;
      target2 = 20;
      cw();
      analogWrite(m1_pwm, 35);
      analogWrite(m2_pwm, 35);
    }
    else if (argument == '16')
    { // Anti Clock
      target1 = 20;
      target2 = 20;
      ccw();
      analogWrite(m1_pwm, 35);
      analogWrite(m2_pwm, 35);
    }

    else if (argument == '17')
    { // Forward wheels
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 100);
      analogWrite(m2_pwm, 100);
    }
    else if (argument == '18')
    { // Backward wheels
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 0);
    }
    else if (argument == '19')
    { // Diagonal FL && RR
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 100);
      analogWrite(m2_pwm, 0);
    }

    else if (argument == '20')
    { // Diagonal FR && RL
      target1 = 20;
      target2 = 20;
      fwd();
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 100);
    }

    else if (argument == '0')
    { //  Stop
      target1 = 0;
      target2 = 0;
      stp();
    }
  }
}

void interrupt_routine1()
{
  ctr1++;
}

void interrupt_routine2()
{
  ctr2++;
}

void readspeed()
{
  s1 = newcount1;
  Serial.print("FL = ");
  Serial.print(s1);
  // Serial.print(",");
  Serial.print(" PWM1 = ");
  Serial.print(pwr1);
  Serial.print(" Target = ");
  Serial.println(target1);

  s2 = newcount2;
  Serial.print("FR = ");
  Serial.print(s2);
  Serial.print(" PWM2 = ");
  Serial.print(pwr2);
  Serial.print(" Target = ");
  Serial.println(target2);
}

// HIGH LEVEL MOTOR FUNCTIONS (fwd, bkw, rt, lt, cw, ccw)
// FR->m2  FL->m1
void fwd()
{
  Serial.println("Bot moving forward");
  m1_ccwMotor();
  m2_ccwMotor();
}
void bkw()
{
  Serial.println("Bot moving backwards");
  m1_cwMotor();
  m2_cwMotor();
}
void rt()
{
  Serial.println("Bot moving right");
  m1_cwMotor();
  m2_ccwMotor();
}
void lt()
{
  Serial.println("Bot moving left");
  m1_ccwMotor();
  m2_cwMotor();
}
void ccw()
{
  Serial.println("Bot moving clockwise");
  m1_ccwMotor();
  m2_cwMotor();
}
void cw()
{
  Serial.println("Bot moving counter-clockwise");
  m1_cwMotor();
  m2_ccwMotor();
}
void stp()
{
  Serial.println("Bot stop");
  m1_stopMotor();
  m2_stopMotor();
}

// LOW LEVEL MOTOR FUNCTIONS

void m1_cwMotor()
{
  // Serial.println("Motor A clockwise");
  digitalWrite(m1_dir, HIGH);
  // analogWrite(m1_pwm, pwr1);
}

void m1_ccwMotor()
{
  // Serial.println("Motor A counter-clockwise");
  digitalWrite(m1_dir, LOW);
  // analogWrite(m1_pwm, pwr1);
}

void m2_cwMotor()
{
  // Serial.println("Motor B clockwise");
  digitalWrite(m2_dir, HIGH);
  // analogWrite(m2_pwm, pwr2);
}

void m2_ccwMotor()
{
  // Serial.println("Motor B counter-clockwise");
  digitalWrite(m2_dir, LOW);
  // analogWrite(m2_pwm, pwr2);
}

void m1_stopMotor()
{
  // Serial.println("Motor A stop");
  analogWrite(m1_pwm, 0);
  // analogWrite(m1_pwm, pwr1);
}

void m2_stopMotor()
{
  // Serial.println("Motor B stop");
  analogWrite(m2_pwm, 0);
  // analogWrite(m1_pwm, pwr2);
}