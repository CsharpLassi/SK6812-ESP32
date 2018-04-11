#include "includes/config.h"
#include "includes/server.h"
#include "includes/led_strip.h"

#include "esp32_digital_led_lib.h"


#include <ArduinoOTA.h>

using namespace std;

void setup()
{
  Serial.begin(921600);

  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);


  xTaskCreatePinnedToCore(
      ledStripUpdateTask,   // Function to implement the task
      "ledStripUpdateTask", // Name of the task
      10000,                // Stack size in words
      NULL,                // Task input parameter
      2,                   // Priority of the task
      NULL,                // Task handle.
      0);                  // Core where the task should run

  xTaskCreatePinnedToCore(
      serverTask,   // Function to implement the task
      "wifiServerTask", // Name of the task
      10000,            // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle.
      0);               // Core where the task should run




  ArduinoOTA.setHostname("SK6812 ESP");
  ArduinoOTA.setPasswordHash("");
  ArduinoOTA.setPort(3232);

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

  digitalWrite(22, HIGH);
}

void loop()
{
  //ledStripUpdateTask(NULL);
  ArduinoOTA.handle();
  delay(200);
}
