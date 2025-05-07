#includes "MPU6050.h"

void MPU6050::setPins(int sda, int scl) {
    sda_pin = sda;
    scl_pin = scl;
}

bool MPU6050::begin() {

    if (sda_pin >= 0 && scl_pin >= 0) { // Check if pins are set
        Wire.begin(sda_pin, scl_pin); // Use custom pins
    } else {
        Wire.begin();  // Use default pins  
    }
// Verificar conexión con el sensor
Wire.beginTransmission(i2c_address);
uint8_t error = Wire.endTransmission();

if (error != 0) {
    return false;  // No responde en esa dirección
}

// Despertar al sensor (registro PWR_MGMT_1)
Wire.beginTransmission(i2c_address);
Wire.write(0x6B);
Wire.write(0x00);  // Bit SLEEP en 0
Wire.endTransmission();

    delay(100);
    return true;
}
