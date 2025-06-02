#ifndef DYNAMICTHRESHOLD_H
#define DYNAMICTHRESHOLD_H


struct DThData {
    float threshold; // Umbral estático
    float dynamicReference; // Referencia dinámica para el umbral
    float alpha; // Factor de suavizado para la referencia dinámica
    float multiplier; // Multiplicador para el umbral dinámico

};

struct CircularBuffer {
    static const uint8_t size = 40;
    float values[size];
    int index = 0;
};

class DynamicThreshold {
    public:

    DynamicThreshold(); // Constructor declarado
    void update(float currentValue); 
    bool isAbove(float currentValue); 
    
    // Métodos para ajustar el umbral dinámico 
    bool setAlpha(float newAlpha);
    void setMultiplier(float newMultiplier);

    //
    void updateBuffer(float mag); 
    float mean();
    float stdDev(float mean);

    private:

    DThData data; // Estructura para almacenar los datos del umbral dinámico
    CircularBuffer buffer; // Buffer circular para almacenar magnitudes

};

#endif
