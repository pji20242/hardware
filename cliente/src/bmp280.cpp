#include "bmp280.h"

/**
 * @brief Construtor da classe BMP280Sensor.
 * @param endereco Endereço I2C do sensor BMP280.
 * @param pressaoNivelMar Pressão de referência ao nível do mar.
 */
BMP280Sensor::BMP280Sensor(uint8_t endereco, float pressaoNivelMar)
    : enderecoI2C(endereco), pressaoReferencia(pressaoNivelMar) {}

/**
 * @brief Inicializa o sensor BMP280.
 * @return true se o sensor for detectado e inicializado corretamente.
 */
bool BMP280Sensor::iniciar() {
    return sensorBMP.begin(enderecoI2C);
}

/**
 * @brief Verifica se o sensor BMP280 está retornando valores válidos.
 * @return true se os dados forem válidos, false caso contrário.
 */
bool BMP280Sensor::estaFuncionando() {
    float temperatura = sensorBMP.readTemperature();
    float pressao = sensorBMP.readPressure();
    return !(isnan(temperatura) || isnan(pressao)); // Verifica se os valores são válidos
}

/**
 * @brief Calcula a altitude com base na pressão medida e pressão de referência.
 * @return Altitude em metros.
 */
float BMP280Sensor::lerAltitude() {
    return sensorBMP.readAltitude(pressaoReferencia);
}
