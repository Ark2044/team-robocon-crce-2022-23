//UART MASTER
#include <PS4BT.h>
#include <usbhub.h>
// #include <PS4USB.h>
#include <Wire.h>
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
    if (PS4.getButtonPress(UP)) {
      Serial.println("UP");
      // Serial2.write("f");
      // delay(100);
      if (!f_flag) {                    //send only once string f
        Serial.println("UP");
        Serial2.write("f");
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
        Serial2.write("b");
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
        Serial2.write("l");
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
        Serial2.write("r");
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
        Serial2.write("a");
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
        Serial2.write("c");
        c_flag = true;
        delay(100);
      }

        f_flag = false;
        b_flag = false;
        l_flag = false;
        r_flag = false;
        s_flag = false;
        a_flag = false;
    }
    else{
      // Serial.println("Stop");
      // Serial2.write("s");
      // delay(100);
      if (!s_flag) {
        Serial.println("Stop");
        Serial2.write("s");
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
  }
}