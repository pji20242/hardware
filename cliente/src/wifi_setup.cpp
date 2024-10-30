#include "wifi_setup.h"
#include <WiFi.h>
#include <Arduino.h>

void setupWiFi(const char* ssid, const char* password) {
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi conectada");
    Serial.println(WiFi.localIP());
}
