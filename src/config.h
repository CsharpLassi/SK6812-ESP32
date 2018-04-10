#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "esp32_digital_led_lib.h"


//WIFI
const char *ssid = "FernFeldFunkerII";
const char *password = "sin(x)/x";

const uint16_t server_port = 80;


const int numberOfLeds = 100;

strand_t pStrand = {.rmtChannel = 0, .gpioNum = 4, .ledType = LED_WS2812_V1, .brightLimit = 255, .numPixels = numberOfLeds, .pixels = nullptr, ._stateVars = nullptr};

#endif