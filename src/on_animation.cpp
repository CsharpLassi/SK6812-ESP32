#include <animation.h>
#include <led_strip.h>
#include <Arduino.h>

#include "esp32_digital_led_lib.h"

void onAnimation::drawNext(strand_t *pStrand)
{
  for (uint8_t i = 0; i < numberOfLeds; i++)
  {
    pStrand->pixels[i] = pixelFromRGB(255>>4,143>>6,197>>6);

    digitalLeds_updatePixels(pStrand);
    delay(ledDelay);
  }
}
