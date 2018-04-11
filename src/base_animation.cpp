#include <animation.h>

void cleanUp(strand_t* pStrand,rgbwColor color)
{
  for (uint8_t i = 0; i < numberOfLeds; i++)
  {
    pStrand->pixels[i] = createPWMColor(color);
  }
}
