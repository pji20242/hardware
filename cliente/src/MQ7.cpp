#include "MQ7.h"
#include <Arduino.h>

/**
 * @brief Construtor da classe MQ7.
 * @param pinoAnalogico Pino analógico onde o sensor está conectado.
 * @param resistenciaCarga Valor da resistência de carga (RL) em ohms.
 */
MQ7::MQ7(int pinoAnalogico, float resistenciaCarga) 
    : pinoAnalogico(pinoAnalogico), resistenciaCarga(resistenciaCarga), resistenciaBase(300.0), valorBruto(0) {}

/**
 * @brief Configura o ADC para realizar leituras no pino do sensor.
 */
void MQ7::iniciar() {
    // Configura resolução de 12 bits para o ADC
    adc1_config_width(ADC_WIDTH_BIT_12);

    // Configura a atenuação do ADC para o pino específico (0-3.3V)
    adc1_config_channel_atten((adc1_channel_t)pinoAnalogico, ADC_ATTEN_DB_11);

    Serial.println("Sensor MQ7 inicializado!");
}

/**
 * @brief Realiza múltiplas leituras do sensor e calcula a média para maior precisão.
 * @return Valor médio lido pelo ADC.
 */
int MQ7::lerSensor() {
    long soma = 0;
    const int numeroLeituras = 10;

    // Realiza múltiplas leituras para reduzir ruídos
    for (int i = 0; i < numeroLeituras; i++) {
        soma += adc1_get_raw((adc1_channel_t)pinoAnalogico);
        delay(100);
    }

    // Calcula o valor médio
    valorBruto = soma / numeroLeituras;
    return valorBruto;
}

/**
 * @brief Calcula a concentração de monóxido de carbono (CO) em ppm.
 * 
 * A concentração é calculada a partir da resistência do sensor (RS) e da curva característica do MQ7.
 * @return Concentração de CO em ppm.
 */
float MQ7::obterConcentracaoCO() {
    // Converte valor bruto do ADC para tensão (V)
    float tensao = (valorBruto / ADC_RESOLUCAO) * TENSAO_REFERENCIA;

    // Calcula a resistência do sensor (RS)
    float resistenciaSensor = (resistenciaCarga * (TENSAO_REFERENCIA - tensao)) / tensao;

    // Calcula o "ratio" entre RS e R0
    float ratio = resistenciaSensor / resistenciaBase;

    // Evita valores negativos ou zero
    if (ratio <= 0) return 0.0;

    // Aplica a equação da curva característica do MQ7
    return pow(10, (log10(ratio) - coeficienteB) / coeficienteM);
}

/**
 * @brief Realiza a calibração do sensor, ajustando o valor de R0 em ar limpo.
 * @param amostras Número de amostras para média durante a calibração.
 */
void MQ7::calibrar(int amostras) {
    long soma = 0;

    // Coleta várias leituras para obter uma média
    for (int i = 0; i < amostras; i++) {
        soma += adc1_get_raw((adc1_channel_t)pinoAnalogico);
        delay(100);
    }

    // Calcula o valor médio lido pelo ADC
    float mediaADC = soma / amostras;

    // Converte para tensão (V)
    float tensao = (mediaADC / ADC_RESOLUCAO) * TENSAO_REFERENCIA;

    // Calcula a resistência do sensor (RS)
    float resistenciaSensor = (resistenciaCarga * (TENSAO_REFERENCIA - tensao)) / tensao;

    // Ajusta o valor de R0 assumindo um "ratio" padrão de 27.5 em ar limpo
    resistenciaBase = resistenciaSensor / 27.5;

    Serial.print("Calibração concluída. R0 = ");
    Serial.println(resistenciaBase);
}
