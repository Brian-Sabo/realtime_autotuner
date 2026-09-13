//header include guard better than GPIO_FUNC_H
#pragma once
#include "hal/gpio_hal.h"
#include "Arduino.h"

static inline bool readPin(uint8_t pin_num){
  return gpio_get_level((gpio_num_t)pin_num);
}

static inline void setPin(uint8_t pin_num){
  gpio_set_level((gpio_num_t)pin_num, 1);
}

static inline void rstPin(uint8_t pin_num){
  gpio_set_level((gpio_num_t)pin_num, 0);
}
