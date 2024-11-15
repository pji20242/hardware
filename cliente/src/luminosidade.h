#ifndef LUMINOSIDADE_H
#define LUMINOSIDADE_H

#include <Arduino.h>

class Luminosidade {
private:
    int analogPin; // Pino analógico onde o sensor está conectado
    int referenceVoltage; // Tensão de referência do sistema (em mV)

public:
    // Construtor
    Luminosidade(int pin, int refVoltage = 3300);

    // Inicializa o sensor
    void begin();

    // Lê o valor analógico bruto (0-4095 para ESP32)
    int readRaw();

    // Converte o valor lido em tensão (mV)
    float readVoltage();

    // Calcula a porcentagem de luminosidade (0 a 100%)
    float calculatePercentage();

    // Define o pino analógico
    void setAnalogPin(int pin);

    // Define a tensão de referência
    void setReferenceVoltage(int refVoltage);
};

#endif
