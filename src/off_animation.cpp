#include <animation.h>
#include <led_strip.h>
#include <Arduino.h>

#include "esp32_digital_led_lib.h"

void offAnimation::drawNext(strand_t *pStrand)
{
  digitalLeds_resetPixels(pStrand);
}
