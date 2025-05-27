#ifndef IMU_H
#define IMU_H

enum AccelUnit {
    G,
    MS2
};

struct IMUData {
    float ax, ay, az;
    float gx, gy, gz;
    float temp = 0.0; // si querés incluirla por defecto
};

class IMU {
public:
    virtual ~IMU() {}
    virtual bool begin() = 0;
    virtual IMUData readRaw() = 0;
    virtual IMUData convertToUnits(IMUData& raw, AccelUnit unit = G) = 0;
    virtual void calibrate(uint16_t samples = 100) = 0;  // Agregar aquí también
    
};

#endif