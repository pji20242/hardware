#include "luminosidade.h"

// Construtor
Luminosidade::Luminosidade(int pin, int refVoltage) : analogPin(pin), referenceVoltage(refVoltage) {}

// Inicializa o sensor
void Luminosidade::begin() {
    pinMode(analogPin, INPUT);
}

// Lê o valor analógico bruto
int Luminosidade::readRaw() {
    return analogRead(analogPin);
}

// Converte o valor analógico para tensão
float Luminosidade::readVoltage() {
    int rawValue = readRaw();
    return (float(rawValue) / 4095.0) * referenceVoltage;
}

// Calcula a porcentagem de luminosidade
float Luminosidade::calculatePercentage() {
    int rawValue = readRaw();
    return (float(rawValue) / 4095.0) * 100.0; // Normalizado para %
}

// Define o pino analógico
void Luminosidade::setAnalogPin(int pin) {
    analogPin = pin;
}

// Define a tensão de referência
void Luminosidade::setReferenceVoltage(int refVoltage) {
    referenceVoltage = refVoltage;
}
