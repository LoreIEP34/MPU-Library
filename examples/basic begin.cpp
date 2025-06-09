#include <Arduino.h>
#include "MPU6050.h"

// Create the sensor object
MPU6050 sensor;  // default address 0x68
// If you want to use address 0x69, use MPU6050 sensor(true); or MPU6050 sensor(0x69);

void setup() {
    Serial.begin(115200);
    delay(1500);  // Wait for the serial monitor to open
    while (!Serial);

 // sensor.setPins(21, 22); // If you want to use specific pins, uncomment this line and adjust the pin numbers


  // Initialize the sensor
    if (!sensor.begin()) {
        Serial.println("Error: Could not initialize the MPU6050");
        while (1);  // Stop execution if not connected
    }
    Serial.println("MPU6050 initialized successfully");
    }

void loop() {
  // Here you could add code to read data from the sensor, but we will only test the connection

  delay(1000);  // 1-second delay
}

