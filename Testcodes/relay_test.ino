#define RELAY1 7 // Defining the pin 7 of the Arduino for the 4 relay module
#define RELAY2 6 // Defining the pin 6 of the Arduino for the 4 relay module
#define RELAY3 5 // Defining the pin 5 of the Arduino for the 4 relay module
#define RELAY4 4 // Defining the pin 4 of the Arduino for the 4 relay module

void setup()
{
    pinMode(RELAY1, OUTPUT); // Defining the pin 7 of the Arduino as output
    pinMode(RELAY2, OUTPUT); // Defining the pin 6 of the Arduino as output
    pinMode(RELAY3, OUTPUT); // Defining the pin 5 of the Arduino as output
    pinMode(RELAY4, OUTPUT); // Defining the pin 4 of the Arduino as output
}

void loop()
{
    digitalWrite(RELAY1, LOW);  // This will Turn ON the relay 1
    delay(5000);                // Wait for 5 seconds
    digitalWrite(RELAY1, HIGH); // This will Turn the Relay Off

    digitalWrite(RELAY2, LOW);  // This will Turn ON the relay
    delay(5000);                // Wait for 5 seconds
    digitalWrite(RELAY2, HIGH); // This will Turn the Relay Off

    digitalWrite(RELAY3, LOW);  // This will Turn ON the relay
    delay(5000);                // Wait for 5 seconds
    digitalWrite(RELAY3, HIGH); // This will Turn the Relay Off

    digitalWrite(RELAY4, LOW);  // This will Turn ON the relay
    delay(5000);                // Wait for 5 seconds
    digitalWrite(RELAY4, HIGH); // This will Turn the Relay Off
}