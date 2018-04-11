#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "esp32_digital_led_lib.h"

#include <stdint.h>
#include <led_color.h>
#include <led_strip.h>

void cleanUp(strand_t*,rgbwColor color);

class animation
{
  public:
    virtual void start();
    virtual void end();
    virtual void drawNext(strand_t*, settings*, bool* interrupt);
};

class onAnimation : public animation
{
  public:
    uint16_t ledDelay = 0;
    void start(){ };
    void end(){ };
    void drawNext(strand_t*, settings*, bool* interrupt);
};

class barAnimation : public animation
{
  private:
    uint8_t step = 0;
    bool foregroundColorIsSet = false;
    rgbwColor foregroundColor;

    rgbwColor getForegroundColor(settings*);

  public:
    uint16_t ledDelay = 0;

    void setForegroundColor(rgbwColor);

    void start();
    void end();
    void drawNext(strand_t*, settings*, bool* interrupt);
};

class offAnimation : public animation
{
  public:
    void start(){ };
    void end(){ };
    void drawNext(strand_t*, settings*, bool* interrupt);
};


#endif
