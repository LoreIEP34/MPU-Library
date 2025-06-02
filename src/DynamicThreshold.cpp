#include <Arduino.h>
#include "DynamicThreshold.h"

// Constructor para inicializar valores por defecto
DynamicThreshold::DynamicThreshold() {
    data.threshold = 0.5f;
    data.dynamicReference = -1.0f;
    data.alpha = 0.1f;
    data.multiplier = 1.2f; // Ajustable
}

void DynamicThreshold::update(float currentValue) {
    if (data.dynamicReference < 0) {
        // Inicializar la referencia dinámica con el primer valor
        data.dynamicReference = currentValue;
    } else {
    data.dynamicReference = data.alpha * currentValue + (1 - data.alpha) * data.dynamicReference;
    }
    data.threshold = data.dynamicReference * data.multiplier; // Ajustable
}

bool DynamicThreshold::isAbove(float currentValue) {
    return currentValue > data.threshold;
}


bool DynamicThreshold::setAlpha(float newAlpha) {
    if (newAlpha >= 0.0f && newAlpha <= 1.0f) {
        data.alpha = newAlpha;
        return true;
    }
    return false;
}

void DynamicThreshold::setMultiplier(float newMultiplier) {
    if (newMultiplier > 0.0f) {
        data.multiplier = newMultiplier;
    }
}

void DynamicThreshold::updateBuffer(float mag) {
    buffer.values[buffer.index] = mag;
    buffer.index = (buffer.index + 1) % buffer.size; // Circular buffer
}

float DynamicThreshold::mean() {
    float sum = 0;
    for (int i = 0; i < buffer.size; i++) {
        sum += buffer.values[i];
    }
    return sum / buffer.size;
}

float DynamicThreshold::stdDev(float mean) {
    float sum = 0;
    for (int i = 0; i < buffer.size; i++) {
         float diff = buffer.values[i] - mean;
        sum += diff * diff;
    }
    return sqrt(sum / buffer.size);
}
    