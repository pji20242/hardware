#include "MQ7.h"

// Construtor da classe
MQ7::MQ7(int pino) {
  _pino = pino;
  _fatorCalibracao = 1.0;  // Fator de calibração padrão
  _valorBruto = 0;
}

// Método para inicializar o sensor
void MQ7::iniciar(bool iniciarSerial) {
  pinMode(_pino, INPUT);  // Define o pino como entrada
  if (iniciarSerial) {
    Serial.begin(115200);
  }
  Serial.println("Sensor MQ7 iniciado");
}

// Método para ler o valor do sensor (com filtragem simples)
int MQ7::lerSensor() {
  const int numLeituras = 10;  // Número de leituras para filtragem
  long soma = 0;

  for (int i = 0; i < numLeituras; i++) {
    soma += analogRead(_pino);
    delay(10);
  }

  _valorBruto = soma / numLeituras;  // Calcula a média das leituras
  return _valorBruto;
}

// Método para obter a concentração de CO (monóxido de carbono)
float MQ7::obterConcentracaoCO() {
  return _valorBruto * _fatorCalibracao;  // Retorna o valor calibrado
}

// Método para calibrar o sensor
void MQ7::calibrar(int amostras) {
  if (amostras <= 0) {
    Serial.println("Número de amostras inválido. Usando valor padrão (5).");
    amostras = 5;
  }

  long soma = 0;
  for (int i = 0; i < amostras; i++) {
    soma += analogRead(_pino);
    delay(100);
  }

  float media = soma / amostras;
  ajustarFatorCalibracao(ADC_RESOLUCAO / media);
  Serial.print("Fator de calibração: ");
  Serial.println(_fatorCalibracao);
}

// Função privada para ajustar o fator de calibração
void MQ7::ajustarFatorCalibracao(float fator) {
  _fatorCalibracao = fator;
}
