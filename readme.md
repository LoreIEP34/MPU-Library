# MPU6050 - Libreria modular para IMU de 6 ejes
Librería C++ para interactuar con el sensor MPU6050. Diseñada para ser ligera, sin dependencias externas, y con enfoque en aplicaciones de actividad física.

## instalación
Descargar o clonar en la carpeta `lib/` de tu proyecto PlatformIO.

---
## Funciones disponibles

#### **Funciones de inicializacion del sensor**
`setPins()`: configura los pines de la comunicacion I2C (SDA, SCL)  
`isConnected()`: muestra el estado de conexion del sensor  
`begin()`: inicializa el sensor  

#### **Funciones de lectura y procesamiento de datos crudos**
`readRaw()`: lee los valores crudos del sensor  
`cinvertToUnits()`: convierte los valores crudos a unidades utilizables (G o m/s^2)  
`getAccelerationMagnitude()`: obtiene la magnitud total de la aceleracion  
`calibrateMagnitudeDelta()`: obtiene la "variacion en reposo" (delta) de la magnitud  

#### **Funciones de calibracion de offsets**
`calibrate()`: calcula y configura los offsets automaticamente   
`setOffsets()`: configura los offstes de manera manual  


####  **Funciones de facilitacion para el debug**
`printOffsets()`: imprime el valor de todos los offsets  
`printAcceleration()`: imprime el valor de todos los ejes de la aceleracion  
`printGyroscope()`: imprime el valor de todos los ejes del giroscopio  
`printTemperature()`: imprime el valor de la temperatura del sensor  

#### **Funciones de actualizacion y control del umbral dinamico**
`update()`: actualiza el valor del umbral  
`isAbove()`:  verifica si el umbral fue superado  
`updateBuffer()`: guarda el ultimo valor en el buffer  

#### **Funciones de configuracion del umbral dinamico**
`setDelta()`: configura el valor de variacion del valor en reposo (delta)  
`setAlpha()`: configura el valor de suavizado del umbral (alpha)   
`setMultiplier`: configura el multiplicador base del umbral.  

---
## Ejemplos incluidos

_...estamos trabajando en ello..._

---

## Contribuciones

Se aceptan pull requests. Usar `clang-format` antes de enviar código. Abrir issues si detectás errores o querés sugerencias.

---
## Licencia

MIT. Libre para uso personal o comercial.

---
## Tabla de parametros

|  Parametro  |  Valor por defecto | Descripcion                      |
|-------------|--------------------|----------------------------------|
| `Alpha`     | `0.1f`             | Suavizado del umbral dinamico    |
| `Delta`     | `0.1f`             | variacion en reposo              |
| `threshold` | `0.5f`             | valor del umbral                 |
| `multiplier`| `1.2f`             | escala del umbral                |
| `offsets`   | `0`                | desviacion cuando el sensor lee 0|



