#include "battery.h"

#include <esp_adc_cal.h>

#include "Watchy.h"  // for logging macros

namespace Watchy {

esp_adc_cal_characteristics_t *getADCCharacteristics() {
  static esp_adc_cal_characteristics_t *adc_chars;  // default is nullptr
  if (!adc_chars) {
    // only initialize it if we're actually going to use it.
    adc_chars = new esp_adc_cal_characteristics_t();
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_12);
    esp_adc_cal_value_t cal = esp_adc_cal_characterize(
        ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, adc_chars);
    if (cal == ESP_ADC_CAL_VAL_DEFAULT_VREF) {
      log_w("adc calibration is using default vref. Accuracy will suffer.");
    }
  }
  return adc_chars;
}

float getBatteryVoltage() {
  // make sure ADC is initialized before we read it.
  esp_adc_cal_characteristics_t *adcChar = getADCCharacteristics();
  // average 4 samples to reduce noise
  int rawAdcVals[] = {0,0,0,0};
  int rawAvg = 0;
  for( uint8_t i = 0; i<4; i++){
    rawAdcVals[i] = adc1_get_raw(ADC1_CHANNEL_6);
    rawAvg += rawAdcVals[i];
    log_d("rawAdcVals[%d]: %d", i, rawAdcVals[i]);
  }
  rawAvg /= 4;
  log_d("rawAvg: %d", rawAvg);
  uint32_t rawAvgCalibrated = esp_adc_cal_raw_to_voltage(rawAvg, adcChar);
  log_d("rawAvgCalibrated: %d", rawAvgCalibrated);
  float newVolt = rawAvgCalibrated * 2.0 / 1000.0;
  log_d("newVolt: %f", newVolt);

  int raw =
      (adc1_get_raw(ADC1_CHANNEL_6) + adc1_get_raw(ADC1_CHANNEL_6) +
       adc1_get_raw(ADC1_CHANNEL_6) + adc1_get_raw(ADC1_CHANNEL_6) +
       2) /
      4;
  log_d("raw: %d", raw);
  uint32_t rawCalibrated = esp_adc_cal_raw_to_voltage(raw, adcChar);
  log_d("raw calibrated: %d", rawCalibrated);
  
  float volt = rawCalibrated * 2.0 / 1000.0;
  log_d("volt: %f", volt);

  return volt;
}

/*
| hours|voltage| rngU | rngT|   coeffUT     |  TTL  | Battery|
|-----:|------:|-----:|----:|--------------:|------:|-------:|
|  0.00| 4.180 |      |     |               |109.57 |100.00% |
| 37.30| 3.850 |-0.330|37.30|-0.008847184987|	72.27 |	65.96% |
| 76.30| 3.700 |-0.150|39.00|-0.003846153846|	33.27 |	30.36% |
|104.37| 3.450 |-0.250|28.07|-0.008906305664|	 5.20 |	 4.75% |
|108.20| 3.310 |-0.140| 3.83|-0.036553524800|	 1.37 |	 1.25% |
|109.23| 3.158 |-0.152| 1.03|-0.147572815500|	 0.34 |	 0.31% |
|109.52| 3.000 |-0.158| 0.29|-0.544827586200|	 0.05 |	 0.05% |
|109.57| 2.850 |-0.150| 0.05|-3.000000000000|	 0.00	|  0.00% |
 */

typedef struct batFnTabEntry {
  float hours;
  float voltage;
  float rngU;
  float rngT;
  float coeffUT;
  float ttl;
  float status;
} batFnTabEntry;

const batFnTabEntry batFnTab[] PROGMEM = {
//  |  hours| voltage| rngU  | rngT |   coeffUT      |  TTL    | Battery |
    {  0.00f, 4.180f , 0.0f  ,  0.0f, 0.0f           , 109.57f , 100.00f }
  , { 37.30f, 3.850f ,-0.330f,37.30f,-0.008847184987f,	72.27f ,	65.96f }
  , { 76.30f, 3.700f ,-0.150f,39.00f,-0.003846153846f,	33.27f ,	30.36f }
  , {104.37f, 3.450f ,-0.250f,28.07f,-0.008906305664f,	 5.20f ,	 4.75f }
  , {108.20f, 3.310f ,-0.140f, 3.83f,-0.036553524800f,	 1.37f ,	 1.25f }
  , {109.23f, 3.158f ,-0.152f, 1.03f,-0.147572815500f,	 0.34f ,	 0.31f }
  , {109.52f, 3.000f ,-0.158f, 0.29f,-0.544827586200f,	 0.05f ,	 0.05f }
  , {109.57f, 2.850f ,-0.150f, 0.05f,-3.000000000000f,	 0.00f ,   0.00f }
};
const byte batFnTabSize = 8;
const float ttlMax      = 109.57f;

byte getIndexForVoltage(float voltage){
  for( int i=0; i<batFnTabSize; i++){
    if( batFnTab[i].voltage < voltage )
      return (byte)(i<1?1:i);
  }
  return batFnTabSize-1;
}

//ttl(u) = TTL<sub>max</sub> - (tab(u).hours + ((u - tab(u).voltage) / tab(u).coeffUT))
float getTTL(float u){
  byte i = getIndexForVoltage(u);
  float ttl = ttlMax - (batFnTab[i].hours + ((u - batFnTab[i].voltage) / batFnTab[i].coeffUT));
  log_d("getTTL(%f) returns: %f", u, ttl);
  return ttl;
}

float getBatteryState(float batteryVoltage) {
  //return mapBatteryVoltage(batteryVoltage, ((float)1.00), ((float)0.00));
  float batteryState = (float) (getTTL(batteryVoltage) / ttlMax);
  log_d("getBatteryState(%f) returns: %f", batteryVoltage, batteryState);
  return batteryState;
}


float mapBatteryVoltage(const float v,
               const float outMax, const float outMin) {
  return (min(BATTERY_MAX_VOLTAGE, max(BATTERY_MIN_VOLTAGE, v)) - BATTERY_MIN_VOLTAGE) / (BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) *
             (outMax - outMin) +
         outMin;
}

}  // namespace Watchy
