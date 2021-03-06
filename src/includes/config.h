#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <rgb_order.h>

#include "esp32_digital_led_lib.h"

//WIFI
extern const char *ssid;
extern const char *password;

extern const uint16_t server_port;

extern const uint8_t numberOfLeds;
extern const uint8_t pinNum;

extern const rgb_order order;

extern const uint8_t maxBrightness;
extern strand_t pStrand;


const char *ssid = "FernFeldFunkerII";
const char *password = "sin(x)/x";
const uint16_t server_port = 80;

const uint8_t numberOfLeds = 100;
const uint8_t pinNum = 4;

const uint8_t maxBrightness = 50;

const rgb_order order = rbg;

strand_t pStrand = {.rmtChannel = 0, .gpioNum = pinNum, .ledType = CUSTOM, .brightLimit = 255, .numPixels = numberOfLeds, .pixels = nullptr, ._stateVars = nullptr};


#endif
