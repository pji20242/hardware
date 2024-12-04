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

// #define SSID "seu_ssid"
// #define PASSWORD "sua_senha"

// Definitions for WiFi
#define SSID "Frederico"
#define PASSWORD "Mari#2606"
// Definitions for MQTT
#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 1883
#define MQTT_Topico_Temperatura "temperatura"
#define MQTT_Topico_Umidade "umidade"
#define MQTT_Topico_PJI3 "PJI3"
// Definitions pins for Sensors
#define ONE_WIRE_BUS 4
#define UUID "matheus"

// Global variables and defines
SensorTempUmid sensor(13, DHT22);
MQTT mqtt(MQTT_BROKER, MQTT_PORT);
String mqttClientId;
BMP280Sensor bmp280;
Luminosidade luminosidade(A15, 3.3);
OneWire oneWire(ONE_WIRE_BUS);
DS18B20 temp_ds18b20(&oneWire);

void setup()
{
    Serial.begin(9600);
    sensor.begin();
    luminosidade.begin();
    luminosidade.setAnalogPin(A15);

    temp_ds18b20.begin(); // TODO: Criar metodo para verificar se o sensor está conectado
    //  arbitrary number for the demo
    temp_ds18b20.setOffset(0.25);

    Wire.begin();
    if (!bmp280.iniciar())
    {
        Serial.println("Erro ao iniciar o sensor BMP280!");
    }
    else
    {
        Serial.println("Sensor BMP280 iniciado com sucesso!");
        float pressao = bmp280.lerPressao();
        float altitude = bmp280.lerAltitude();
    }
    // setupWiFi(SSID, PASSWORD);
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId))
    {
        mqtt.subscribe(MQTT_Topico_PJI3);
    }
}

void loop()
{
    //float tempDHT = sensor.lerTemperatura();
    float umidDHT = sensor.lerUmidade();
    temp_ds18b20.requestTemperatures();

    std::string payload = UUID;

    // DS18B20 - Temperatura
    if (temp_ds18b20.isConnected())
    {
        Serial.print("Temp DS18B20: ");
        Serial.print(temp_ds18b20.getTempC());
        Serial.println("°C");
        payload += create_payload(TEMPERATURA, temp_ds18b20.getTempC());
    }

    // BMP280
    if (bmp280.estaFuncionando())
    {
        Serial.print("Pressão: ");
        Serial.print(bmp280.lerPressao());
        Serial.print("hPa - Altitude: ");
        Serial.print(bmp280.lerAltitude());
        Serial.println("m");
        payload += create_payload(ATMOSFERA, bmp280.lerPressao());
    }

    // MQ2

    // MQ7

    // DHT22
    if (!isnan(umidDHT))
    {
        // Serial.print("Temperatura: ");
        // Serial.print(tempDHT);
        Serial.print("°C - Umidade: ");
        Serial.print(umidDHT);
        Serial.println("%");
        payload += create_payload(UMIDADE, umidDHT);
    }

    // LDR - Luminosidade
    // Verifica se o sensor de luminosidade está funcionando corretamente
    if (luminosidade.calculatePercentage() < 100)
    {
    Serial.print("Luminosidade: ");
    Serial.print(luminosidade.calculatePercentage());
    Serial.print("%");
    // Serial.print(" - Tensão: ");
    // Serial.print(luminosidade.readVoltage());
    // Serial.print(" mV - ");
    // Serial.print(" - ReadRaw: ");
    // Serial.print(luminosidade.readRaw());
    // Serial.println("");
    payload += create_payload(LUMINOSIDADE, luminosidade.calculatePercentage());
    }
    // Publish the payload
    mqtt.publish(MQTT_Topico_PJI3, payload.c_str());
    mqtt.loop();
delay(5000);
}