#include "esp32_digital_led_lib.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <uri_parser.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <inttypes.h>
#include <stdio.h>

const char *ssid = "";
const char *password = "";

WiFiServer server(80);

const int numberOfLeds = 94;

strand_t pStrand = {.rmtChannel = 0, .gpioNum = 4, .ledType = LED_SK6812W_V1, .brightLimit = 255, .numPixels = numberOfLeds, .pixels = nullptr, ._stateVars = nullptr};

int stepper = 0;
int colord = 0;

int step = 0;
int o = 0;
uint8_t colorPlace = 0;
bool rgb = false;
bool strobo = false;
bool white = false;
bool wanderLedOn = false;
bool singleColorLedOn = false;
bool rgbCycleColorLedOn = false;
bool rgbWanderColorLedOn = false;
bool christmasOn = false;
bool randomOn = false;

bool reverseMode = false;

uint8_t userDelay = 0;
uint32_t userColor = 4278190080;
pixelColor_t pixelColor;
pixelColor_t pixelColorBrightness;
uint8_t userBrightness = 120;

std::vector<std::tuple<String, uint32_t>> getParams(std::string s)
{
  std::vector<std::tuple<String, uint32_t>> params;
  int locInStr = 0;
  int locInStr2 = 0;
  
  String str = s.c_str();
  String paramName = "";
  while (true)
  {
    locInStr = str.indexOf('=', locInStr2);
    if (locInStr == -1)
      break;
    else
      paramName = str.substring(locInStr2 + 1, locInStr);
    locInStr2 = str.indexOf('&', locInStr);
    
    if (locInStr2 == -1)
      locInStr2 = str.length();
    
    uint32_t val;
    String strVal = str.substring(locInStr + 1, locInStr2);

    if(strVal[0] == 'x')
      val = std::strtoul(strVal.substring(1).c_str(), NULL, 16);
    else
      val = std::strtoul(strVal.c_str(), NULL, 10);
    
    params.push_back(std::make_tuple(paramName, val));
  }
  return params;
}

void disableAll()
{
  rgb = false;
  strobo = false;
  white = false;
  wanderLedOn = false;
  singleColorLedOn = false;
  rgbCycleColorLedOn = false;
  rgbWanderColorLedOn = false;
  christmasOn = false;
  randomOn = false;
  pixelColor.num = 0;
  pixelColorBrightness.num = 0;
}

void stepMethod(uint8_t stepSize = 1)
{
  if (reverseMode)
    step -= stepSize;
  else
    step += stepSize;
  step = (step + numberOfLeds) % numberOfLeds;
}

std::vector<uint8_t> transferBetweenTwoColors(uint8_t numPixels, uint8_t offset)
{
  std::vector<uint8_t> ret;
  ret.push_back((255 - ((255 / numPixels) * offset)));
  ret.push_back(0 + ((255 / numPixels) * offset));
  return ret;
}

void rgbWander()
{
  uint8_t y;
  for (uint8_t i = 0; i < numberOfLeds - (numberOfLeds % 3); i++)
  {
    std::vector<uint8_t> colors = transferBetweenTwoColors(numberOfLeds / 3, (i % (numberOfLeds / 3)) + 1);
    y = (i + step) % numberOfLeds;
    if (i < numberOfLeds / 3)
    {
      pixelColor.r = colors[0] * (userBrightness / 255.0);
      pixelColor.g = colors[1] * (userBrightness / 255.0);
      pStrand.pixels[y] = pixelColor;
    }
    else if (i < (numberOfLeds / 3) * 2)
    {
      pixelColor.g = colors[0] * (userBrightness / 255.0);
      pixelColor.b = colors[1] * (userBrightness / 255.0);
      pStrand.pixels[y] = pixelColor;
    }
    else
    {
      pixelColor.b = colors[0] * (userBrightness / 255.0);
      pixelColor.r = colors[1] * (userBrightness / 255.0);
      pStrand.pixels[y] = pixelColor;
    }
  }
  stepMethod();
}

bool redDown = true;
bool greenDown = false;
bool blueDown = false;
void rgbCycle()
{
  pixelColorBrightness = pixelFromRGBW(pixelColor.r * (userBrightness / 255.0), pixelColor.g * (userBrightness / 255.0), pixelColor.b * (userBrightness / 255.0), 0);
  for (int i = 0; i < numberOfLeds; i++)
    pStrand.pixels[i] = pixelColorBrightness;

  if (reverseMode)
  {
    if (pixelColor.r == 254 && redDown)
    {
      redDown = false;
      blueDown = true;
    }
    else if (pixelColor.b == 254 && blueDown)
    {
      blueDown = false;
      greenDown = true;
    }
    else if (pixelColor.g == 254 && greenDown)
    {
      greenDown = false;
      redDown = true;
    }
    if (redDown)
    {
      pixelColor.r += 1;
      pixelColor.g -= 1;
    }
    else if (greenDown)
    {
      pixelColor.g += 1;
      pixelColor.b -= 1;
    }
    else if (blueDown)
    {
      pixelColor.b += 1;
      pixelColor.r -= 1;
    }
  }
  else
  {
    if (pixelColor.r == 0 && redDown)
    {
      redDown = false;
      greenDown = true;
    }
    else if (pixelColor.g == 0 && greenDown)
    {

      blueDown = true;
      greenDown = false;
    }
    else if (pixelColor.b == 0 && blueDown)
    {
      blueDown = false;
      redDown = true;
    }
    if (redDown)
    {
      pixelColor.r -= 1;
      pixelColor.g += 1;
    }
    else if (greenDown)
    {
      pixelColor.g -= 1;
      pixelColor.b += 1;
    }
    else if (blueDown)
    {
      pixelColor.b -= 1;
      pixelColor.r += 1;
    }
  }
  stepMethod();
}

void ledStripUpdateTask(void *pvParams)
{

  while (true)
  {
    if (rgb)
    {
      stepMethod();
      int k = step % 4;
      pixelColor = pixelFromRGBW(k == 0 ? userBrightness : 0,
                                 k == 1 ? userBrightness : 0,
                                 k == 2 ? userBrightness : 0,
                                 k == 3 ? userBrightness : 0);
      for (int i = 0; i < numberOfLeds; i++)
        pStrand.pixels[i] = pixelColor;
    }
    else if (strobo)
    {
      bool offCycle = step % 2 == 0;
      pixelColor.num = offCycle ? 0 : userColor;
      pixelColorBrightness = pixelFromRGBW(pixelColor.r * (userBrightness / 255.0), pixelColor.g * (userBrightness / 255.0), pixelColor.b * (userBrightness / 255.0), pixelColor.w * (userBrightness / 255.0));
      for (int i = 0; i < numberOfLeds; i++)
        pStrand.pixels[i] = pixelColorBrightness;
      stepMethod();
    }
    else if (white)
    {

      for (int i = 0; i < numberOfLeds; i++)
      {
        pStrand.pixels[i] = pixelFromRGBW(0,
                                          0,
                                          0,
                                          userBrightness);
      }
    }
    else if (wanderLedOn)
    {
      pixelColor.num = userColor;
      pStrand.pixels[(step + 1) % numberOfLeds] = pixelFromRGBW(0, 0, 0, 0);
      pStrand.pixels[step] = pixelFromRGBW(pixelColor.r * (userBrightness / 255.0), pixelColor.g * (userBrightness / 255.0), pixelColor.b * (userBrightness / 255.0),  pixelColor.w * (userBrightness / 255.0));
      pStrand.pixels[(numberOfLeds + step - 1) % numberOfLeds] = pixelFromRGBW(0, 0, 0, 0);
      stepMethod();
    }
    else if (rgbWanderColorLedOn)
      rgbWander();
    else if (rgbCycleColorLedOn)
      rgbCycle();
    else if (singleColorLedOn)
    {
      pixelColor.num = userColor;
      for (int i = 0; i < numberOfLeds; i++)
        pStrand.pixels[i] = pixelFromRGBW(pixelColor.r * (userBrightness / 255.0), pixelColor.g * (userBrightness / 255.0), pixelColor.b * (userBrightness / 255.0),  pixelColor.w * (userBrightness / 255.0));
    }
    else{
      for (int i = 0; i < numberOfLeds; i++)
        pStrand.pixels[i] = pixelFromRGBW(0, 0, 0, 0);
      delay(100);
    }

    delay(userDelay);
    digitalLeds_updatePixels(&pStrand);
  }
}
void wifiServerTask(void *pvParams)
{
  while (true)
  {
    WiFiClient client = server.available();

    if (client)
    {
      std::string currentLine = "";
      while (client.connected())
      {
        if (client.available())
        {
          std::string request = "";
          char c = ' ';
          for (int i = 0; i < 4; i++)
            client.read();
          while (true)
          {
            c = client.read();

            if (c == ' ')
            {
              //Serial.println(request);
              while (client.available())
                client.read();
              break;
            }
            request += c;
          }
          //Serial.print(request + "|" + request.length());
          std::transform(request.begin(), request.end(), request.begin(), ::tolower);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          client.print(" <a href=\"/H\">Click here to to turn the LED on pin 5 on.</a><br>");
          client.print(" <a href=\"/L\">Click here to to turn the LED on pin 5 off.</a><br>");
          client.print(" <a href=\"/RGB\">Click here to to RGB.</a><br>");
          client.print(" <a href=\"/STROBO\">Click here to to STROBO.</a><br>");
          client.print(" <a href=\"/WANDER\">Click here to to turn WANDER.</a><br>");
          client.print(" <a href=\"/rgbwander\">Click here to to turn RGBWANDER.</a><br>");
          client.print(" <a href=\"/rgbcycle\">Click here to to turn RGBCYCLE.</a><br>");
          client.print(" <a href=\"/single\">Click here to to turn USERCOLOR.</a><br>");
          client.print(" <a href=\"/reverse\">Click here to to turn REVERSE.</a><br>");
          client.print(" <a href=\"/OFF\">Click here to  to turn OFF.</a><br>");

          client.println();

          Uri u = Uri::Parse(request);

          String header = u.Path.c_str();
          if (header == "/options")
          {
            if (u.QueryString.length() > 0)
            {
              std::vector<std::tuple<String, uint32_t>> strArr = getParams(u.QueryString);
              String s;
              for (std::vector<std::tuple<String, uint32_t>>::iterator it = strArr.begin(); it != strArr.end(); ++it)
              {
                s = std::get<0>(*it);
                if (s == "delay")
                  userDelay = std::get<1>(*it);
                else if (s == "brightness")
                  userBrightness = std::get<1>(*it);
                else if (s == "rbrightness")
                  userBrightness = (uint16_t)userBrightness * (100 + (int32_t)std::get<1>(*it)) / 100;
                else if (s == "color")
                  userColor = std::get<1>(*it);
              }
            }
          }
          else if (header == "/h")
          {
            disableAll();
            white = true;
            if (step < 200)
              step += 10;
          }
          else if (header == "/l")
          {
            disableAll();
            white = true;
            if (step >= 10)
              step -= 10;
            else
              white = false;
          }
          else if (header == "/rgb")
          {
            disableAll();
            rgb = !rgb;
          }
          else if (header == "/strobo")
          {
            disableAll();
            strobo = !strobo;
          }
          else if (header == "/wander")
          {
            disableAll();
            wanderLedOn = !wanderLedOn;
          }
          else if (header == "/reverse")
          {
            reverseMode = !reverseMode;
          }
          else if (header == "/rgbwander")
          {
            disableAll();
            rgbWanderColorLedOn = !rgbWanderColorLedOn;
          }
          else if (header == "/rgbcycle")
          {
            disableAll();
            pixelColor.r = 255;
            redDown = true;
            rgbCycleColorLedOn = !rgbCycleColorLedOn;
          }
          else if (header == "/single")
          {
            disableAll();
            singleColorLedOn = !singleColorLedOn;
          }
          else if (header == "/off")
            disableAll();

          break;
        }
      }
    }
    delay(10);
  }
}
void setup()
{
  Serial.begin(921600);
  pinMode(4, OUTPUT);

  digitalWrite(4, HIGH);
  if (digitalLeds_initStrands(&pStrand, 1))
  {
    while (true)
    {
    };
  }
  digitalLeds_resetPixels(&pStrand);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10);
  }

  server.begin();

  xTaskCreatePinnedToCore(
      ledStripUpdateTask,   /* Function to implement the task */
      "ledStripUpdateTask", /* Name of the task */
      10000,                /* Stack size in words */
      NULL,                 /* Task input parameter */
      2,                    /* Priority of the task */
      NULL,                 /* Task handle. */
      1);                   /* Core where the task should run */

  xTaskCreatePinnedToCore(
      wifiServerTask,   /* Function to implement the task */
      "wifiServerTask", /* Name of the task */
      10000,            /* Stack size in words */
      NULL,             /* Task input parameter */
      1,                /* Priority of the task */
      NULL,             /* Task handle. */
      0);               /* Core where the task should run */

  ArduinoOTA.setHostname("SK6812 ESP");
  ArduinoOTA.setPasswordHash("");

  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      });

  ArduinoOTA.begin();
}

void loop()
{
  ArduinoOTA.handle();
  delay(200);
}