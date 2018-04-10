#include <led_strip.h>
#include <stdint.h>

#include <Arduino.h>

void initLedStrip()
{
  pinMode(pinNum, OUTPUT);
  digitalWrite(pinNum, HIGH);
  if (digitalLeds_initStrands(&pStrand, 1))
  {
    while (true)
    {
    };
  }
  digitalLeds_resetPixels(&pStrand);
}

void ledStripUpdateTask(void *pvParams)
{
  while(1);
}
