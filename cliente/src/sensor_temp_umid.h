#ifndef SENSOR_TEMP_UMID_H
#define SENSOR_TEMP_UMID_H

#include <DHT.h>
#include <DHT_U.h>

class SensorTempUmid {
public:
    SensorTempUmid(int pin, int type);
    void begin();
    float lerTemperatura();
    float lerUmidade();

private:
    DHT sensor;
};

#endif
