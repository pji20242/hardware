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
#define MQTT_TOPIC "PJI3"

// Configurações de sensores
#define ONE_WIRE_BUS 4 // Pino para o sensor DS18B20
#define PINO_MQ2 32    // GPIO 32 para o sensor MQ2
#define PINO_MQ7 33    // GPIO 33 para o sensor MQ7
#define RL 300         // Valor do RL no circuito (300 ohms)

// Intervalos de leituras
#define INTERVALO_LEITURA 10000 // 10 segundos

// UUID do dispositivo
#define UUID "matheus"

/*
    OBJETOS E VARIÁVEIS GLOBAIS
*/
SensorTempUmid sensorDHT22(13, DHT22);    // Sensor DHT22 (Temperatura e Umidade)
MQTT mqtt(MQTT_BROKER, MQTT_PORT);        // Cliente MQTT
String mqttClientId;                      // ID do cliente MQTT
BMP280Sensor bmp280;                      // Sensor BMP280 (Pressão e Altitude)
Luminosidade sensorLuminosidade(A15, 3.3); // Sensor LDR de Luminosidade
OneWire oneWire(ONE_WIRE_BUS);            // Comunicação OneWire
DS18B20 sensorDS18B20(&oneWire);          // Sensor DS18B20 (Temperatura)
MQ2 sensorMQ2(PINO_MQ2, RL);              // Sensor MQ2 (Gás inflamável)
MQ7 sensorMQ7(PINO_MQ7);                  // Sensor MQ7 (Monóxido de Carbono)

/*
    FUNÇÃO SETUP
*/
void setup()
{
    Serial.begin(115200);
    Serial.println("Iniciando sistema...");

    // Inicializa os sensores
    sensorDHT22.begin();
    sensorLuminosidade.begin();
    sensorDS18B20.begin();
    sensorMQ2.iniciar();
    sensorMQ2.calibrar();
    sensorMQ7.iniciar();
    sensorMQ7.calibrar();

    // Configuração do WiFi
    setupWiFi(SSID, PASSWORD);

    // Configuração do MQTT
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId))
    {
        mqtt.subscribe(MQTT_TOPIC);
        Serial.println("Conectado ao broker MQTT!");
    }
    else
    {
        Serial.println("Erro: Não foi possível conectar ao broker MQTT.");
    }

    // Configuração do BMP280
    Wire.begin();
    if (!bmp280.iniciar())
    {
        Serial.println("Erro: Falha ao iniciar o sensor BMP280!");
    }
    else
    {
        Serial.println("Sensor BMP280 inicializado com sucesso!");
    }
}

/*
    FUNÇÃO LOOP
*/
void loop()
{
    // Solicita temperatura do sensor DS18B20
    sensorDS18B20.requestTemperatures();

    // Inicializa o payload MQTT
    std::string payload = UUID;

    /*
        LEITURAS DOS SENSORES
    */

    // Leitura DS18B20
    if (sensorDS18B20.isConnected())
    {
        float temperaturaDS = sensorDS18B20.getTempC();
        Serial.printf("Temperatura DS18B20: %.2f °C\n", temperaturaDS);
        payload += create_payload(TEMPERATURA, temperaturaDS);
    }
    else
    {
        Serial.println("Erro: Sensor DS18B20 desconectado.");
    }

    // Leitura BMP280
    if (bmp280.estaFuncionando())
    {
        float pressao = bmp280.lerPressao();
        float altitude = bmp280.lerAltitude();
        Serial.printf("Pressão BMP280: %.2f hPa - Altitude: %.2f m\n", pressao, altitude);
        payload += create_payload(ATMOSFERA, pressao);
    }
    else
    {
        Serial.println("Erro: Falha ao ler dados do sensor BMP280.");
    }

    // Leitura MQ2
    if (sensorMQ2.lerSensor() >= 0)
    {
        float ppmMQ2 = sensorMQ2.obterPPM();
        Serial.printf("Concentração GLP MQ2: %.2f ppm\n", ppmMQ2);
        payload += create_payload(INFLAMAVEL, ppmMQ2);
    }

    // Leitura MQ7
    int leituraMQ7 = sensorMQ7.lerSensor();
    if (leituraMQ7 >= 0)
    {
        float concentracaoCO = sensorMQ7.obterConcentracaoCO();
        Serial.printf("Concentração de CO MQ7: %.2f ppm\n", concentracaoCO);
        payload += create_payload(MONO_CARBONO, concentracaoCO);
    }

    // Leitura DHT22
    float umidade = sensorDHT22.lerUmidade();
    if (!isnan(umidade))
    {
        Serial.printf("Umidade DHT22: %.2f %%\n", umidade);
        payload += create_payload(UMIDADE, umidade);
    }

    // Leitura LDR
    float luminosidade = sensorLuminosidade.calculatePercentage();
    if (luminosidade >= 0 && luminosidade <= 100)
    {
        Serial.printf("Luminosidade: %.2f %%\n", luminosidade);
        payload += create_payload(LUMINOSIDADE, luminosidade);
    }

    /*
        ENVIO DOS DADOS VIA MQTT
    */
    mqtt.publish(MQTT_TOPIC, payload.c_str());
    Serial.println("Payload enviado ao broker MQTT:");
    Serial.println(payload.c_str());

    // Mantém a conexão MQTT ativa
    mqtt.loop();

    // Intervalo entre as leituras
    delay(INTERVALO_LEITURA);
}
