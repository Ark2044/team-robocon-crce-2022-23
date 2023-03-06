#include <PS4BT.h>
#include <usbhub.h>
// #include <PS4USB.h>
//#include <SoftwareSerial.h>
#include <Wire.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
// PS4USB PS4(&Usb);

// SoftwareSerial softSerial(17, 16);
//USBHub Hub1(&Usb); // Some dongles have a hub inside
//PS4USB PS4(&Usb);  // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode

BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);

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
  Usb.Task();

  if (PS4.connected()) {
    //Serial.println("In");
    if (PS4.getButtonClick(CIRCLE)) {
      //Serial.println("CIRCLE");
      Serial2.write("100");
      delay(100);
    }

    if (PS4.getButtonClick(CROSS)) {
      //Serial.println("CROSS");
      Serial2.write("0");
      delay(100);
    }
  }
}