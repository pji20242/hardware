#include "MQ2.h"
#include <Arduino.h>

/**
 * @brief Construtor da classe MQ2.
 * @param pinoAnalogico Pino analógico onde o sensor está conectado.
 * @param resistenciaCarga Valor da resistência de carga (RL) em ohms.
 */
MQ2::MQ2(int pinoAnalogico, float resistenciaCarga)
    : pinoAnalogico(pinoAnalogico), resistenciaCarga(resistenciaCarga), resistenciaBase(10.0), valorBruto(0) {}

/**
 * @brief Configura o ADC para realizar leituras no pino do sensor.
 */
void MQ2::iniciar() {
    // Configura a resolução de 12 bits do ADC
    adc1_config_width(ADC_WIDTH_BIT_12);

    // Configura atenuação do ADC para leituras de 0 a 3.3V
    adc1_config_channel_atten((adc1_channel_t)pinoAnalogico, ADC_ATTEN_DB_11);

    Serial.println("Sensor MQ2 inicializado!");
}

/**
 * @brief Realiza várias leituras do sensor e calcula a média para reduzir ruídos.
 * @return Valor médio lido pelo ADC.
 */
int MQ2::lerSensor() {
    long soma = 0;
    const int numeroLeituras = 10;

    // Realiza múltiplas leituras para maior precisão
    for (int i = 0; i < numeroLeituras; i++) {
        soma += adc1_get_raw((adc1_channel_t)pinoAnalogico);
        delay(50); // Pequeno atraso entre leituras
    }

    // Calcula a média das leituras
    valorBruto = soma / numeroLeituras;
    return valorBruto;
}

/**
 * @brief Calcula a resistência do sensor (RS) com base na leitura do ADC.
 * 
 * A resistência é calculada usando a fórmula:  
 * RS = RL * (Vref - Vsensor) / Vsensor
 * 
 * @return Resistência RS em ohms.
 */
float MQ2::obterResistenciaSensor() {
    // Converte o valor bruto do ADC em tensão
    float tensao = (valorBruto / ADC_RESOLUCAO) * TENSAO_REFERENCIA;

    // Calcula a resistência RS do sensor
    float resistenciaSensor = (resistenciaCarga * (TENSAO_REFERENCIA - tensao)) / tensao;
    return resistenciaSensor;
}

/**
 * @brief Calcula a concentração de gás inflamável em PPM.
 * 
 * A concentração é calculada usando a curva característica do sensor MQ2:  
 * ppm = 10 ^ ((log10(RS/R0) - B) / M)
 * 
 * @return Concentração de gás em PPM.
 */
float MQ2::obterPPM() {
    // Calcula a resistência do sensor RS
    float resistenciaSensor = obterResistenciaSensor();

    // Calcula o "ratio" entre RS e R0
    float ratio = resistenciaSensor / resistenciaBase;

    // Evita valores inválidos de ratio
    if (ratio <= 0) return 0.0;

    // Aplica a equação logarítmica da curva característica
    float ppm = pow(10, (log10(ratio) - coeficienteB) / coeficienteM);
    return ppm;
}

/**
 * @brief Calibra o sensor para determinar a resistência-base (R0) em ar limpo.
 * 
 * Durante a calibração, o sensor coleta múltiplas leituras e ajusta o valor de R0.  
 * R0 é calculado como RS / 9.83, onde 9.83 é o valor típico do ratio em ar limpo.
 * 
 * @param amostras Número de leituras para a calibração.
 */
void MQ2::calibrar(int amostras) {
    long soma = 0;

    // Realiza múltiplas leituras do ADC para calcular a média
    for (int i = 0; i < amostras; i++) {
        soma += adc1_get_raw((adc1_channel_t)pinoAnalogico);
        delay(100); // Pequeno atraso entre leituras
    }

    // Calcula a tensão média do ADC
    float mediaADC = soma / amostras;
    float tensao = (mediaADC / ADC_RESOLUCAO) * TENSAO_REFERENCIA;

    // Calcula a resistência do sensor em ar limpo (RS)
    float resistenciaSensor = (resistenciaCarga * (TENSAO_REFERENCIA - tensao)) / tensao;

    // Ajusta R0 usando o ratio típico de 9.83 em ar limpo
    resistenciaBase = resistenciaSensor / 9.83;

    // Exibe o valor de R0 calibrado
    Serial.print("Calibração concluída. R0 = ");
    Serial.println(resistenciaBase);
}
