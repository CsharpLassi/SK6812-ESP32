#include <led_strip.h>
#include <animation.h>

#include <stdint.h>
#include <Arduino.h>

static animation* currentAnimation = NULL;
static settings currentSettings;
static bool interrupt = false;


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

    currentAnimation->drawNext(&pStrand, &currentSettings, &interrupt);
    currentAnimation = NULL;
    interrupt = false;
  }
}

void setAnimation(animation* newAnimation)
{
  if (currentAnimation != NULL)
  {
    interrupt = true;
    while (interrupt)
      delay(10);
  }

  currentAnimation = newAnimation;
}

void setOn()
{
  static onAnimation animation;
  animation.ledDelay = 100;


  setAnimation(&animation);
}

void setOn(rgbwColor color)
{
  static onAnimation animation;
  animation.ledDelay = 100;

  setBackgroundColor(color);


  setAnimation(&animation);
}

void setOff()
{
  static offAnimation animation;

  setAnimation(&animation);
}

void setBackgroundColor(rgbwColor color)
{
  currentSettings.backgroundColor = color;
}
