#include <led_strip.h>
#include <animation.h>

#include <stdint.h>
#include <Arduino.h>

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

    currentAnimation->drawNext(&pStrand);
    currentAnimation = NULL;
  }
}

void setOn()
{
  static onAnimation animation;
  animation.ledDelay = 100;



  currentAnimation = &animation;
}

void setOff()
{
  static offAnimation animation;
  animation.ledDelay = 100;



  currentAnimation = &animation;
}
