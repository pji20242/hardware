#include <Arduino.h>
#include "sensor_temp_umid.h"
#include "mqtt.h"
#include "uuid.h"
#include "mqtt_callback.h"
#include "wifi_setup.h"
#include <Wire.h>
#include "bmp280.h"
#include "luminosidade.h"
#include <string>
#include "DS18B20.h"
#include "message.h"
#include "MQ2.h"
#include "MQ7.h"

/*
    DEFINIÇÕES DE CONSTANTES
*/

// Configurações de WiFi
#define SSID "Frederico"
#define PASSWORD "Mari#2606"

// Configurações do MQTT
#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 1883
#define MQTT_Topico_PJI3 "PJI3"

// Configurações de sensores
#define ONE_WIRE_BUS 4 // Pino para o sensor DS18B20
#define PINO_MQ2 32    // GPIO 32 para o sensor MQ2
#define PINO_MQ7 33    // GPIO 33 para o sensor MQ7

// UUID do dispositivo
#define UUID "matheus"

/*
    OBJETOS E VARIÁVEIS GLOBAIS
*/
SensorTempUmid sensor(13, DHT22);    // Sensor DHT22 (Temperatura e Umidade)
MQTT mqtt(MQTT_BROKER, MQTT_PORT);   // Cliente MQTT
String mqttClientId;                 // ID do cliente MQTT
BMP280Sensor bmp280;                 // Sensor BMP280 (Pressão e Altitude)
Luminosidade luminosidade(A15, 3.3); // Sensor LDR de Luminosidade
OneWire oneWire(ONE_WIRE_BUS);       // Comunicação OneWire
DS18B20 temp_ds18b20(&oneWire);      // Sensor DS18B20 (Temperatura)
MQ2 mq2(PINO_MQ2);                   // Sensor MQ2 (Gás inflamável)
MQ7 mq7(PINO_MQ7);                   // Sensor MQ7 (Monóxido de Carbono)

/*
    FUNÇÃO SETUP
*/
void setup()
{
    Serial.begin(115200);
    Serial.println("Iniciando sistema...");

    // Inicializa os sensores
    sensor.begin();       // DHT22
    luminosidade.begin(); // LDR
    temp_ds18b20.begin(); // DS18B20
    mq2.iniciar();        // MQ2
    mq7.iniciar();        // MQ7

    // Configurações de WiFi
    setupWiFi(SSID, PASSWORD);

    // Configurações do MQTT
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId))
    {
        mqtt.subscribe(MQTT_Topico_PJI3);
        Serial.println("Conectado ao broker MQTT!");
    }
    else
    {
        Serial.println("Erro ao conectar ao broker MQTT.");
    }

    // Configurações do BMP280
    Wire.begin();
    if (!bmp280.iniciar())
    {
        Serial.println("Erro ao iniciar o sensor BMP280!");
    }
    else
    {
        Serial.println("Sensor BMP280 iniciado com sucesso!");
    }

    // Calibração inicial dos sensores MQ2 e MQ7
    Serial.println("Calibrando sensores MQ2 e MQ7...");
    mq2.calibrar(10);
    mq7.calibrar(10);
    Serial.println("Calibração concluída.");
}

/*
    FUNÇÃO LOOP
*/
void loop()
{
    // Solicita temperatura do sensor DS18B20
    temp_ds18b20.requestTemperatures();

    // Variável para armazenar o payload MQTT
    std::string payload = UUID;

    // DS18B20 - Temperatura
    if (temp_ds18b20.isConnected())
    {
        float tempDS = temp_ds18b20.getTempC();
        Serial.print("Temperatura DS18B20: ");
        Serial.print(tempDS);
        Serial.println(" °C");

        payload += create_payload(TEMPERATURA, tempDS);
    }
    else
    {
        Serial.println("Erro: Sensor DS18B20 não conectado!");
    }

    // BMP280 - Pressão e Altitude
    if (bmp280.estaFuncionando())
    {
        float pressao = bmp280.lerPressao();
        float altitude = bmp280.lerAltitude();

        Serial.print("Pressão BMP280: ");
        Serial.print(pressao);
        Serial.print(" hPa - Altitude: ");
        Serial.print(altitude);
        Serial.println(" m");

        payload += create_payload(ATMOSFERA, pressao);
    }
    else
    {
        Serial.println("Erro: Sensor BMP280 não está funcionando!");
    }

    // MQ2 - Sensor de Gás Inflamável
    int leituraMQ2 = mq2.lerSensor();
    float concentracaoMQ2 = mq2.obterConcentracaoGas();
    if (leituraMQ2 > 0)
    {
        Serial.print("Leitura MQ2 (Gás Inflamável): ");
        Serial.print(concentracaoMQ2);
        Serial.println(" ppm");

        payload += create_payload(INFLAMAVEL, concentracaoMQ2);
    }

    // MQ7 - Sensor de Monóxido de Carbono (CO)
    int leituraMQ7 = mq7.lerSensor();
    float concentracaoMQ7 = mq7.obterConcentracaoCO();
    if (leituraMQ7 > 0)
    {
        Serial.print("Leitura MQ7 (CO): ");
        Serial.print(concentracaoMQ7);
        Serial.println(" ppm");

        payload += create_payload(MONO_CARBONO, concentracaoMQ7);
    }

    // DHT22 - Umidade
    float umidDHT = sensor.lerUmidade();
    if (!isnan(umidDHT))
    {
        Serial.print("Umidade DHT22: ");
        Serial.print(umidDHT);
        Serial.println(" %");

        payload += create_payload(UMIDADE, umidDHT);
    }

    // LDR - Luminosidade
    float luminosidadePct = luminosidade.calculatePercentage();
    if (luminosidadePct >= 0 && luminosidadePct <= 100)
    {
        Serial.print("Luminosidade: ");
        Serial.print(luminosidadePct);
        Serial.println(" %");

        payload += create_payload(LUMINOSIDADE, luminosidadePct);
    }

    // Publica os dados no broker MQTT
    mqtt.publish(MQTT_Topico_PJI3, payload.c_str());
    Serial.println("Payload publicado no MQTT:");
    Serial.println(payload.c_str());

    // Mantém a conexão MQTT ativa
    mqtt.loop();

    // Intervalo de 5 segundos entre as leituras
    delay(5000);
}
