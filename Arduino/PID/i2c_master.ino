// I2C Master write code /
// List of the lubraries included in the project****** /
#include <Wire.h>                                                           //Arduino library that enables I2C functionality
// defining local variables that will be used in the program****** /
String readString;                                                          // defining the string
int ChoiceOfTransmission;                                                   // definig a varijable that will be decide to which arduino are we sending the data
// List of the variables that will be sent via I2C****** /
byte I2C_OnOff;                                                             //defining the variable that will be sent
/* Setup loop*********** /

/*
  Example sketch for the PS4 Bluetooth library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/
// #include <PS4BT.h>
// #include <usbhub.h>
// #include<Wire.h>
// // Satisfy the IDE, which needs to see the include statment in the ino too.
// #ifdef dobogusinclude
// #include <spi4teensy3.h>
// #endif
// #include <SPI.h>

// USB Usb;
// //USBHub Hub1(&Usb); // Some dongles have a hub inside
// //PS4USB PS4(&Usb);  // You have to create the Bluetooth Dongle instance like so

// /* You can create the instance of the PS4BT class in two ways */
// // This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// // You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode

// BTD Btd(&Usb);
// PS4BT PS4(&Btd, PAIR);
// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

// bool printAngle, printTouch;

// #include <SoftwareSerial.h>
// SoftwareSerial softSerial(19, 18);

char ip;
String value="";
int value_to_int;


void setup() {
  Wire.begin();                                                             // join I2C bus (address here is optional for master)
  Serial.println("Type On to turn on the LED and Off to shut it down!");    // Instruction text and check off what is loaded

  Serial.begin(9600);
  Serial2.begin(9600);
// #if !defined(MIPSEL)
//   while (!Serial)
//     ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
// #endif
//   if (Usb.Init() == -1) {
//     //Serial.print(F("\r\nOSC did not start"));
//     while (1);  // Halt
//   }
//   Serial.println(F("\r\nPS4 USB Library Started"));
}

void loop() {
  // Usb.Task();

  // if (PS4.connected()) {

  //   if (PS4.getButtonClick(CIRCLE)) {
  //     Serial.println("100");
  //     I2C_OnOff = 100;
  //     //ChoiceOfTransmission = 1;
  //     Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
  //     Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //     Wire.endTransmission();                                                   // Closeing the transmition chanel
  //     //ChoiceOfTransmission = 2; 
  //     Wire.beginTransmission(9);                                                // Oening the transmition chanel to device with the name 1
  //     Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //     Wire.endTransmission();                                                   // Closeing the transmition chanel
  //     //ChoiceOfTransmission = 0;
  //     //ChoiceOfTransmission = 2;

  //   }

  //   else if (PS4.getButtonClick(CROSS)) {
  //     Serial.println("0");
  //     I2C_OnOff = 0;
  //     Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
  //       Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //       Wire.endTransmission();                                                   // Closeing the transmition chanel
  //       //ChoiceOfTransmission = 2; 
  //       Wire.beginTransmission(9);                                                // Oening the transmition chanel to device with the name 1
  //       Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //       Wire.endTransmission();
  //   }
  //   /************/
  //   // Switch statemant that depending on the chosen slave arduino executes the transmition of dat* /
  //   switch (ChoiceOfTransmission)
  //   {
  //     case 1:               //Sending data on the first arduino
  //       Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
  //       Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //       Wire.endTransmission();                                                   // Closeing the transmition chanel
  //       //ChoiceOfTransmission = 2; 
  //       Wire.beginTransmission(9);                                                // Oening the transmition chanel to device with the name 1
  //       Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //       Wire.endTransmission();                                                   // Closeing the transmition chanel
  //       ChoiceOfTransmission = 0;                                               // setting the case choice varijable to so that there are no repeat transmitions unless user types the comand again
  //       break;
  //     case 2:               //Sending data on the second arduino
  //       // Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
  //       // Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
  //       // Wire.endTransmission();                                                   // Closeing the transmition chanel
  //       // ChoiceOfTransmission = 0;                                                 // setting the case choice varijable to so that there are no repeat transmitions unless user types the comand again
  //       break;
  //     default:              // if the user choice is not put in or the command has
  //       // allready been sent then choice variable is 0 or set to 0,
  //       // which is out of the range for command to tansmit data,
  //       // if that is the case then this part of the code is executed.
  //       break;
  //   }

  //   Wire.requestFrom(9, 6);    // request 6 bytes from slave device #8

  //   while (Wire.available()) { // slave may send less than requested
  //     char c = Wire.read(); // receive a byte as character
  //     Serial.print(c);         // print the character
  //   }

  //   Wire.requestFrom(10, 6);    // request 6 bytes from slave device #8


  //   while (Wire.available()) { // slave may send less than requested
  //     char c = Wire.read(); // receive a byte as character
  //     Serial.print(c);         // print the character
  //   }
  //   /************/

  // }

  while(Serial2.available())
  {
    ip=Serial2.read();
    value+=ip;
  }
  // value_to_int=value.toInt();
  Serial1.print("Value: ");
  Serial1.println(value);
  if (value=="100") {
      Serial.println("100");
      I2C_OnOff = 100;
      //ChoiceOfTransmission = 1;
      Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
      Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
      Wire.endTransmission();                                                   // Closeing the transmition chanel
      //ChoiceOfTransmission = 2; 
      Wire.beginTransmission(9);                                                // Oening the transmition chanel to device with the name 1
      Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
      Wire.endTransmission();                                                   // Closeing the transmition chanel
      //ChoiceOfTransmission = 0;
      //ChoiceOfTransmission = 2;

    }

    else if (value=="0") {
      Serial.println("0");
      I2C_OnOff = 0;
      Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
        Wire.endTransmission();                                                   // Closeing the transmition chanel
        //ChoiceOfTransmission = 2; 
        Wire.beginTransmission(9);                                                // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
        Wire.endTransmission();
    }
    value="";

    /************/
    // Switch statemant that depending on the chosen slave arduino executes the transmition of dat* /
    switch (ChoiceOfTransmission)
    {
      case 1:               //Sending data on the first arduino
        Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
        Wire.endTransmission();                                                   // Closeing the transmition chanel
        //ChoiceOfTransmission = 2; 
        Wire.beginTransmission(9);                                                // Oening the transmition chanel to device with the name 1
        Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
        Wire.endTransmission();                                                   // Closeing the transmition chanel
        ChoiceOfTransmission = 0;                                               // setting the case choice varijable to so that there are no repeat transmitions unless user types the comand again
        break;
      case 2:               //Sending data on the second arduino
        // Wire.beginTransmission(10);                                                // Oening the transmition chanel to device with the name 1
        // Wire.write(I2C_OnOff);                                                    // Sending the desired information via I2C to the slave device
        // Wire.endTransmission();                                                   // Closeing the transmition chanel
        // ChoiceOfTransmission = 0;                                                 // setting the case choice varijable to so that there are no repeat transmitions unless user types the comand again
        break;
      default:              // if the user choice is not put in or the command has
        // allready been sent then choice variable is 0 or set to 0,
        // which is out of the range for command to tansmit data,
        // if that is the case then this part of the code is executed.
        break;
    }

    Wire.requestFrom(9, 6);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
    }

    Wire.requestFrom(10, 6);    // request 6 bytes from slave device #8


    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
    }

}