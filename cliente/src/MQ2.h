#ifndef MQ2_H
#define MQ2_H

#include <Arduino.h>

#define ADC_RESOLUCAO 1023.0  // Resolução do ADC, padrão para Arduino com ADC de 10 bits

class MQ2 {
  public:
    // Construtor da classe
    MQ2(int pino);

    // Métodos públicos
    void iniciar(bool iniciarSerial = true);
    int lerSensor();
    float obterConcentracaoGas();
    void calibrar(int amostras = 5);

  private:
    int _pino;  // Pino de leitura analógica
    int _valorBruto;  // Valor bruto lido do sensor
    float _fatorCalibracao;  // Fator de calibração para o sensor

    // Função para aplicar calibração
    void ajustarFatorCalibracao(float fator);
};

#endif
