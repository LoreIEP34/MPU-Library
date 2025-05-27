#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"

MPU6050 imu;
float delta;

void setup() {
  Serial.begin(115200);
  delay(1000);

  int sda = 8, scl = 9; 
  imu.setPins(sda, scl);
  // Inicializa el sensor
  if (!imu.begin()) {
    Serial.println("Error al iniciar MPU6050");
    while (1) delay(1000);
  }
  Serial.println("MPU6050 iniciado correctamente.");

  // Calibración automática (100 muestras)
  Serial.println("Calibrando...");
  imu.calibrate(100);
  
  Serial.println("Iniciando calibración de magnitud...");
  delta = imu.calibrateMagnitudeDelta();
  

}

void loop() {
  // Leer datos crudos
  IMUData rawData = imu.readRaw();

  // Convertir con offsets aplicados, en G
  IMUData data = imu.convertToUnits(rawData);

  float magnitude = imu.getAccelerationMagnitude();

  // Mostrar valores
  imu.printAcceleration(data);
  imu.printGyroscope(data);
  imu.printTemperature(data);
  imu.printOffsets();
  Serial.print("Magnitud: ");
  Serial.println(magnitude, 4); 
  Serial.print("Delta de magnitud en reposo: ");
  Serial.println(delta, 4);
  delay(200);
}
