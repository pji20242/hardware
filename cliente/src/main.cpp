#include <Arduino.h>
#include "sensor_temp_umid.h"
#include "mqtt.h"
#include "uuid.h"
#include "mqtt_callback.h"
#include "wifi_setup.h"
#include <Wire.h>

// #define SSID "seu_ssid"
// #define PASSWORD "sua_senha"

#define SSID "Frederico"
#define PASSWORD "Mari#2606"

#define MQTT_BROKER "18.231.155.48"
#define MQTT_PORT 1883
#define MQTT_Topico_Temperatura "temperatura"
#define MQTT_Topico_Umidade "umidade"

SensorTempUmid sensor(4, DHT22);
MQTT mqtt(MQTT_BROKER, MQTT_PORT);
String mqttClientId;

void setup() {
    Serial.begin(9600);
    sensor.begin();
    Wire.begin();
    setupWiFi(SSID, PASSWORD);

    mqttClientId = gerarUUID();
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId)) {
        mqtt.subscribe(MQTT_Topico_Temperatura);
        mqtt.subscribe(MQTT_Topico_Umidade);
    }
}

void loop() {
    float temperatura = sensor.lerTemperatura();
    float umidade = sensor.lerUmidade();

    if (!isnan(temperatura)) {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.print("°C - Umidade: ");
        Serial.print(umidade);
        Serial.println("%");

        String payloadTemp = String(temperatura) + " " + mqttClientId;
        String payloadUmid = String(umidade) + " " + mqttClientId;

        mqtt.publish(MQTT_Topico_Temperatura, payloadTemp.c_str());
        mqtt.publish(MQTT_Topico_Umidade, payloadUmid.c_str());
    } else {
        Serial.println("Erro ao ler a temperatura!");
    }

    mqtt.loop();
    delay(2000);
}