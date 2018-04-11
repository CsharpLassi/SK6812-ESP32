#include <animation.h>
#include <led_strip.h>
#include <Arduino.h>

#include "esp32_digital_led_lib.h"

void onAnimation::drawNext(strand_t *pStrand,settings* settings,bool* interrupt)
{
  rgbwColor showColor = settings->backgroundColor;

  for (uint8_t i = 0; i < numberOfLeds; i++)
  {
    pStrand->pixels[i] = createPWMColor(showColor);

    digitalLeds_updatePixels(pStrand);
    delay(ledDelay);
  }

  while (!(*interrupt))
  {
    showColor = settings->backgroundColor;
    for (uint8_t i = 0; i < numberOfLeds; i++)
    {
      pStrand->pixels[i] = createPWMColor(showColor);
    }
    digitalLeds_updatePixels(pStrand);
    delay(10);
  }
}
