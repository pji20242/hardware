#ifndef MQTT_CALLBACK_H
#define MQTT_CALLBACK_H

#include <Arduino.h>

void callback(char* topic, byte* payload, unsigned int length);

#endif
