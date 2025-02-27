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
#include <WiFi.h>
#include <PubSubClient.h>
#include "mq.h"

// Definitions for WiFi
#define SSID "seu_ssid"
#define PASSWORD "sua_senha"

// Definitions for MQTT
#define MQTT_BROKER "vm0.pji3.sj.ifsc.edu.br"
#define MQTT_PORT 1883
#define MQTT_Topico_PJI3 "pji3"
#define MQTT_USER "device" // Nome de usuário para autenticação no broker
#define MQTT_PASSWORD "teste" // Senha para autenticação no broker

// Definitions pins for Sensors
#define ONE_WIRE_BUS 32

//UUID 
#define UUID "333d564f-7684-4066-948c-f20446a9ccae"

// Global variables and defines
SensorTempUmid sensor(13, DHT22);
MQTT mqtt(MQTT_BROKER, MQTT_PORT);
String mqttClientId;
BMP280Sensor bmp280;
Luminosidade luminosidade(35, 3.3);
OneWire oneWire(ONE_WIRE_BUS);
DS18B20 temp_ds18b20(&oneWire);

struct leituras_t
{
  int MQ_2;  //300-10000
  int MQ_7; //10-10000
};

leituras_t leitura;

void setup()
{
    Serial.begin(9600);
    //inicia DHT22
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
    //-----------MQs------------------------
    Serial.print("MQ2: Calibrating...\n");                
    RoMQ2 = MQCalibration(MQ2PIN); //Calibrating the sensor. Please make sure the sensor is in clean air 
                //when you perform the calibration                    
    Serial.print("MQ2: Calibration is done...\n"); 
    Serial.print("RoMQ2=");
    Serial.print(RoMQ2);
    Serial.print("kohm");
    Serial.print("\n");

    Serial.print("MQ7: Calibrating...\n");                
    RoMQ7 = MQCalibration(MQ7PIN); //Calibrating the sensor. Please make sure the sensor is in clean air 
                //when you perform the calibration                    
    Serial.print("MQ7: Calibration is done...\n"); 
    Serial.print("RoMQ7=");
    Serial.print(RoMQ7);
    Serial.print("kohm");
    Serial.print("\n");

    setupWiFi(SSID, PASSWORD);
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
        Serial.println("Conectado ao broke MQTT!");
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
    if (luminosidade.calculatePercentage() < 100)
    {
        Serial.print("Luminosidade: ");
        Serial.print(luminosidade.calculatePercentage());
        Serial.println("%");

        payload += create_payload(LUMINOSIDADE, luminosidade.calculatePercentage());
    }

    leitura.MQ_2 = MQ2GetGasPercentage(MQRead(MQ2PIN)/RoMQ2,GAS_SMOKE);
    payload += create_payload(FUMACA, leitura.MQ_2);
    leitura.MQ_7 = MQ7GetGasPercentage(MQRead(MQ7PIN)/RoMQ7,GAS_CARBON_MONOXIDE);
    payload += create_payload(MONO_CARBONO, leitura.MQ_7);
    Serial.print("Smoke Value = ");
    Serial.println(leitura.MQ_2);
    Serial.print("CO Value = "); 
    Serial.println(leitura.MQ_7); 


    //Publica o payload
    mqtt.setCallback(callback);
    if (mqtt.connect(mqttClientId.c_str(), MQTT_USER, MQTT_PASSWORD))
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
delay(900000); // 15 minutos
//delay(10000); //
}