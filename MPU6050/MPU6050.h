#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include "lib/IMU/IMU.h"


class MPU6050 : public IMU {
    private:
    uint8_t i2c_address;
    int sda_pin = -1;
    int scl_pin = -1;

    public:
    MPU6050() : i2c_address(0x68) {} // Default address
    MPU6050(bool ad0_high) : i2c_address(ad0_high ? 0x69 : 0x68) {}
    MPU6050(uint8_t address) : i2c_address(address) {}

    void setPins(int sda, int scl);
    bool begin() override;
    void readAccel(float& ax, float& ay, float& az) override;
    void readGyro(float& gx, float& gy, float& gz) override;
    float readTemp() override;

};
#endif


