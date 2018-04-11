#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "esp32_digital_led_lib.h"

#include <stdint.h>
#include <led_color.h>

struct settings
{
  rgbwColor backgroundColor;
};

class animation
{
  public:
    virtual void drawNext(strand_t*, settings*, bool* interrupt);
};

class onAnimation : public animation
{
  public:
    uint16_t ledDelay = 0;

    void drawNext(strand_t*, settings*, bool* interrupt);
};

class offAnimation : public animation
{
  public:
    void drawNext(strand_t*, settings*, bool* interrupt);
};


#endif
