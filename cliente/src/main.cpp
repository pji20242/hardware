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
//#define SSID "Frederico"
//#define PASSWORD "Mari#2606"
#define SSID "poco"
#define PASSWORD "deivid12"

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

/************************mq2sensor************************************/
/************************Hardware Related Macros************************************/
#define         MQ2PIN                       (36)     //define which analog input channel you are going to use
#define         RL_VALUE_MQ2                 (1)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR_MQ2      (9.577)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
               //which is derived from the chart in datasheet

/************************MQ7sensor************************************/
/************************Hardware Related Macros************************************/
#define         MQ7PIN                       (39)      //define which analog input channel you are going to use
#define         RL_VALUE_MQ7                 (1)      //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR_MQ7      (26.09)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
              //which is derived from the chart in datasheet

/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
               //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
               //normal operation

/**********************Application Related Macros**********************************/
#define         GAS_HYDROGEN                  (0)
#define         GAS_LPG                       (1)
#define         GAS_METHANE                   (2)
#define         GAS_CARBON_MONOXIDE           (3)
#define         GAS_ALCOHOL                   (4)
#define         GAS_SMOKE                     (5)
#define         GAS_PROPANE                   (6)
#define         accuracy                      (0)   //for linearcurves

/*****************************Globals************************************************/
float           RoMQ2 = 0;                            //Ro is initialized to 10 kilo ohms
float           RoMQ7 = 0;                            //Ro is initialized to 10 kilo ohms

struct leituras_t
{
  int MQ_2;  //300-10000
  int MQ_7; //10-10000
};

leituras_t leitura;

float MQResistanceCalculation(int raw_adc, int mq_pin)
{
  if (mq_pin == 1){
  return ( ((float)RL_VALUE_MQ2*(1023-raw_adc)/raw_adc));
  }
  else if (mq_pin == 2){
  return ( ((float)RL_VALUE_MQ7*(1023-raw_adc)/raw_adc));
  }  
}


float MQCalibration(int mq_pin)
{
  int i;
  float RS_AIR_val=0,r0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {                     //take multiple samples
  RS_AIR_val += MQResistanceCalculation(analogRead(mq_pin), mq_pin);
  delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  RS_AIR_val = RS_AIR_val/CALIBARAION_SAMPLE_TIMES;              //calculate the average value

  if (mq_pin == 1){
  r0 = RS_AIR_val/RO_CLEAN_AIR_FACTOR_MQ2;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                 //according to the chart in the datasheet 
  }
  else if (mq_pin == 2){
  r0 = RS_AIR_val/RO_CLEAN_AIR_FACTOR_MQ7;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                 //according to the chart in the datasheet 
  }   
  
  return r0; 
}

float MQRead(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
  rs += MQResistanceCalculation(analogRead(mq_pin),mq_pin);
  delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;  
}

int MQ2GetGasPercentage(float rs_ro_ratio, int gas_id)
{ 
  if ( accuracy == 0 ) {
  if ( gas_id == GAS_HYDROGEN ) {
  return (pow(10,((-2.109*(log10(rs_ro_ratio))) + 2.983)));
  } else if ( gas_id == GAS_LPG ) {
  return (pow(10,((-2.123*(log10(rs_ro_ratio))) + 2.758)));
  } else if ( gas_id == GAS_METHANE ) {
  return (pow(10,((-2.622*(log10(rs_ro_ratio))) + 3.635)));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
  return (pow(10,((-2.955*(log10(rs_ro_ratio))) + 4.457)));
  } else if ( gas_id == GAS_ALCOHOL ) {
  return (pow(10,((-2.692*(log10(rs_ro_ratio))) + 3.545)));
  } else if ( gas_id == GAS_SMOKE ) {
  return (pow(10,((-2.331*(log10(rs_ro_ratio))) + 3.596)));
  } else if ( gas_id == GAS_PROPANE ) {
  return (pow(10,((-2.174*(log10(rs_ro_ratio))) + 2.799)));
  }    
  }
} 

int MQ7GetGasPercentage(float rs_ro_ratio, int gas_id)
{ 
  if ( accuracy == 0 ) {
  if ( gas_id == GAS_CARBON_MONOXIDE ) {
  return (pow(10,((-1.525*(log10(rs_ro_ratio))) + 1.994)));
  } else if ( gas_id == GAS_HYDROGEN ) {
  return (pow(10,((-1.355*(log10(rs_ro_ratio))) + 1.847)));
  } else if ( gas_id == GAS_LPG ) {
  return (pow(10,((-7.622*(log10(rs_ro_ratio))) + 8.919 )));
  } else if ( gas_id == GAS_METHANE ) {
  return (pow(10,((-11.01*(log10(rs_ro_ratio))) + 14.32)));
  } else if ( gas_id == GAS_ALCOHOL ) {
  return (pow(10,((-14.72*(log10(rs_ro_ratio))) + 19.31)));
  }   
  } 
}




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

    //-----------MQs------------------------
    Serial.print("MQ2: Calibrating...\n");                
    RoMQ2 = MQCalibration(MQ2PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
                //when you perform the calibration                    
    Serial.print("MQ2: Calibration is done...\n"); 
    
    Serial.print("MQ7: Calibrating...\n");                
    RoMQ7 = MQCalibration(MQ7PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air 
                //when you perform the calibration                    
    Serial.print("MQ7: Calibration is done...\n"); 
    Serial.print("RoMQ2=");
    Serial.print(RoMQ2);
    Serial.print("kohm");
    Serial.print("\n");
    Serial.print("RoMQ7=");
    Serial.print(RoMQ7);
    Serial.print("kohm");
    Serial.print("\n");
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

    leitura.MQ_2 = MQ2GetGasPercentage(MQRead(MQ2PIN)/RoMQ2,GAS_SMOKE);
    payload += create_payload(FUMACA, leitura.MQ_2);
    leitura.MQ_7 = MQ7GetGasPercentage(MQRead(MQ7PIN)/RoMQ7,GAS_CARBON_MONOXIDE);
    payload += create_payload(MONO_CARBONO, leitura.MQ_7);
    Serial.print("Smoke Value = ");
    Serial.println(leitura.MQ_2);
    Serial.print("CO Value = "); 
    Serial.println(leitura.MQ_7); 


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