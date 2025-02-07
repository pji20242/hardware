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
#include <MQUnifiedsensor.h>
#include <WiFi.h>
#include <PubSubClient.h>

// #define SSID "seu_ssid"
// #define PASSWORD "sua_senha"

// Definitions for WiFi
#define SSID "Frederico"
#define PASSWORD "Mari#2606"
//#define SSID "poco"
//#define PASSWORD "deivid12"

// Definitions for Sensors MQ2 and MQ7
#define Board ("ESP-32") //
#define PINO_MQ2 23    // GPIO 23 para o sensor MQ2
#define PINO_MQ7 26    // GPIO 26 para o sensor MQ7
/***********************Software Related Macros************************************/
//#define         Type                    ("MQ-2") //MQ2
#define Voltage_Resolution (5)
#define ADC_Bit_Resolution (10) // For arduino UNO/MEGA/NANO
#define RatioMQ2CleanAir (9.83) //RS / R0 = 9.83 ppm 

// Definitions for MQTT
#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 1883
#define MQTT_Topico_PJI3 "pji3"

// Definitions pins for Sensors
#define ONE_WIRE_BUS 32
//#define UUID "3170dd2b-f944-4835-8a4a-e2ab5dee3b25"
#define UUID "333d564f-7684-4066-948c-f20446a9ccae"

// Global variables and defines
SensorTempUmid sensor(13, DHT22);
MQTT mqtt(MQTT_BROKER, MQTT_PORT);
String mqttClientId;
BMP280Sensor bmp280;
Luminosidade luminosidade(35, 3.3);
OneWire oneWire(ONE_WIRE_BUS);
DS18B20 temp_ds18b20(&oneWire);

void setup()
{
    Serial.begin(9600);
    sensor.begin();
    luminosidade.begin();
    luminosidade.setAnalogPin(35);

    temp_ds18b20.begin();
    temp_ds18b20.setOffset(0.25);

    Wire.begin();
    if (!bmp280.iniciar())
    {
        Serial.println("Erro: Falha ao iniciar o sensor BMP280!");
    }
    else
    {
        Serial.println("Sensor BMP280 iniciado com sucesso!");
        float pressao = bmp280.lerPressao();
        float altitude = bmp280.lerAltitude();
    }

    setupWiFi(SSID, PASSWORD);

    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId))
    {
        Serial.println("Conectado ao broker MQTT!");
        mqtt.subscribe(MQTT_Topico_PJI3);
    }else{
        Serial.println("Falha ao conectar ao broker MQTT!");
    }
}

void loop()
{


    //float tempDHT = sensor.lerTemperatura();
    float umidDHT = sensor.lerUmidade();
    temp_ds18b20.requestTemperatures();

    std::string payload = UUID;

    /*
        LEITURAS DOS SENSORES
    */

    // Leitura DS18B20
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
        Serial.print("Umidade: ");
        Serial.print(umidDHT);
        Serial.println("%");
        payload += create_payload(UMIDADE, umidDHT);
    } else {
        Serial.println("Erro: Falha ao ler o sensor DHT22!");
    }

    // LDR - Luminosidade
    // Verifica se o sensor de luminosidade está funcionando corretamente
    if (luminosidade.calculatePercentage() < 100)
    {
        Serial.print("Luminosidade: ");
        Serial.print(luminosidade.calculatePercentage());
        Serial.println("%");

        payload += create_payload(LUMINOSIDADE, luminosidade.calculatePercentage());
    }
    // Publish the payload

    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId))
    {
        Serial.println("Conectado ao broker MQTT!");
        mqtt.subscribe(MQTT_Topico_PJI3);
    }else{
        Serial.println("Falha ao conectar ao broker MQTT!");
    }

    //caso o wifi não esteja conectado, tenta reconectar
    if (WiFi.status() != WL_CONNECTED) {
        setupWiFi(SSID, PASSWORD);
    }
    
    
    mqtt.publish(MQTT_Topico_PJI3, payload.c_str());
    printf("Payload: %s\n", payload.c_str());
    mqtt.loop();
Serial.println("");
delay(60000); //
}