#include <Arduino.h>
#include <MPU6050.h>
#include <DynamicThreshold.h>

// Instancia del sensor
MPU6050 imu; // sensor MPU6050
DynamicThreshold dth; // Estructura para almacenar datos del umbral dinámico

// Variables para contar pasos
int stepCount = 0;
bool wasAboveThreshold = false;
unsigned long lastStepTime = 0;
const unsigned long stepDelay = 300; // Tiempo mínimo entre pasos (en ms)


void setup() {
    Serial.begin(115200);
    delay(1000);
// ---------------------------     inicia el sensor        ----------------------------
    int sda = 8, scl = 9; // Cambia estos valores si usas pines diferentes
    imu.setPins(sda, scl); // Configura los pines SDA y SCL
    if (!imu.begin()) {
        Serial.println("Error al iniciar MPU6050");
        while (1) delay(1000);
    }
    Serial.println("MPU6050 iniciado correctamente.");

// ---------------------------     calibración del sensor       ----------------------------
    imu.setOffsets(0, 0, 0, 0, 0, 0); // Ajustá si ya tenés calibración
    imu.printOffsets();

    Serial.println("Iniciando calibración de magnitud...");
    float delta = imu.calibrateMagnitudeDelta();
    Serial.print("Delta de magnitud en reposo: ");
    Serial.println(delta, 4);


}

void loop() {
 
// ---------------------------     lectura de datos del sensor       ----------------------------    
    IMUData rawData = imu.readRaw();
    IMUData data = imu.convertToUnits(rawData);
    float magnitude = imu.getAccelerationMagnitude();

// ---------------------------     funcionamiento del umbral dinámico       ----------------------------    
// Actualizar umbral dinámico
    dth.update(magnitude);
    bool isAbove = dth.isAbove(magnitude);

    // Verificación de cruce de umbral (LOW to HIGH) y tiempo mínimo entre pasos
    if (isAbove && !wasAboveThreshold && millis() - lastStepTime > stepDelay) {
        stepCount++;
        lastStepTime = millis();
        Serial.print("Paso detectado. Total: ");
        Serial.println(stepCount);
    }

    // Guardar estado anterior
    wasAboveThreshold = isAbove;

    // ---------------------------     impresión de datos del sensor       ----------------------------
    /*
    Serial.print("Magnitud: ");
    Serial.print(magnitude, 4);
    Serial.print(" | Umbral: ");
    Serial.print(dth.isAbove(magnitude) ? "ALTO" : "normal");
    Serial.print(" | Paso: ");
    Serial.println(stepCount);
    */

    delay(50); // Frecuencia de muestreo: 20 Hz
}

