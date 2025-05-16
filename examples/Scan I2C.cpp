#include <Arduino.h>
#include <Wire.h>

void setup() {
  delay(2000);  // Wait for 2 seconds before starting
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial to initialize

  Serial.println("Scanning for I2C devices...");
  Wire.begin(8, 9);  // Change these pins if using different ones

  byte count = 0;
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at 0x");
      Serial.println(address, HEX);
      count++;
    }
  }

  if (count == 0) {
    Serial.println("No I2C devices found.");
  } else {
    Serial.println("Scan complete.");
  }
}

void loop() {}
