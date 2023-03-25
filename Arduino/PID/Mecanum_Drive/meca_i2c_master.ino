// I2C Master with PS4 code /
// List of the lubraries included in the project****** /
#include <PS4BT.h>
#include <usbhub.h>
#include <Wire.h>  //Arduino library that enables I2C functionality
// defining local variables that will be used in the program****** /

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

//flags
bool f_flag = false;
bool b_flag = false;
bool l_flag = false;
bool r_flag = false;
bool s_flag = false;
bool c_flag = false;
bool a_flag = false;

USB Usb;
// PS4USB PS4(&Usb);

BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

String readString;         // defining the string
int ChoiceOfTransmission;  // definig a varijable that will be decide to which arduino are we sending the data
// List of the variables that will be sent via I2C****** /
byte I2C_OnOff;  //defining the variable that will be sent
// byte dir1,dir2,dir3,dir4;
// byte senderArray1[]={I2C_OnOff,dir1,dir2};
// byte senderArray2[]={I2C_OnOff,dir3,dir4};
char ip;
String value = "";
int value_to_int;

long long int prevT1 = 0, currT1, prevT2 = 0, currT2;


void setup() {
  // pinMode(resetPin,OUTPUT);
  // digitalWrite(resetPin,HIGH);
  Wire.begin();  // join I2C bus (address here is optional for master)
  //Serial.println("Type On to turn on the LED and Off to shut it down!");    // Instruction text and check off what is loaded

  Serial.begin(9600);
  // Serial2.begin(9600);

#if !defined(MIPSEL)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  Serial.println(F("\r\nPS4 USB Library Started"));
}

void loop() {
  // while (Serial2.available()) {
  //   ip = Serial2.read();
  //   value += ip;
  // }
  // value_to_int=value.toInt();
  // Serial1.print("Value: ");
  // Serial1.println(value);
  Usb.Task();

  if (PS4.connected()) {
    currT1 = micros();
    currT2 = micros();
    //Serial.println("In");
    if (PS4.getButtonPress(UP)) {
      Serial.println("UP");
      // Serial2.write("f");
      // delay(100);
      if (!f_flag) {  //send only once string f
        Serial.println("UP");
        I2C_OnOff = 'f';

        Wire.beginTransmission(9);   // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        f_flag = true;
        delay(100);
      }

      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(DOWN)) {
      Serial.println("DOWN");
      // Serial2.write("b");
      // delay(100);
      if (!b_flag) {
        Serial.println("DOWN");
        I2C_OnOff = 'b';


        Wire.beginTransmission(9);   // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        b_flag = true;
        delay(100);
      }
      f_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(LEFT)) {
      // Serial.println("LEFT");
      // Serial2.write("l");
      // delay(100);
      if (!l_flag) {
        Serial.println("LEFT");
        I2C_OnOff = 'l';


        Wire.beginTransmission(9);   // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        l_flag = true;
        delay(100);
      }

      f_flag = false;
      b_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(RIGHT)) {
      // Serial.println("RIGHT");
      // Serial2.write("r");
      // delay(100);
      if (!r_flag) {
        Serial.println("RIGHT");
        I2C_OnOff = 'r';

        Wire.beginTransmission(9);   // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        r_flag = true;
        delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      s_flag = false;
      c_flag = false;
      a_flag = false;
    }

    else if (PS4.getButtonPress(L1)) {
      // Serial.println("L1");
      // Serial2.write("a");
      // delay(100);
      if (!a_flag) {
        Serial.println("L1");
        I2C_OnOff = 'a';


        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel

        Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closeing the transmition chanel
        a_flag = true;
        delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      c_flag = false;
    }

    else if (PS4.getButtonPress(R1)) {
      // Serial.println("R1");
      // Serial2.write("c");
      // delay(100);
      if (!c_flag) {
        Serial.println("R1");
        I2C_OnOff = 'c';


        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel

        Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closeing the transmition chanel
        c_flag = true;
        delay(100);
      }

      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      s_flag = false;
      a_flag = false;
    } else {
      // Serial.println("Stop");
      // Serial2.write("s");
      // delay(100);
      if (!s_flag) {
        Serial.println("Stop");
        I2C_OnOff = 's';



        Wire.beginTransmission(9);   // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
        Wire.endTransmission();      // Closeing the transmition chanel
        s_flag = true;
        delay(100);
      }
      f_flag = false;
      b_flag = false;
      l_flag = false;
      r_flag = false;
      c_flag = false;
      a_flag = false;
    }

    // if(currT1-prevT1==13000000)
    // {
    // Wire.requestFrom(9, 6);  // request 6 bytes from slave device #8
    // String s1;
    // Serial.print("Slave 1: ");
    // while (Wire.available()) {  // slave may send less than requested
    //   s1 = Wire.readString();     // receive a byte as character
    // }
    // Serial.println(s1);          // print the character
    // prevT1=currT1;
    // }

    // if(currT2-prevT2==17000000)
    // {

    // Wire.requestFrom(10, 6);  // request 6 bytes from slave device #8
    // String s2;
    // Serial.print("Slave 2: ");
    // while (Wire.available()) {  // slave may send less than requested
    //   s2 = Wire.readString();     // receive a byte as character
    // }
    // Serial.println(s2);          // print the character
    // prevT2=currT2;
  }
}