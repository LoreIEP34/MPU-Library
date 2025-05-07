#include <Wire.h>
#include "MPU6050.h"

// Crear el objeto sensor
MPU6050 sensor();  // default address 0x68
//si quieres usar la dirección 0x69, usa MPU6050 sensor(true); o MPU6050 sensor(0x69);

void setup() {
  Serial.begin(115200);
  while (!Serial);

 // sensor.setPins(21, 22); 
 // Si quieres usar pines específicos, descomenta esta línea y ajusta los números de pines


  // Iniciar el sensor
  if (!sensor.begin()) {
    Serial.println("Error: No se pudo inicializar el MPU6050");
    while (1);  // Detener la ejecución si no se conecta
  }
  Serial.println("MPU6050 inicializado correctamente");
}

void loop() {
  // Aquí podrías agregar código para leer datos del sensor, pero solo vamos a testear la conexión

  delay(1000);  // Retraso de 1 segundo
}
