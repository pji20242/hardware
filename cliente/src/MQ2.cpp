#include "MQ2.h"

// Construtor da classe
MQ2::MQ2(int pino) {
  _pino = pino;
  _fatorCalibracao = 1.0;  // Fator de calibração padrão
  _valorBruto = 0;
}

// Método para inicializar o sensor
void MQ2::iniciar(bool iniciarSerial) {
  pinMode(_pino, INPUT);  // Define o pino como entrada
  if (iniciarSerial) {
    Serial.begin(115200);  // Inicia a comunicação serial
  }
  Serial.println("Sensor MQ2 iniciado");
}

// Método para ler o valor do sensor com filtragem simples
int MQ2::lerSensor() {
  const int numLeituras = 10;  // Número de leituras para filtragem
  long soma = 0;

  for (int i = 0; i < numLeituras; i++) {
    soma += analogRead(_pino);
    delay(10);  // Pequeno atraso para evitar leituras muito rápidas
  }

  _valorBruto = soma / numLeituras;  // Calcula a média das leituras
  return _valorBruto;
}

// Método para obter a concentração de gás (valor normalizado)
float MQ2::obterConcentracaoGas() {
  return _valorBruto * _fatorCalibracao;  // Retorna o valor calibrado
}

// Método para calibrar o sensor
void MQ2::calibrar(int amostras) {
  if (amostras <= 0) {
    Serial.println("Número de amostras inválido. Usando valor padrão (5).");
    amostras = 5;
  }

  long soma = 0;
  for (int i = 0; i < amostras; i++) {
    soma += analogRead(_pino);  // Soma os valores lidos do sensor
    delay(100);  // Atraso de 100ms entre as leituras
  }

  float media = soma / amostras;  // Calcula a média dos valores lidos
  ajustarFatorCalibracao(ADC_RESOLUCAO / media);  // Ajusta o fator de calibração com base na média
  Serial.print("Fator de calibração: ");
  Serial.println(_fatorCalibracao);
}

// Função privada para ajustar o fator de calibração
void MQ2::ajustarFatorCalibracao(float fator) {
  _fatorCalibracao = fator;
}
