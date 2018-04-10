#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

#include "esp32_digital_led_lib.h"


//WIFI
extern const char *ssid;
extern const char *password;

extern const uint16_t server_port;

extern const uint8_t numberOfLeds;
extern const uint8_t pinNum;


extern strand_t pStrand;

#endif
