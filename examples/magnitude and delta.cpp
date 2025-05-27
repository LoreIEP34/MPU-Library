#include <Arduino.h>
#include <MPU6050.h>

// Instancia del sensor
MPU6050 imu;

float minMagnitude = 1000;  // valor alto inicial
float maxMagnitude = 0;     // valor bajo inicial
unsigned long startTime = millis();
unsigned long duration = 10000; // Medir durante 10 segundos
float delta = 0; // Variable para almacenar la diferencia entre max y min

void calibrateMagnitude() {
    while (millis() - startTime < duration) {
        IMUData raw = imu.readRaw();
        imu.convertToUnits(raw);
        float magnitude = imu.getAccelerationMagnitude();

        if (magnitude < minMagnitude) minMagnitude = magnitude;
        if (magnitude > maxMagnitude) maxMagnitude = magnitude;

        Serial.print("Magnitude: ");
        Serial.println(magnitude, 4);
        delay(10);  // 100 Hz
    }
    delta = maxMagnitude - minMagnitude; // Calcula la diferencia
    Serial.println("---- Calibración completa ----");
    Serial.print("Mínima magnitud: ");
    Serial.println(minMagnitude, 4);
    Serial.print("Máxima magnitud: ");
    Serial.println(maxMagnitude, 4);
    Serial.print("Delta: ");
    Serial.println(delta, 4);
  }

void setup() {
  Serial.begin(115200);
  delay(1000);

  int sda = 8, scl = 9; // Cambia estos valores si usas pines diferentes
  imu.setPins(sda, scl); // Configura los pines SDA y SCL
 
  if (!imu.begin()) {
    Serial.println("Error al iniciar MPU6050");
    while (1) delay(1000);
  }
  Serial.println("MPU6050 iniciado correctamente.");
    imu.setOffsets(0, 0, 0, 0, 0, 0); // Ajustá si ya tenés calibración
    imu.printOffsets();

    calibrateMagnitude(); // Llama a la función de calibración
 
}

void loop() {
 
    IMUData rawData = imu.readRaw();
    IMUData data = imu.convertToUnits(rawData);

    float magnitude = imu.getAccelerationMagnitude();

    Serial.print("Magnitud: ");
    Serial.println(magnitude, 4); // Cuatro decimales para precisión

    delay(200); // Muestreo cada 200ms
}
