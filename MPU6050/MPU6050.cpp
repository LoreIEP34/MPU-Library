#include "MPU6050.h"

void MPU6050::setPins(int sda, int scl) {
    sda_pin = sda;
    scl_pin = scl;
}

bool MPU6050::begin() {

    if (sda_pin >= 0 && scl_pin >= 0) { // Check if custom pins are set
        Wire.begin(sda_pin, scl_pin); // Initialize I2C with custom pins
    } else {
        Wire.begin();  // Initialize I2C with default pins
    }

    // Check connection with the sensor
    Wire.beginTransmission(i2c_address);
    uint8_t error = Wire.endTransmission();

    if (error != 0) {
        return false;  // Sensor does not respond at the specified address
    }

    // Wake up the sensor (PWR_MGMT_1 register)
    Wire.beginTransmission(i2c_address);
    Wire.write(0x6B); // Register address for PWR_MGMT_1
    Wire.write(0x00);  // Set SLEEP bit to 0 to wake up the sensor
    Wire.endTransmission();

    delay(100); // Allow time for the sensor to stabilize
    return true;
}
