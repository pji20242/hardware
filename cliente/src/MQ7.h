#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>

#define ADC_RESOLUCAO 1023.0  // Resolução do ADC

class MQ7 {
  public:
    // Construtor da classe
    MQ7(int pino);

    // Métodos públicos
    void iniciar(bool iniciarSerial = true);
    int lerSensor();
    float obterConcentracaoCO();
    void calibrar(int amostras = 5);

  private:
    int _pino;  // Pino de leitura analógica
    int _valorBruto;  // Valor bruto lido do sensor
    float _fatorCalibracao;  // Fator de calibração para o sensor

    // Função para aplicar calibração
    void ajustarFatorCalibracao(float fator);
};

#endif
