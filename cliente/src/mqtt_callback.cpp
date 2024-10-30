#include "mqtt_callback.h"

void callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }
    Serial.print("MQTT Callback: ");
    Serial.println(msg);
}
