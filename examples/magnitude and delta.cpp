#include <Arduino.h>
#include <MPU6050.h>

// Instancia del sensor
MPU6050 imu;


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

    Serial.println("Iniciando calibración de magnitud...");
    float delta = imu.calibrateMagnitudeDelta();
    Serial.print("Delta de magnitud en reposo: ");
    Serial.println(delta, 4);

}

void loop() {
 
    IMUData rawData = imu.readRaw();
    IMUData data = imu.convertToUnits(rawData);

    float magnitude = imu.getAccelerationMagnitude();

    Serial.print("Magnitud: ");
    Serial.println(magnitude, 4); 

    delay(200); // Muestreo cada 200ms
}
