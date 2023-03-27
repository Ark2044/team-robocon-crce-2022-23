// I2C Master with PS4 code /
// List of the libraries included in the project
#include <PS4BT.h>
#include <usbhub.h>
#include <Wire.h> //Arduino library that enables I2C functionality

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

// flags
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

byte I2C_OnOff; // defining the variable that will be sent

long long int prevT1 = 0, currT1, prevT2 = 0, currT2;

void setup()
{
  Wire.begin(); // join I2C bus (address here is optional for master)

  Serial.begin(9600);

#if !defined(MIPSEL)
  while (!Serial)
    ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1)
  {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ; // Halt
  }
  Serial.println(F("\r\nPS4 USB Library Started"));
}

void loop()
{
  Usb.Task();

  if (PS4.connected())
  {
    currT1 = micros();
    currT2 = micros();
    // Serial.println("In");
    if (PS4.getButtonPress(UP))
    {
      if (!f_flag)
      { // send only once char f
        Serial.println("UP");
        I2C_OnOff = 'f';

        Wire.beginTransmission(9);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
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

    else if (PS4.getButtonPress(DOWN))
    {
      if (!b_flag)
      {
        Serial.println("DOWN");
        I2C_OnOff = 'b';

        Wire.beginTransmission(9);  // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 1
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
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

    else if (PS4.getButtonPress(LEFT))
    {
      if (!l_flag)
      {
        I2C_OnOff = 'l';

        Wire.beginTransmission(9);  // Opening the transmission channel to device with the name 19
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 10
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
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

    else if (PS4.getButtonPress(RIGHT))
    {
      if (!r_flag)
      {
        Serial.println("RIGHT");
        I2C_OnOff = 'r';

        Wire.beginTransmission(9);  // Opening the transmission channel to device with the name 9
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 10
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
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

    else if (PS4.getButtonPress(L1))
    {
      if (!a_flag)
      {
        Serial.println("L1");
        I2C_OnOff = 'a';

        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 10
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel

        Wire.beginTransmission(9); // Opening the transmission channel to device with the name 9
        Wire.write(I2C_OnOff);     // Sending the desired information via I2C to the slave device
        Wire.endTransmission();    // Closing the transmission channel
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

    else if (PS4.getButtonPress(R1))
    {
      if (!c_flag)
      {
        Serial.println("R1");
        I2C_OnOff = 'c';

        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 10
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel

        Wire.beginTransmission(9); // Opening the transmission channel to device with the name 9
        Wire.write(I2C_OnOff);     // Sending the desired information via I2C to the slave device
        Wire.endTransmission();    // Closing the transmission channel
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
    else
    {
      if (!s_flag)
      {
        Serial.println("Stop");
        I2C_OnOff = 's';

        Wire.beginTransmission(9);  // Opening the transmission channel to device with the name 9
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
        Wire.beginTransmission(10); // Opening the transmission channel to device with the name 10
        Wire.write(I2C_OnOff);      // Sending the desired information via I2C to the slave device
        Wire.endTransmission();     // Closing the transmission channel
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