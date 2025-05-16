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

void MPU6050::readAccel(float& ax, float& ay, float& az) {
    // Read accelerometer data from the sensor
    Wire.beginTransmission(i2c_address);
    Wire.write(0x3B); // Starting register for accelerometer data
    Wire.endTransmission(false);
    Wire.requestFrom(i2c_address, 6, true); // Request 6 bytes

    ax = (Wire.read() << 8 | Wire.read()) / 16384.0; // Convert to g
    ay = (Wire.read() << 8 | Wire.read()) / 16384.0;
    az = (Wire.read() << 8 | Wire.read()) / 16384.0;
}

void MPU6050::readGyro(float& gx, float& gy, float& gz) {
    // Read gyroscope data from the sensor
    Wire.beginTransmission(i2c_address);
    Wire.write(0x43); // Starting register for gyroscope data
    Wire.endTransmission(false);
    Wire.requestFrom(i2c_address, 6, true); // Request 6 bytes

    gx = (Wire.read() << 8 | Wire.read()) / 131.0; // Convert to degrees/sec
    gy = (Wire.read() << 8 | Wire.read()) / 131.0;
    gz = (Wire.read() << 8 | Wire.read()) / 131.0;
}

float MPU6050::readTemp() {
    // Read temperature data from the sensor
    Wire.beginTransmission(i2c_address);
    Wire.write(0x41); // Starting register for temperature data
    Wire.endTransmission(false);
    Wire.requestFrom(i2c_address, 2, true); // Request 2 bytes

    int16_t tempRaw = (Wire.read() << 8 | Wire.read());
    return tempRaw / 340.0 + 36.53; // Convert to degrees Celsius
}
