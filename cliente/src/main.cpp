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

#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 1883
#define MQTT_Topico_Temperatura "temperatura"
#define MQTT_Topico_Umidade "umidade"
#define MQTT_Topico_PJI3 "PJI3"
SensorTempUmid sensor(4, DHT22);
MQTT mqtt(MQTT_BROKER, MQTT_PORT);
String mqttClientId;


string generatePayload(const float &ArraySensors[2]){
    String payload = mqttClientId;
    int i
    char l = 'A'
    while(l<=(*(&arr + 1) - arr)) {
        payload += "_" + l + "=" + String(ArraySensors[i]);
        l++;
        i++;
    }
    payload += "@";
}

void setup() {
    Serial.begin(9600);
    sensor.begin();
    Wire.begin();
    setupWiFi(SSID, PASSWORD);

    mqttClientId = gerarUUID();
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId)) {
        //mqtt.subscribe(MQTT_Topico_Temperatura);
        //mqtt.subscribe(MQTT_Topico_Umidade);
        mqtt.subscribe(MQTT_Topico_PJI3);
    }

    
}

void loop() {
    //float temperatura = sensor.lerTemperatura();
    //float umidade = sensor.lerUmidade();
    
    float ArraySensors[2] = {};
    ArraySensors[0] = sensor.lerTemperatura();
    ArraySensors[1] = sensor.lerUmidade();
    

    if (!isnan(temperatura)) {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.print("°C - Umidade: ");
        Serial.print(umidade);
        Serial.println("%");

        generatePayload(ArraySensors)
        //String payloadTemp = "A=" + String(temperatura);
        //String payloadUmid = "B=" + String(umidade);
        //String payload = mqttClientId + "_" + payloadTemp + "_" + payloadUmid + "@";

        //mqtt.publish(MQTT_Topico_Temperatura, payloadTemp.c_str());
        //mqtt.publish(MQTT_Topico_Umidade, payloadUmid.c_str());
        mqtt.publish(MQTT_Topico_PJI3,payload.c_str());
    } else {
        Serial.println("Erro ao ler a temperatura!");
    }

    mqtt.loop();
    delay(2000);
}