#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include "sensor_temp_umid.h"
#include "uuid.h"
#include "mqtt_callback.h"
#include "wifi_setup.h"
#include <Wire.h>
#include "bmp280.h"
#include "luminosidade.h"
#include <string>
#include "DS18B20.h"
#include "message.h"

// Definições para Wi-Fi
#define SSID "Frederico"
#define PASSWORD "Mari#2606"

// Definições para MQTT
#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 8080
#define MQTT_TOPIC_PJI3 "PJI3"

// Definição do UUID
#define UUID "matheus"

// Definição do certificado da CA em formato PEM
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIID...restante_do_certificado...\n" \
"-----END CERTIFICATE-----\n";

// Variáveis globais e definições
SensorTempUmid sensor(13, DHT22);
WiFiClientSecure net;
MQTTClient client;
String mqttClientId;
BMP280Sensor bmp280;
Luminosidade luminosidade(A15, 3.3);
OneWire oneWire(4);
DS18B20 temp_ds18b20(&oneWire);

void connect() {
  Serial.print("Verificando conexão Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nConectando ao broker MQTT...");
  while (!client.connect(mqttClientId.c_str(), "usuario", "senha")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConectado!");

  client.subscribe(MQTT_TOPIC_PJI3);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Mensagem recebida: " + topic + " - " + payload);
}

void setup() {
  Serial.begin(9600);
  sensor.begin();
  luminosidade.begin();
  luminosidade.setAnalogPin(A15);
  temp_ds18b20.begin();
  temp_ds18b20.setOffset(0.25);

  Wire.begin();
  if (!bmp280.iniciar()) {
    Serial.println("Erro: Falha ao iniciar o sensor BMP280!");
  } else {
    Serial.println("Sensor BMP280 iniciado com sucesso!");
  }

  WiFi.begin(SSID, PASSWORD);
  net.setCACert(ca_cert);
  client.begin(MQTT_BROKER, MQTT_PORT, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  float umidDHT = sensor.lerUmidade();
  temp_ds18b20.requestTemperatures();

  std::string payload = UUID;

  if (temp_ds18b20.isConnected()) {
    Serial.print("Temp DS18B20: ");
    Serial.print(temp_ds18b20.getTempC());
    Serial.println("°C");
    payload += create_payload(TEMPERATURA, temp_ds18b20.getTempC());
  }

  if (bmp280.estaFuncionando()) {
    Serial.print("Pressão: ");
    Serial.print(bmp280.lerPressao());
    Serial.print("hPa - Altitude: ");
    Serial.print(bmp280.lerAltitude());
    Serial.println("m");
    payload += create_payload(ATMOSFERA, bmp280.lerPressao());
  }

  if (!isnan(umidDHT)) {
    Serial.print("Umidade: ");
    Serial.print(umidDHT);
    Serial.println("%");
    payload += create_payload(UMIDADE, umidDHT);
  }

  if (luminosidade.calculatePercentage() < 100) {
    Serial.print("Luminosidade: ");
    Serial.print(luminosidade.calculatePercentage());
    Serial.println("%");
    payload += create_payload(LUMINOSIDADE, luminosidade.calculatePercentage());
  }

  client.publish(MQTT_TOPIC_PJI3, payload.c_str());
  delay(5000);
}
