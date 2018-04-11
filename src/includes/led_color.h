#ifndef __LED_COLOR_H__
#define __LED_COLOR_H__

#include <stdint.h>
#include <WString.h>

#include "esp32_digital_led_lib.h"

using namespace std;

struct rgbwColor
{
  uint8_t r;
  uint8_t g;
  uint8_t b;

  uint8_t w;
};

extern const uint8_t brightLimit;

rgbwColor parseColor(String strValue);

pixelColor_t createPWMColor(rgbwColor);

#endif
