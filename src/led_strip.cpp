#include <led_strip.h>
#include <stdint.h>

#include <Arduino.h>
typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  uint8_t white;
} pixelColor;

typedef struct
{
  bool whiteMode;
  uint16_t delayPerLed;

  pixelColor colors[256];

} animation;

static animation* currentAnimation = NULL;


void initLedStrip()
{
  pinMode(pinNum, OUTPUT);
  digitalWrite(pinNum, HIGH);


  if (digitalLeds_initStrands(&pStrand, 1))
  {
    while (true)
    {
      digitalWrite(22, HIGH);
      delay(1000);

      digitalWrite(22, LOW);
      delay(1000);
    };
  }
  digitalLeds_resetPixels(&pStrand);
}

void ledStripUpdateTask(void *pvParams)
{
  initLedStrip();

  while(1)
  {


    if(currentAnimation == NULL)
    {
      delay(10);
      continue;
    }

    uint16_t delayTime = 0;

    uint8_t startLed = 0;
    uint8_t endLed = numberOfLeds;

    bool end = true;

    if (currentAnimation->delayPerLed > 0)
    {
      static uint8_t currentLed = 0;
      delayTime = currentAnimation->delayPerLed;

      startLed = currentLed;
      endLed = ++currentLed;

      end = false;

      if (endLed >= numberOfLeds -1)
      {
        end = true;
        currentLed= 0;
      }

    }

    for (uint8_t i = startLed; i < endLed; i++)
    {
      if (currentAnimation->whiteMode)
      {
        if (pStrand.ledType == LED_SK6812W_V1)
        {
          pStrand.pixels[i] = pixelFromRGBW(0,0,0,currentAnimation->colors[i].white);
        }
        else
        {
          uint8_t value = currentAnimation->colors[i].white;
          pStrand.pixels[i] = pixelFromRGB(value,value,value);
        }
      }

    }


    if(end)
      currentAnimation = NULL;
    //strip.show();
    digitalLeds_updatePixels(&pStrand);
    delay(10 + delayTime);
  }
}

void setOn()
{
  static animation animation;

  animation.whiteMode = true;
  animation.delayPerLed = 100;
  for (uint8_t i = 0; i < numberOfLeds; i++)
  {
    animation.colors[i].white = brightLimit;
  }

  currentAnimation = &animation;
}

void setOff()
{
  static animation animation;

  animation.whiteMode = true;
  for (uint8_t i = 0; i < numberOfLeds; i++)
  {
    animation.colors[i].white = 0;
  }

  currentAnimation = &animation;
}
