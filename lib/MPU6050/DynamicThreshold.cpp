#include <Arduino.h>
#include "DynamicThreshold.h"

// Constructor para inicializar valores por defecto
DynamicThreshold::DynamicThreshold() {
    data.threshold = 0.5f;
    data.dynamicReference = 0.0f;
    data.alpha = 0.1f;
    data.multiplier = 1.2f; // Ajustable
}

void DynamicThreshold::update(float currentValue) {
    data.dynamicReference = data.alpha * currentValue + (1 - data.alpha) * data.dynamicReference;
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


    