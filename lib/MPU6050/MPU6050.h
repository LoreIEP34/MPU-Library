#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include "IMU.h"


class MPU6050 : public IMU {
   
    public:
    MPU6050() : i2c_address(0x68) {} // Default address
    MPU6050(bool ad0_high) : i2c_address(ad0_high ? 0x69 : 0x68) {}
    MPU6050(uint8_t address) : i2c_address(address) {}

    void setPins(int sda, int scl);
    bool begin() override;
     
    IMUData readRaw() override;
    IMUData convertToUnits(IMUData& raw, AccelUnit unit = G);

    void calibrate(uint16_t samples = 100);
    void printOffsets();
    void setOffsets(int16_t ax_off, int16_t ay_off, int16_t az_off, int16_t gx_off, int16_t gy_off, int16_t gz_off);
    
    float getAccelerationMagnitude();
    float calibrateMagnitudeDelta(unsigned long duration_ms = 5000);


    void printAcceleration(const IMUData& data);
    //void printAccelMagnitude(const IMUData& data);
    //void printAccelDelta(const IMUData& data);

    void printGyroscope(const IMUData& data);
    //void printGyroMagnitude(const IMUData& data);
    //void printGyroDelta(const IMUData& data);

    void printTemperature(const IMUData& data);
    
    private:
    uint8_t i2c_address;
    int sda_pin = -1;
    int scl_pin = -1;
    int16_t readInt16(uint8_t* data, uint8_t index);
    int16_t ax_offset = 0, ay_offset = 0, az_offset = 0;
    int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;
    IMUData raw;
    IMUData data;

};
#endif


