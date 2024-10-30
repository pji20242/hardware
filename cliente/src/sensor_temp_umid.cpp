#include "sensor_temp_umid.h"

SensorTempUmid::SensorTempUmid(int pin, int type) : sensor(pin, type) {}

void SensorTempUmid::begin() {
    sensor.begin();
}

float SensorTempUmid::lerTemperatura() {
    return sensor.readTemperature();
}

float SensorTempUmid::lerUmidade() {
    return sensor.readHumidity();
}
