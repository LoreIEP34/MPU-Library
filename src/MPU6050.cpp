#include <arduino.h>
#include "MPU6050.h"

void MPU6050::setPins(int sda, int scl) {
    sda_pin = sda;
    scl_pin = scl;
}

bool MPU6050::isConnected() {
    Wire.beginTransmission(i2c_address);
    return Wire.endTransmission() == 0;
}

bool MPU6050::begin() {
    static bool wireInitialized = false;
    static bool isInitialized = false;


    if (!wireInitialized) {
        if (sda_pin >= 0 && scl_pin >= 0) { // Check if custom pins are set
            Wire.begin(sda_pin, scl_pin); // Initialize I2C with custom pins
        } else {
            Wire.begin();  // Initialize I2C with default pins
        }
        wireInitialized = true; // Set the flag to true after initialization
    }   

    if (!isConnected()) return false;

    if (isInitialized) return true; // If already initialized, skip further setup
    // Wake up the sensor (PWR_MGMT_1 register)
    Wire.beginTransmission(i2c_address);
    Wire.write(0x6B); // Register address for PWR_MGMT_1
    Wire.write(0x00);  // Set SLEEP bit to 0 to wake up the sensor
    Wire.endTransmission();

    isInitialized = true; // Set the flag to true after initialization
    delay(100); // Allow time for the sensor to stabilize
    return true;
}

int16_t MPU6050::readInt16(uint8_t* data, uint8_t index) {
    return (int16_t)(((uint16_t)data[index] << 8) | data[index + 1]);
}

bool MPU6050::readRegisters(uint8_t startRegister, uint8_t* buffer, uint8_t length) {
    Wire.beginTransmission(i2c_address);
    Wire.write(startRegister);
    if (Wire.endTransmission(false) != 0) return false;
    
    Wire.requestFrom(i2c_address, length);
    if (Wire.available() < length) return false;

    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = Wire.read();
    }
    return true;
}

IMUData MPU6050::readRaw() {
    const uint8_t bytesToRead = 14;
    uint8_t rawData[bytesToRead];

    if (!readRegisters(0x3B, rawData, bytesToRead)) {
        Serial.println("Error leyendo datos del MPU6050");
        raw.ready = false; 
        return raw;
    }

    raw.accel.x = readInt16(rawData, 0);
    raw.accel.y = readInt16(rawData, 2);
    raw.accel.z = readInt16(rawData, 4);

    raw.temp.data = readInt16(rawData, 6); // Temperatura en 16 bits
    
    raw.gyro.x = readInt16(rawData, 8);
    raw.gyro.y = readInt16(rawData, 10);
    raw.gyro.z = readInt16(rawData, 12);

    raw.ready = true; // Marca que los datos están listos
    return raw;

}

IMUData MPU6050::convertToUnits(IMUData& raw, AccelUnit unit) {
    float accelScale =  16384.0; // 2g range
    float gyroScale = 131.0; // 250 degrees/s range
    float accelConversion = (unit == G) ? 1.0 : 9.80665; // m/s^2

    data.accel.x = (raw.accel.x - ax_offset) / accelScale * accelConversion;
    data.accel.y = (raw.accel.y - ay_offset) / accelScale * accelConversion;
    data.accel.z = (raw.accel.z - az_offset) / accelScale * accelConversion;

    data.gyro.x = (raw.gyro.x - gx_offset) / gyroScale; // degrees/s    
    data.gyro.y = (raw.gyro.y - gy_offset) / gyroScale;
    data.gyro.z = (raw.gyro.z - gz_offset) / gyroScale;

    data.temp.data = (raw.temp.data / 340.0) + 36.53; // Convert to Celsius

    return data;
}

float MPU6050::getAccelerationMagnitude() {
    
    IMUData data = convertToUnits(raw);
    float magnitude = sqrt(pow(data.accel.x, 2) + pow(data.accel.y, 2) + pow(data.accel.z, 2));
    return magnitude;
}
float MPU6050::calibrateMagnitudeDelta(unsigned long duration_ms) {
    float minMag = 1000.0f;
    float maxMag = 0.0f;
    unsigned long start = millis();

    while (millis() - start < duration_ms) {
        raw = readRaw();
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
        raw = readRaw();

        ax_sum += raw.accel.x;
        ay_sum += raw.accel.y;
        az_sum += raw.accel.z;
        
        gx_sum += raw.gyro.x;
        gy_sum += raw.gyro.y;
        gz_sum += raw.gyro.z;
        
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
    Serial.print(data.accel.x, 3);
    Serial.print(", ay: ");
    Serial.print(data.accel.y, 3);
    Serial.print(", az: ");
    Serial.print(data.accel.z, 3);
    Serial.println(")");
}

void MPU6050::printGyroscope(const IMUData& data) {
    Serial.print("Gyroscope (");
    Serial.print("gx: ");
    Serial.print(data.gyro.x, 3);
    Serial.print(", gy: ");
    Serial.print(data.gyro.y, 3);
    Serial.print(", gz: ");
    Serial.print(data.gyro.z, 3);
    Serial.println(")");
}

void MPU6050::printTemperature(const IMUData& data) {
    Serial.print("Temperature: ");
    Serial.print(data.temp.data, 2);
    Serial.println(" °C");
}


