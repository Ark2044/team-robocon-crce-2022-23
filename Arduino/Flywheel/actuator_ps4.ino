#include <PS4USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS4USB PS4(&Usb);

void setup() {
Serial.begin(115200);
#if !defined(_MIPSEL_)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  pinMode(7, OUTPUT); // Configure pin 7 as an Output
  pinMode(8, OUTPUT); // Configure pin 8 as an Output

  digitalWrite(7, HIGH); // Initialize pin 7 as Low
  digitalWrite(8, HIGH); // Initialize pin 7 as Low
  Serial.print(F("\r\nPS4 USB Library Started"));

}

void loop() {
  Usb.Task();

  if (PS4.connected())
  {
      if (PS4.getButtonClick(UP)) {
        // Extend Linear Actuator
        digitalWrite(7, LOW);
        digitalWrite(8, HIGH);
        delay(2000); // 2 seconds

        // Stops Actuator
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);

        delay(2000); // 2 seconds
      }
      
    if (PS4.getButtonClick(DOWN)) {
        // Retracts Linear Actuator
        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);
        delay(2000); // 2 seconds

        // Stop Actuator
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);

        delay(2000); // 2 seconds
      }
  }
}