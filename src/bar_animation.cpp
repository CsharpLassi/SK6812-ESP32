#include <animation.h>
#include <led_strip.h>
#include <Arduino.h>

#include "esp32_digital_led_lib.h"

void barAnimation::start()
{
  step = 0;
}

void barAnimation::setForegroundColor(rgbwColor useColor)
{
  foregroundColorIsSet = true;
  foregroundColor = useColor;
}

rgbwColor barAnimation::getForegroundColor(settings* settings)
{
  if(foregroundColorIsSet)
    return foregroundColor;
  return settings->foregroundColor;
}

void barAnimation::drawNext(strand_t *pStrand,settings* settings,bool* interrupt)
{


    if (ledDelay == 0)
      ledDelay = 100;


    rgbwColor backgroundColor;
    rgbwColor foregroundColor;

    while (!(*interrupt))
    {
      backgroundColor = settings->backgroundColor;
      foregroundColor = getForegroundColor(settings);

      cleanUp(pStrand,backgroundColor);

      pStrand->pixels[step] = createPWMColor(foregroundColor);
      step = (step +1) %numberOfLeds;



      digitalLeds_updatePixels(pStrand);
      delay(ledDelay);
    }
}

void barAnimation::end()
{
  foregroundColorIsSet = false;
}
