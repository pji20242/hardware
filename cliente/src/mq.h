#ifndef MQ_H
#define MQ_H

#include <Arduino.h>

// Definitions for MQ2 sensor
#define MQ2PIN 36
#define RL_VALUE_MQ2 1
#define RO_CLEAN_AIR_FACTOR_MQ2 9.577

// Definitions for MQ7 sensor
#define MQ7PIN 39
#define RL_VALUE_MQ7 1
#define RO_CLEAN_AIR_FACTOR_MQ7 26.09

// Definitions for calibration and reading
#define CALIBARAION_SAMPLE_TIMES 50
#define CALIBRATION_SAMPLE_INTERVAL 500
#define READ_SAMPLE_INTERVAL 50
#define READ_SAMPLE_TIMES 5

// Definitions for gas types
#define GAS_HYDROGEN 0
#define GAS_LPG 1
#define GAS_METHANE 2
#define GAS_CARBON_MONOXIDE 3
#define GAS_ALCOHOL 4
#define GAS_SMOKE 5
#define GAS_PROPANE 6
#define accuracy 0

// Global variables
extern float RoMQ2;
extern float RoMQ7;

// Function prototypes
float MQResistanceCalculation(int raw_adc, int mq_pin);
float MQCalibration(int mq_pin);
float MQRead(int mq_pin);
int MQ2GetGasPercentage(float rs_ro_ratio, int gas_id);
int MQ7GetGasPercentage(float rs_ro_ratio, int gas_id);

#endif