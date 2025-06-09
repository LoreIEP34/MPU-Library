#ifndef IMU_H
#define IMU_H

enum AccelUnit {
    G,
    MS2
};



struct AccelData {
    float x = 0, y = 0, z = 0;
};
struct GyroData {
    float x = 0, y = 0, z = 0;
};
struct TempData {
    float data = 0;
};


struct IMUData {
    AccelData accel;
    GyroData gyro;
    TempData temp;
    bool ready = false; 
};

class IMU {
public:
    virtual ~IMU() {}
    virtual bool begin() = 0;
    virtual IMUData readRaw() = 0;
    virtual IMUData convertToUnits(IMUData& raw, AccelUnit unit = G) = 0;
    virtual void calibrate(uint16_t samples = 100) = 0;  
    
};

#endif