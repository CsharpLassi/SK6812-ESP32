#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdint.h>

extern const char *ssid;
extern const char *password;
extern const uint16_t server_port;

void initServer();
void serverTask(void *pvParams);

#endif
