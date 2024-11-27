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

// #define SSID "seu_ssid"
// #define PASSWORD "sua_senha"

#define SSID "Frederico"
#define PASSWORD "Mari#2606"

#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 1883
#define MQTT_Topico_Temperatura "temperatura"
#define MQTT_Topico_Umidade "umidade"
#define MQTT_Topico_PJI3 "PJI3"
SensorTempUmid sensor(13, DHT22);
MQTT mqtt(MQTT_BROKER, MQTT_PORT);
String mqttClientId;
BMP280Sensor bmp280;
Luminosidade luminosidade(35, 3.3);

// String generatePayload(const float &ArraySensors[2]){
//     String payload = mqttClientId;
//     int i
//     char l = 'A'
//     while(l<=(*(&arr + 1) - arr)) {
//         payload += "_" + l + "=" + String(ArraySensors[i]);
//         l++;
//         i++;
//     }
//     payload += "@";
// }

void setup() {
    Serial.begin(9600);
    sensor.begin();
    luminosidade.begin();
    luminosidade.setAnalogPin(35);

    Wire.begin();
    if (!bmp280.iniciar()) {
        Serial.println("Erro ao iniciar o sensor BMP280!");
    } else {
        Serial.println("Sensor BMP280 iniciado com sucesso!");
        float pressao = bmp280.lerPressao();
        float altitude = bmp280.lerAltitude();
    }
    //setupWiFi(SSID, PASSWORD);

    mqttClientId = gerarUUID();
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId)) {
        //mqtt.subscribe(MQTT_Topico_Temperatura);
        //mqtt.subscribe(MQTT_Topico_Umidade);
        mqtt.subscribe(MQTT_Topico_PJI3);
    }

    
}

void loop() {
    float tempDHT = sensor.lerTemperatura();
    float umidDHT= sensor.lerUmidade();
    
    float ArraySensors[2] = {};
    ArraySensors[0] = sensor.lerTemperatura();
    ArraySensors[1] = sensor.lerUmidade();
    

    if (!isnan(tempDHT)) {
        Serial.print("Temperatura: ");
        Serial.print(tempDHT);
        Serial.print("°C - Umidade: ");
        Serial.print(umidDHT);
        Serial.println("%");


        //BMP280
        Serial.print("Pressão: ");
        Serial.print(bmp280.lerPressao());
        Serial.print("hPa - Altitude: ");
        Serial.print(bmp280.lerAltitude());
        Serial.println("m");


        //LDR - Luminosidade
        Serial.print("Luminosidade: ");
        Serial.print(luminosidade.calculatePercentage());
        Serial.println("%");
        Serial.print("Tensão: ");
        Serial.print(luminosidade.readVoltage());
        Serial.println("mV");
        Serial.println("ReadRaw: ");
        Serial.println(luminosidade.readRaw());


        //generatePayload(ArraySensors)
        //String payloadTemp = "A=" + String(temperatura);
        //String payloadUmid = "B=" + String(umidade);
        //String payload = mqttClientId + "_" + payloadTemp + "_" + payloadUmid + "@";

        //mqtt.publish(MQTT_Topico_Temperatura, payloadTemp.c_str());
        //mqtt.publish(MQTT_Topico_Umidade, payloadUmid.c_str());
        //......mqtt.publish(MQTT_Topico_PJI3,payload.c_str());
    } else {
        Serial.println("Erro ao ler a temperatura!");
    }

    //mqtt.loop();
    delay(2000);
}