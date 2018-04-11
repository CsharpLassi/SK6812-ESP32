#include <led_color.h>

#include <WString.h>


using namespace std;

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

pixelColor_t createPWMColor(rgbwColor color)
{
  pixelColor_t pwmColor;

  uint8_t maxValue = 0;
  if (color.r > maxValue)
    maxValue = color.r;

  if (color.g > maxValue)
    maxValue = color.g;

  if (color.b > maxValue)
    maxValue = color.b;

  if (maxValue == 0)
    return pwmColor;

  float fac = (float)brightLimit/maxValue;

  if (fac < 1)
  {
    pwmColor.r = color.r*fac;
    pwmColor.g = color.g*fac;
    pwmColor.b = color.b*fac;
  }
  else
  {
    pwmColor.r = color.r;
    pwmColor.g = color.g;
    pwmColor.b = color.b;
  }



  return pwmColor;
}
