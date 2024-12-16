#ifndef MQ2_H
#define MQ2_H

#include <driver/adc.h>
#include <cmath>

// Resolução do ADC de 12 bits (0 - 4095)
#define ADC_RESOLUCAO 4095.0

// Tensão de referência da ESP32 (3.3V)
#define TENSAO_REFERENCIA 3.3

/**
 * @brief Classe para leitura e cálculo da concentração de gases inflamáveis utilizando o sensor MQ2.
 */
class MQ2 {
public:
    /**
     * @brief Construtor da classe MQ2.
     * @param pinoAnalogico Pino analógico onde o sensor está conectado.
     * @param resistenciaCarga Valor da resistência de carga (RL) em ohms.
     */
    MQ2(int pinoAnalogico, float resistenciaCarga = 300.0);

    /**
     * @brief Configura o ADC para leitura do sensor.
     */
    void iniciar();

    /**
     * @brief Realiza a leitura média do valor bruto do ADC.
     * @return Valor médio lido pelo ADC.
     */
    int lerSensor();

    /**
     * @brief Calcula a resistência do sensor (RS) com base na leitura do ADC.
     * @return Resistência RS em ohms.
     */
    float obterResistenciaSensor();

    /**
     * @brief Calcula a concentração de gás em PPM usando a curva característica do MQ2.
     * @return Concentração em PPM.
     */
    float obterPPM();

    /**
     * @brief Calibra o sensor para determinar o valor de referência (R0) em ar limpo.
     * @param amostras Número de amostras para a calibração.
     */
    void calibrar(int amostras = 10);

private:
    int pinoAnalogico;        // Pino analógico conectado ao sensor
    float resistenciaCarga;   // Resistência RL (resistor de carga) em ohms
    float resistenciaBase;    // Resistência de referência (R0) calibrada
    int valorBruto;           // Valor bruto lido do ADC

    // Parâmetros da curva logarítmica do sensor MQ2
    const float coeficienteM = -0.47; // Inclinação da curva
    const float coeficienteB = 1.72;  // Intercepto da curva
};

#endif
