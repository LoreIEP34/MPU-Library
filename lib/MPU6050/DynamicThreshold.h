#ifndef DYNAMICTHRESHOLD_H
#define DYNAMICTHRESHOLD_H


struct DThData {
    float threshold; // Umbral estático
    float dynamicReference; // Referencia dinámica para el umbral
    float alpha; // Factor de suavizado para la referencia dinámica
    float multiplier; // Multiplicador para el umbral dinámico

};

class DynamicThreshold {
    public:

    DynamicThreshold(); // Constructor declarado
    void update(float currentValue); 
    bool isAbove(float currentValue); 
    
    // Métodos para ajustar el umbral dinámico 
    void setAlpha(float newAlpha);
    void setMultiplier(float newMultiplier);

    private:

    DThData data; // Estructura para almacenar los datos del umbral dinámico
   
    //DynamicThreshold accelThreshold;
};

#endif
