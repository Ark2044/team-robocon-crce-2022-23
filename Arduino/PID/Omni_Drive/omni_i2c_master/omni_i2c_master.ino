// I2C Master write code /
// List of the lubraries included in the project****** /
#include <Wire.h>  //Arduino library that enables I2C functionality
// defining local variables that will be used in the program****** /
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

const int resetPin=40;


void setup() {
  // pinMode(resetPin,OUTPUT);
  // digitalWrite(resetPin,HIGH);  
  Wire.begin();  // join I2C bus (address here is optional for master)
  //Serial.println("Type On to turn on the LED and Off to shut it down!");    // Instruction text and check off what is loaded

  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  while (Serial2.available()) {
    ip = Serial2.read();
    value += ip;
  }
  // value_to_int=value.toInt();
  // Serial1.print("Value: ");
  // Serial1.println(value);
  if (value == "f") {
    Serial.print("value: ");
    Serial.println(value);

    I2C_OnOff = 'f';

    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel
  }

  else if (value == "b") {
    Serial.print("value: ");
    Serial.println(value);
    //Serial.println("100");
    I2C_OnOff = 'b';


    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel
  }

  else if (value == "r") {
    Serial.print("value: ");
    Serial.println(value);
    //Serial.println("100");
    I2C_OnOff = 'r';

    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel
  }

  else if (value == "l") {
    Serial.print("value: ");
    Serial.println(value);
    //Serial.println("100");
    I2C_OnOff = 'l';


    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel
  }

  else if (value == "s") {
Serial.print("value: ");
    Serial.println(value);    
    //Serial.println("0");
    I2C_OnOff = 's';



    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel
    // digitalWrite(resetPin,LOW);    
  }

  else if (value == "a") {
    Serial.print("value: ");
    Serial.println(value);
    //Serial.println("0");
    I2C_OnOff = 'a';


    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel

    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
  }

  else if (value == "c") {
    Serial.print("value: ");
    Serial.println(value);
    //Serial.println("0");
    I2C_OnOff = 'c';


    Wire.beginTransmission(10);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);       // Sending the desired information via I2C to the slave device
    Wire.endTransmission();      // Closeing the transmition chanel

    Wire.beginTransmission(9);  // Oening the transmition chanel to device with the name 1
    Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
    Wire.endTransmission();     // Closeing the transmition chanel
  }

  value = "";

  Wire.requestFrom(9, 6);  // request 6 bytes from slave device #8

  while (Wire.available()) {  // slave may send less than requested
    char c = Wire.read();     // receive a byte as character
    Serial.print(c);          // print the character
  }

  Wire.requestFrom(10, 6);  // request 6 bytes from slave device #8


  while (Wire.available()) {  // slave may send less than requested
    char c = Wire.read();     // receive a byte as character
    Serial.print(c);          // print the character
  }
}