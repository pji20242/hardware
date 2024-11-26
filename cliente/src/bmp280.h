#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Adafruit_BMP280.h>

class BMP280Sensor {
private:
    Adafruit_BMP280 bmp;       // Instância do sensor BMP280
    uint8_t enderecoBMP;       // Endereço I2C do sensor
    float pressaoNivelMar;     // Pressão ao nível do mar em hPa

public:
    // Construtor: permite definir o endereço do sensor e a pressão ao nível do mar
    BMP280Sensor(uint8_t endereco = 0x76, float pressaoNivelMar = 1013.25);

    // Inicializa o sensor e retorna verdadeiro (true) se bem-sucedido
    bool iniciar();

    // Verifica se o sensor está funcionando corretamente
    bool estaFuncionando();

    // Funções para obter os dados do sensor
    float lerTemperatura();    // Retorna a temperatura em graus Celsius
    float lerPressao();        // Retorna a pressão em hPa
    float lerAltitude();       // Retorna a altitude baseada na pressão ao nível do mar

    // Atualiza o valor da pressão ao nível do mar
    void configurarPressaoNivelMar(float novaPressao);
};

#endif
