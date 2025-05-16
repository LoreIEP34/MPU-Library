#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"

MPU6050 imu;

void setup() {
  Serial.begin(115200);
  delay(1000);

  int sda = 8, scl = 9; // Cambia estos valores si usas pines diferentes
  imu.setPins(sda, scl); // Configura los pines SDA y SCL
  // Inicializa el sensor
  if (!imu.begin()) {
    Serial.println("Error al iniciar MPU6050");
    while (1) delay(1000);
  }
  Serial.println("MPU6050 iniciado correctamente.");

  // Calibración automática (100 muestras)
  Serial.println("Calibrando...");
  imu.calibrate(100);
  imu.printOffsets();

  // O bien, si querés setear offsets manualmente, descomenta y ajusta:
  /*
  imu.setOffsets(100, -50, 16300, 10, 5, -3);
  imu.printOffsets();
  */
}

void loop() {
  // Leer datos crudos
  IMUData rawData = imu.readRaw();

  // Convertir con offsets aplicados, en G
  IMUData data = imu.ConvertToUnits(rawData, G);

  // Mostrar valores
  imu.printAcceleration(data);
  imu.printGyroscope(data);
  imu.printTemperature(data);

  delay(500);
}
