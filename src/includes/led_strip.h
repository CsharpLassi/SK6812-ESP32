#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include "esp32_digital_led_lib.h"

#include <led_color.h>

extern strand_t pStrand;
extern const uint8_t pinNum;
extern const uint8_t numberOfLeds;

extern const uint8_t brightLimit;

void initLedStrip();
void ledStripUpdateTask(void *pvParams);

void setOn();
void setOn(rgbwColor);
void setOff();

void setBackgroundColor(rgbwColor color);

#endif
