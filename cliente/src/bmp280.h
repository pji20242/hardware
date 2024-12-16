#ifndef BMP280_SENSOR_H
#define BMP280_SENSOR_H

#include <Adafruit_BMP280.h>

/**
 * @brief Classe responsável pela interface com o sensor BMP280.
 */
class BMP280Sensor {
private:
    Adafruit_BMP280 sensorBMP;    // Instância do sensor BMP280
    uint8_t enderecoI2C;          // Endereço I2C do sensor BMP280
    float pressaoReferencia;      // Pressão de referência ao nível do mar (em hPa)

public:
    /**
     * @brief Construtor da classe BMP280Sensor.
     * @param endereco Endereço I2C do sensor BMP280 (padrão: 0x76).
     * @param pressaoNivelMar Pressão de referência ao nível do mar (padrão: 1013.25 hPa).
     */
    BMP280Sensor(uint8_t endereco = 0x76, float pressaoNivelMar = 1013.25);

    /**
     * @brief Inicializa o sensor BMP280.
     * @return true se a inicialização for bem-sucedida, false caso contrário.
     */
    bool iniciar();

    /**
     * @brief Verifica se o sensor está funcionando corretamente.
     * @return true se os dados do sensor forem válidos, false caso contrário.
     */
    bool estaFuncionando();

    /**
     * @brief Lê a temperatura medida pelo sensor.
     * @return Temperatura em graus Celsius.
     */
    inline float lerTemperatura() { return sensorBMP.readTemperature(); }

    /**
     * @brief Lê a pressão medida pelo sensor.
     * @return Pressão em hPa.
     */
    inline float lerPressao() { return sensorBMP.readPressure() / 100.0F; }

    /**
     * @brief Calcula a altitude baseada na pressão medida e na pressão de referência.
     * @return Altitude em metros.
     */
    float lerAltitude();

    /**
     * @brief Atualiza o valor da pressão de referência ao nível do mar.
     * @param novaPressao Novo valor de pressão em hPa.
     */
    inline void configurarPressaoReferencia(float novaPressao) { pressaoReferencia = novaPressao; }
};

#endif
