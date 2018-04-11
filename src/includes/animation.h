#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "esp32_digital_led_lib.h"

#include <stdint.h>

typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;

  uint8_t white;
} pixelColor;

class animation
{
  public:
    virtual void drawNext(strand_t* pStrand);
};

class onAnimation : public animation
{
  public:
    uint16_t ledDelay = 0;
    void drawNext(strand_t* pStrand);
};

class offAnimation : public animation
{
  public:
    uint16_t ledDelay = 0;
    void drawNext(strand_t* pStrand);
};


#endif
