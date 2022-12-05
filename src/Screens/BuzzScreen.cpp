#include "BuzzScreen.h"

#include "Watchy.h"

#include "driver/ledc.h"
#include "esp_err.h"

void buzz(){
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = (10),  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .gpio_num       = VIB_MOTOR_PIN,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_channel_config(&ledc_channel));

    //start
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (4095)));
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
    delay(500);    
    //for( byte i = 0; i < 100; i++) {
      ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, 20));
      delay(500);    
      ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, 40);
      delay(500);    
      ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, 50);
      delay(500);    
      ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, 100));
      delay(500);    
    //}

    //stop, we hope ;(
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, (0)));
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
    ESP_ERROR_CHECK_WITHOUT_ABORT(ledc_stop(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0, 0));

    pinMode(VIB_MOTOR_PIN, INPUT);
}

void BuzzScreen::show() {
  Watchy::RTC.setRefresh(RTC_REFRESH_NONE);

  //buzz();
  
  pinMode(VIB_MOTOR_PIN, OUTPUT);
  // five cycles of 3 buzzes each
  for (int c = 0; c < 5; c++) {
    for (int b = 0; b < 3; b++) {
      digitalWrite(VIB_MOTOR_PIN, true);
      delay(100);
      digitalWrite(VIB_MOTOR_PIN, false);
      delay(100);
    }
    delay(600);
  }
  
  Screen::back(); // automatically return to parent
}
