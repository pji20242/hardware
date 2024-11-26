#include "bmp280.h"

// Construtor: inicializa o endereço e a pressão ao nível do mar
BMP280Sensor::BMP280Sensor(uint8_t endereco, float pressaoNivelMar)
    : enderecoBMP(endereco), pressaoNivelMar(pressaoNivelMar) {}

// Inicializa o sensor BMP280
bool BMP280Sensor::iniciar() {
    return bmp.begin(enderecoBMP); // Retorna verdadeiro se o sensor for encontrado
}

// Verifica se o sensor está funcionando corretamente
bool BMP280Sensor::estaFuncionando() {
    float temperatura = bmp.readTemperature();
    float pressao = bmp.readPressure();
    return !(isnan(temperatura) || isnan(pressao)); // Verifica se os dados são válidos
}

// Lê a temperatura em graus Celsius
float BMP280Sensor::lerTemperatura() {
    return bmp.readTemperature();
}

// Lê a pressão em hPa
float BMP280Sensor::lerPressao() {
    return bmp.readPressure() / 100.0F; // Converte a pressão para hPa
}

// Calcula a altitude com base na pressão ao nível do mar
float BMP280Sensor::lerAltitude() {
    return bmp.readAltitude(pressaoNivelMar);
}

// Atualiza o valor da pressão ao nível do mar
void BMP280Sensor::configurarPressaoNivelMar(float novaPressao) {
    pressaoNivelMar = novaPressao;
}
