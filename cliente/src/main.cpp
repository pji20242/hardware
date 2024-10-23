#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>         // Inclui a biblioteca WiFi para conectar o ESP32 a uma rede WiFi.
#include <PubSubClient.h> // Inclui a biblioteca PubSubClient para conectar e comunicar com um servidor MQTT.
#include <ESPmDNS.h>      // Inclui a biblioteca ESPmDNS que permite ao ESP32 responder a solicitações multicast DNS.
#include <iostream>
#include <string>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <SPI.h>
#include "ADS1X15.h"
#include <random>
#include "EmonLib.h" // inclui a biblioteca
using std::string;

// Configuração de conexão WiFi
#define SSID "MatheusGalaxy" // SSID da rede WiFi.
#define PASSWORD "mddc5504"  // Senha da rede WiFi.

// Configuração de conexão MQTT
#define MQTT_BROKER "18.231.155.48"     // Endereço do broker MQTT.
#define MQTT_PORT 1883                  // Porta do broker MQTT.
#define MQTT_Topico_Temp "temperatura"  // Tópico MQTT para publicar mensagens.
#define MQTT_Topico_Luz "luminosidade"  // Tópico MQTT para gerenciar o estado da luz.
#define MQTT_Topico_Corrente "corrente" // Tópico MQTT para gerenciar as cargas ligadas/desligadas.
#define MQTT_Topico_ReleAC "releac"
#define MQTT_Topico_ReleLamp "relelamp"
// Configuração de conexão MQTT
String MQTT_ID; // ID do cliente MQTT.

// Define o cliente de rede
WiFiClient espClient;

// Define o cliente MQTT
PubSubClient MQTT(espClient);                                                                    // Cria um cliente MQTT que usará o cliente Wi-Fi para se conectar ao broker MQTT.
string string_informacao_temp = "", string_informacao_luz = "", string_informacao_corrente = ""; // String para armazenar os dados a serem publicados.

// Define o sensor de temperatura e humidade DHT22
DHT sensor_temp(4, DHT22); // defini o pino e o tipo do DTH
long temperatura;          // variavel para temperatura

// Configurações do ADS1115
float ads_luminosidade;
ADS1115 ADS(0x49);
float f = 0;

// Define os pinos para os relés
#define RELE_PIN_LAM 13 // Pino D13 para relé da luz
#define RELE_PIN_AC 14  // Pino D14 para relé do ar condicionado

// Parametros para sensor de corrente ACS712
#define ACS_MPY 30 // ganho/calibracao da corrente
EnergyMonitor emon1;
double Irms = 0;

String gerarUUID()
{
  uint32_t values[4];
  for (int i = 0; i < 4; ++i)
  {
    values[i] = esp_random();
  }

  String uuid = String(values[0], 16) + String(values[1], 16) + String(values[2], 16) + String(values[3], 16);

  return uuid;
}

void setupWIFI()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return; // Se já estiver conectado, retorna e não faz mais nada.
  }
  else
  {
    // Conecta com o WiFi
    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(SSID);

    WiFi.begin(SSID, PASSWORD); // Inicia a conexão à rede WiFi.

    // Loop para checar a conexão
    while (WiFi.status() != WL_CONNECTED)
    {                    // Continua tentando conectar até que a conexão seja estabelecida.
      delay(500);        // Aguarda meio segundo antes de tentar novamente.
      Serial.print("."); // Imprime um ponto no Monitor Serial para indicar que ainda está tentando conectar.
    }

    // Mostra informações da conexão
    Serial.println("");
    Serial.println("WiFi conectada"); // Imprime uma mensagem no Monitor Serial indicando que a conexão foi estabelecida.
    Serial.println("Endereço IP:");
    Serial.println(WiFi.localIP()); // Imprime o endereço IP atribuído ao ESP32 pela rede WiFi.
  }
}

void FuncaoCallback(char *topic, byte *payload, unsigned int length)
{
  String msg = "";

  Serial.print("- MQTT Callback Tópico: ");
  Serial.println(topic);

  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }

  // Verifica o tópico
  if (strcmp(topic, MQTT_Topico_ReleLamp) == 0)
  {
    // Tópico de controle da luminosidade

    switch (msg.toInt())
    {
    case 0:
      digitalWrite(RELE_PIN_LAM, LOW); // Desliga o relé 1.
      Serial.println("- MQTT: Desligando Relé 1");
      break;

    case 1:
      digitalWrite(RELE_PIN_LAM, HIGH); // Liga o relé 1.
      Serial.println("- MQTT: Ligando Relé 1");
      break;

    default:
      Serial.println("- MQTT: Comando inválido para Relé 1");
      break;
    }
  }
  else if (strcmp(topic, MQTT_Topico_ReleAC) == 0)
  {
    // Tópico de controle da corrente

    switch (msg.toInt())
    {
    case 0:
      digitalWrite(RELE_PIN_AC, LOW); // Desliga o relé 2.
      Serial.println("- MQTT: Desligando Relé 2");
      break;

    case 1:
      digitalWrite(RELE_PIN_AC, HIGH); // Liga o relé 2.
      Serial.println("- MQTT: Ligando Relé 2");
      break;

    default:
      Serial.println("- MQTT: Comando inválido para Relé 2");
      break;
    }
  }
  else
  {
    Serial.println("- MQTT: Tópico desconhecido");
  }
}

void setupMQTT()
{
  // Configura a conexão com o broker MQTT

  MQTT.setServer(MQTT_BROKER, MQTT_PORT); // Configura o servidor (broker) e a porta que o cliente MQTT deve usar para se conectar.
  MQTT.setCallback(FuncaoCallback);       // Configura a função de callback que será chamada quando uma mensagem for recebida em um tópico ao qual o cliente está inscrito.

  // Executa a conexão

  while (!MQTT.connected())
  { // Este é um loop que continuará tentando conectar ao broker MQTT até que a conexão seja estabelecida.
    Serial.print("- Configuração do MQTT: Tentando se conectar ao Broker MQTT: ");
    Serial.println(MQTT_BROKER);

    if (MQTT.connect(MQTT_ID.c_str()))
    { // Tenta conectar ao broker MQTT usando o ID do cliente fornecido.
      Serial.println("- Configuração do MQTT: Conectado com sucesso");
      MQTT.subscribe(MQTT_Topico_Luz); // Se a conexão for bem-sucedida, ele se inscreve no tópico especificado por `MQTT_Topico_Luz`.
      MQTT.subscribe(MQTT_Topico_Temp);
      MQTT.subscribe(MQTT_Topico_Corrente);
      MQTT.subscribe(MQTT_Topico_ReleLamp);
      MQTT.subscribe(MQTT_Topico_ReleAC);
    }
    else
    {
      Serial.println("- Configuração do MQTT: Falha ao se conectar, tentando novamente em 2s");
      delay(2000); // Se a tentativa de conexão falhar, ele aguardará 2 segundos e tentará novamente.
    }
  }
}

void setup()
{
  Serial.begin(9600);
  sensor_temp.begin(); // inicia o sensor de temperatura
  Wire.begin();
  setupWIFI();
  MQTT_ID = gerarUUID();
  setupMQTT();

  //////////////////////////////////////
  ADS.begin();
  ADS.setGain(0); // 6.144 volt
  // select slow so the led blinks visible for the eye.
  ADS.setDataRate(0);  // 0 = slow   4 = medium   7 = fast
  f = ADS.toVoltage(); // voltage factor
  ADS.requestADC(0);

  // set the thresholds to Trigger RDY pin
  ADS.setComparatorThresholdLow(0x0000);
  ADS.setComparatorThresholdHigh(0x0200);
  ADS.setComparatorQueConvert(0); // enable RDY pin !!
  ADS.setComparatorLatch(0);

  pinMode(RELE_PIN_LAM, OUTPUT);
  pinMode(RELE_PIN_AC, OUTPUT);

  // Configura gpio para Corrente: pino analógico, calibracao.
  emon1.current(34, ACS_MPY);
}

void loop()
{
  if (isnan(sensor_temp.readTemperature()))
  {
    Serial.println(F("Erro ao ler a temperatura!"));
  }
  else
  {
    temperatura = sensor_temp.readTemperature();
  }

  // imprime na serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print("°");

  float mediaLuminosidade;
  if (ADS.isReady())
  {
    int numLeituras = 10; // Número desejado de leituras para a média
    float totalLuminosidade = 0.0;

    for (int i = 0; i < numLeituras; ++i)
    {
      int16_t val_0 = ADS.readADC(3);
      ADS.requestADC(0); // solicita uma nova leitura
      float luminosidade = val_0 * f;

      totalLuminosidade += luminosidade;
      delay(50); // Aguarda um curto intervalo entre as leituras para evitar interferências
    }

    mediaLuminosidade = totalLuminosidade / numLeituras;
    Serial.print("\tMédia de Luminosidade: ");
    Serial.println(mediaLuminosidade, 3); // Ajuste o número para o número desejado de casas decimais
  }

  /* chama função para calculo da corrente */
  Irms = emon1.calcIrms(1996); // Calculate Irms only(int:numero_de_amostras)

  Serial.print("\tCorrente: ");
  Serial.print(Irms);
  Serial.println("");

  string MQTT_ID_concatenar = MQTT_ID.c_str(); // Converte de volta para std::string se necessário

  // Armazena os dados a serem publicados em uma string para envio ao MQTT
  string_informacao_temp = std::to_string(temperatura) + " " + MQTT_ID_concatenar;
  string_informacao_luz = std::to_string(mediaLuminosidade * 100) + " " + MQTT_ID_concatenar;
  string_informacao_corrente = std::to_string(Irms) + " " + MQTT_ID_concatenar;

  // Publica a string `string_informacao_` para o tópico MQTT especificado por `MQTT_Topico_publicar`. Qualquer cliente MQTT que esteja inscrito neste tópico receberá essa mensagem.
  MQTT.publish(MQTT_Topico_Temp, string_informacao_temp.c_str());
  MQTT.publish(MQTT_Topico_Luz, string_informacao_luz.c_str());
  MQTT.publish(MQTT_Topico_Corrente, string_informacao_corrente.c_str());

  setupWIFI(); // Chama a função `setupWIFI()` que definimos anteriormente para verificar a conexão Wi-Fi e tentar reconectar se necessário.

  setupMQTT(); // Chama a função `setupMQTT()` que definimos anteriormente para verificar a conexão MQTT e tentar reconectar se necessário.

  MQTT.loop(); // Esta linha é necessária para manter a conexão MQTT. Ela deve ser chamada regularmente para permitir que o cliente MQTT processe as mensagens recebidas e mantenha a conexão com o broker MQTT.

  delay(2000);
}