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
#include <WiFiClientSecure.h>

// Definições para WiFi
#define SSID "Frederico"
#define PASSWORD "Mari#2606"

// Definições para MQTT
#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 8080 // Porta padrão para MQTT com TLS/SSL
#define MQTT_Topico_PJI3 "pji3"
#define MQTT_USER "seu_usuario" // Nome de usuário para autenticação no broker
#define MQTT_PASSWORD "sua_senha" // Senha para autenticação no broker

// Definições para o certificado CA
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"SEU_CERTIFICADO_CA_AQUI\n" \
"-----END CERTIFICATE-----\n";

// Definições para Sensores
#define ONE_WIRE_BUS 32
#define UUID "333d564f-7684-4066-948c-f20446a9ccae"

// Variáveis globais
SensorTempUmid sensor(13, DHT22);
WiFiClientSecure espClient; // Usar WiFiClientSecure para TLS/SSL
PubSubClient mqttClient(espClient); // Usar PubSubClient com WiFiClientSecure
String mqttClientId;
BMP280Sensor bmp280;
Luminosidade luminosidade(35, 3.3);
OneWire oneWire(ONE_WIRE_BUS);
DS18B20 temp_ds18b20(&oneWire);

void setupWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi conectado");
}

void setupMQTT() {
    espClient.setCACert(ca_cert); // Configurar o certificado CA
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(callback);

    if (mqttClient.connect(mqttClientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
        Serial.println("Conectado ao broker MQTT com TLS/SSL!");
        mqttClient.subscribe(MQTT_Topico_PJI3);
    } else {
        Serial.println("Falha ao conectar ao broker MQTT!");
    }
}

void setup() {
    Serial.begin(9600);
    sensor.begin();
    luminosidade.begin();
    luminosidade.setAnalogPin(35);

    temp_ds18b20.begin();
    temp_ds18b20.setOffset(0.25);

    Wire.begin();
    if (!bmp280.iniciar()) {
        Serial.println("Erro: Falha ao iniciar o sensor BMP280!");
    } else {
        Serial.println("Sensor BMP280 iniciado com sucesso!");
    }

    setupWiFi(SSID, PASSWORD);
    setupMQTT();
}

void loop() {
    if (!mqttClient.connected()) {
        setupMQTT();
    }

    // Leitura dos sensores e construção do payload
    float umidDHT = sensor.lerUmidade();
    temp_ds18b20.requestTemperatures();

    std::string payload = UUID;

    if (temp_ds18b20.isConnected()) {
        payload += create_payload(TEMPERATURA, temp_ds18b20.getTempC());
    }

    if (bmp280.estaFuncionando()) {
        payload += create_payload(ATMOSFERA, bmp280.lerPressao());
    }

    if (!isnan(umidDHT)) {
        payload += create_payload(UMIDADE, umidDHT);
    }

    if (luminosidade.calculatePercentage() < 100) {
        payload += create_payload(LUMINOSIDADE, luminosidade.calculatePercentage());
    }

    // Publicar o payload
    mqttClient.publish(MQTT_Topico_PJI3, payload.c_str());
    printf("Payload: %s\n", payload.c_str());

    mqttClient.loop();
    delay(60000); // Publicar a cada 60 segundos
}