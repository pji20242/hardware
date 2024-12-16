#ifndef MQ7_H
#define MQ7_H

#include <driver/adc.h>

// Resolução do conversor analógico-digital (12 bits)
#define ADC_RESOLUCAO 4095.0 

// Tensão de referência para o ADC (em Volts)
#define TENSAO_REFERENCIA 3.3

/**
 * @brief Classe para leitura e cálculo de concentração de monóxido de carbono (CO) usando o sensor MQ7.
 */
class MQ7 {
public:
    /**
     * @brief Construtor da classe MQ7.
     * @param pinoAnalogico Pino analógico onde o sensor está conectado.
     * @param resistenciaCarga Valor da resistência de carga (em ohms).
     */
    MQ7(int pinoAnalogico, float resistenciaCarga = 300.0);

    /**
     * @brief Configura o ADC para leitura do sensor.
     */
    void iniciar();

    /**
     * @brief Realiza a leitura do valor bruto do sensor (média de várias leituras).
     * @return Valor médio lido pelo ADC.
     */
    int lerSensor();

    /**
     * @brief Calcula a concentração de monóxido de carbono (CO) em ppm (partes por milhão).
     * @return Concentração de CO em ppm.
     */
    float obterConcentracaoCO();

    /**
     * @brief Realiza a calibração do sensor, ajustando o valor de resistência-base (R0).
     * @param amostras Número de amostras para calibração.
     */
    void calibrar(int amostras = 10);

private:
    int pinoAnalogico;      // Pino analógico conectado ao sensor
    float resistenciaCarga; // Valor da resistência de carga (RL) em ohms
    float resistenciaBase;  // Resistência de referência (R0) calibrada
    int valorBruto;         // Valor bruto lido pelo ADC

    // Coeficientes da curva característica do sensor MQ7
    const float coeficienteM = -0.77; // Inclinação da curva
    const float coeficienteB = 1.55;  // Interceptação da curva
};

#endif
