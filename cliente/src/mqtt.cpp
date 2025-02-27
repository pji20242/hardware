#include "mqtt.h"
#include <Arduino.h>

MQTT::MQTT(const char* broker, int port) : mqttClient(espClient) {
    mqttClient.setServer(broker, port);
}

void MQTT::setCallback(void (*callback)(char*, byte*, unsigned int)) {
    mqttClient.setCallback(callback);
}

//bool MQTT::connect(const String& clientId) {
//    if (!mqttClient.connected()) {
//        return mqttClient.connect(clientId.c_str());
//    }
//    return true;
//}
bool MQTT::connect(const String& clientId, const String& User, const String& Pass){
    if(!mqttClient.connected()){
        return mqttClient.connect(clientId.c_str(),User.c_str(),Pass.c_str());
    }
    return true;
}



void MQTT::subscribe(const char* topic) {
    mqttClient.subscribe(topic);
}

void MQTT::publish(const char* topic, const char* payload) {
    mqttClient.publish(topic, payload);
}

void MQTT::loop() {
    mqttClient.loop();
}
