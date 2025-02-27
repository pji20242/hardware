#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTT {
public:
    MQTT(const char* broker, int port);
    void setCallback(void (*callback)(char*, byte*, unsigned int));
//    bool connect(const String& clientId);
    bool connect(const String& clientId, const String& User , const String& Pass);
    void subscribe(const char* topic);
    void publish(const char* topic, const char* payload);
    void loop();

private:
    WiFiClient espClient;
    PubSubClient mqttClient;
};

#endif
