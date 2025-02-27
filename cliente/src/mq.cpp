#include "mq.h"

float RoMQ2 = 10; // Ro is initialized to 10 kilo ohms
float RoMQ7 = 10; // Ro is initialized to 10 kilo ohms

float MQResistanceCalculation(int raw_adc, int mq_pin)
{
  if (mq_pin == MQ2PIN){
    return ( ((float)RL_VALUE_MQ2*(4096-raw_adc)/raw_adc));
  }
  else if (mq_pin == MQ7PIN){
    return ( ((float)RL_VALUE_MQ7*(4096-raw_adc)/raw_adc));
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

  if (mq_pin == MQ2PIN){
  r0 = RS_AIR_val/RO_CLEAN_AIR_FACTOR_MQ2;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                 //according to the chart in the datasheet 
  }
  else if (mq_pin == MQ7PIN){
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