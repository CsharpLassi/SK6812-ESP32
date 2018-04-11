#include <led_color.h>

#include <WString.h>


using namespace std;

const float piOver6 = 0.5235987755982988;
const float pi = 3.141592653589793;

rgbwColor parseColor(String strValue)
{

  rgbwColor output;
  output.r = 0;
  output.g = 0;
  output.b = 0;

  if (strValue[0] == '%' && strValue[1] == '2' && strValue[2] == '3')
  {
    output.r = strtoul(strValue.substring(3,5).c_str(), NULL, 16);
    output.g = strtoul(strValue.substring(5,7).c_str(), NULL, 16);
    output.b = strtoul(strValue.substring(7,9).c_str(), NULL, 16);
  }

  return output;
}

hsvwColor convertToHSV(rgbwColor color)
{
  hsvwColor newColor;
  newColor.w = color.w;

  float r = color.r /(float)255;
  float g = color.g /(float)255;
  float b = color.b /(float)255;

  float max = r;
  float min = r;
  uint8_t maxColor = 0;

  if (g > max)
  {
    maxColor = 1;
    max = g;
  }
  if(g < min)
  {
    min = g;
  }

  if (b > max)
  {
    maxColor = 2;
    max = b;
  }
  if(b < min)
  {
    min = b;
  }

  float delta = max -min;

  if (delta == 0)
    newColor.h = 0;
  else if(maxColor == 0 )
    newColor.h = piOver6 * (0+(g-b)/(delta));
  else if(maxColor == 1 )
    newColor.h = piOver6 * (2+(b-r)/(delta));
  else if(maxColor == 2 )
    newColor.h = piOver6 * (4+(r-g)/(delta));

  if(newColor.h < 0)
    newColor.h += pi;

  if (max == 0)
    newColor.s = 0;
  else
    newColor.s = delta/max;

  newColor.v = max;

  return newColor;
}

pixelColor_t createPWMColor(rgbwColor color)
{
  hsvwColor hsvValue = convertToHSV(color);
  pixelColor_t pwmColor;

  float h = hsvValue.h;
  float s = hsvValue.s;
  float v = hsvValue.v;

  if(v > (maxBrightness/(float)255))
    v =  maxBrightness/(float)255;

  //v = 0.01;

  uint8_t hi = (uint16_t) (h / piOver6);

  float f = (h / piOver6) - hi;

  float p = v *(1-s);
  float q = v *(1-s*f);
  float t = v *(1-s*(1-f));

  float r;
  float g;
  float b;


  if(hi == 0 || hi == 6)
  {
    r = v;
    g = t;
    b = p;
  }
  else if (hi==1)
  {
    r = q;
    g = v;
    b = p;
  }
  else if (hi==2)
  {
    r = p;
    g = v;
    b = t;
  }
  else if (hi==3)
  {
    r = p;
    g = q;
    b = v;
  }
  else if (hi==4)
  {
    r = t;
    g = p;
    b = v;
  }
  else if (hi==5)
  {
    r = v;
    g = p;
    b = q;
  }

  uint8_t br = (uint8_t)(r*255);
  uint8_t bg = (uint8_t)(g*255);
  uint8_t bb = (uint8_t)(b*255);

  switch (order)
  {
      case rgb:
        return pixelFromRGB(br,bg,bb);
      case rbg:
        return pixelFromRGB(br,bb,bg);
      case gbr:
        return pixelFromRGB(bg,bb,br);
      case grb:
        return pixelFromRGB(bg,br,bb);
      case brg:
        return pixelFromRGB(bb,br,bg);
      case bgr:
        return pixelFromRGB(bb,bg,br);
  }

  return pixelFromRGB(br,bg,bb);
}
