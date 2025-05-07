#ifndef IMU_H
#define IMU_H


class IMU {
    public:
        virtual ~IMU() {}
        virtual bool begin() = 0;
        virtual void readAccel(float& ax, float& ay, float& az) = 0;
        virtual void readGyro(float& gx, float& gy, float& gz) = 0;
        virtual float readTemp() = 0;
    };
    #endif
    
