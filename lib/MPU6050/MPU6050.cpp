#include <arduino.h>
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

int16_t MPU6050::readInt16(uint8_t* data, uint8_t index) {
    return (int16_t)((data[index] << 8) | data[index + 1]);
}

IMUData MPU6050::readRaw() {
    const uint8_t bytesToRead = 14;
   
    uint8_t rawData[bytesToRead];
    IMUData raw;

    Wire.beginTransmission(i2c_address);
    Wire.write(0x3B); // ACCEL_XOUT_H
    Wire.endTransmission(false);
    
    Wire.requestFrom(i2c_address, bytesToRead);


    for (int i = 0; i < 14; i++) {
        rawData[i] = Wire.read();
    }

    raw.ax = readInt16(rawData, 0);
    raw.ay = readInt16(rawData, 2);
    raw.az = readInt16(rawData, 4);
    raw.temp = readInt16(rawData, 6);
    raw.gx = readInt16(rawData, 8);
    raw.gy = readInt16(rawData, 10);
    raw.gz = readInt16(rawData, 12);

    return raw;
}

IMUData MPU6050::convertToUnits(IMUData& raw, AccelUnit unit) {
    IMUData data;
    float accelScale =  16384.0; // 2g range
    float gyroScale = 131.0; // 250 degrees/s range
    float accelConversion = (unit == G) ? 1.0 : 9.80665; // m/s^2

    data.ax = (raw.ax - ax_offset) / accelScale * accelConversion;
    data.ay = (raw.ay - ay_offset) / accelScale * accelConversion;
    data.az = (raw.az - az_offset) / accelScale * accelConversion;

    data.gx = (raw.gx - gx_offset) / gyroScale; // degrees/s
    data.gy = (raw.gy - gy_offset) / gyroScale;
    data.gz = (raw.gz - gz_offset) / gyroScale; 
    
    data.temp = (raw.temp / 340.0) + 36.53; // Convert to Celsius

    return data;
}

float MPU6050::getAccelerationMagnitude() {
    IMUData raw = readRaw();
    IMUData data = convertToUnits(raw);
    float magnitude = sqrt(pow(data.ax, 2) + pow(data.ay, 2) + pow(data.az, 2));
    return magnitude;
}
float MPU6050::calibrateMagnitudeDelta(unsigned long duration_ms) {
    float minMag = 1000.0f;
    float maxMag = 0.0f;
    unsigned long start = millis();

    while (millis() - start < duration_ms) {
        IMUData raw = readRaw();
        convertToUnits(raw);
        float mag = getAccelerationMagnitude();

        if (mag < minMag) minMag = mag;
        if (mag > maxMag) maxMag = mag;

        delay(10); // 100 Hz
    }
    float delta = maxMag - minMag;
    return delta;
}


void MPU6050::calibrate(uint16_t samples) {
    long ax_sum = 0, ay_sum = 0, az_sum = 0;
    long gx_sum = 0, gy_sum = 0, gz_sum = 0;

    for (uint16_t i = 0; i < samples; i++) {
        IMUData raw = readRaw();

        ax_sum += raw.ax;
        ay_sum += raw.ay;
        az_sum += raw.az;
        
        gx_sum += raw.gx;
        gy_sum += raw.gy;
        gz_sum += raw.gz;
        
        delay(2); // Pequeño delay entre muestras para evitar sobrecarga de I2C
    }

    ax_offset = ax_sum / samples;
    ay_offset = ay_sum / samples;
    az_offset = (az_sum / samples) - 16384;  // Ajuste para que Z = 1g
    gx_offset = gx_sum / samples;
    gy_offset = gy_sum / samples;
    gz_offset = gz_sum / samples;
}


void MPU6050::printOffsets() {
    Serial.print("Accel Offsets: ax=");
    Serial.print(ax_offset);
    Serial.print(", ay=");
    Serial.print(ay_offset);
    Serial.print(", az=");
    Serial.println(az_offset);

    Serial.print("Gyro Offsets: gx=");
    Serial.print(gx_offset);
    Serial.print(", gy=");
    Serial.print(gy_offset);
    Serial.print(", gz=");
    Serial.println(gz_offset);
}

void MPU6050::setOffsets(int16_t ax_off, int16_t ay_off, int16_t az_off, int16_t gx_off, int16_t gy_off, int16_t gz_off) {
    ax_offset = ax_off;
    ay_offset = ay_off;
    az_offset = az_off;
    gx_offset = gx_off;
    gy_offset = gy_off;
    gz_offset = gz_off;
}


void MPU6050::printAcceleration(const IMUData& data) {
    Serial.print("Acceleration (");
    Serial.print("ax: ");
    Serial.print(data.ax, 3);
    Serial.print(", ay: ");
    Serial.print(data.ay, 3);
    Serial.print(", az: ");
    Serial.print(data.az, 3);
    Serial.println(")");
}

void MPU6050::printGyroscope(const IMUData& data) {
    Serial.print("Gyroscope (");
    Serial.print("gx: ");
    Serial.print(data.gx, 3);
    Serial.print(", gy: ");
    Serial.print(data.gy, 3);
    Serial.print(", gz: ");
    Serial.print(data.gz, 3);
    Serial.println(")");
}

void MPU6050::printTemperature(const IMUData& data) {
    Serial.print("Temperature: ");
    Serial.print(data.temp, 2);
    Serial.println(" °C");
}