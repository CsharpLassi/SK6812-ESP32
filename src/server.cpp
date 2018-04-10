#include "includes/server.h"
#include "includes/led_strip.h"

#include <WiFi.h>
#include <uri_parser.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <inttypes.h>
#include <stdio.h>

using namespace std;

WiFiServer server(server_port);

void initServer()
{
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10);
  }

  server.begin();
}

void serverTask(void *pvParams)
{
  initServer();

  while (true)
  {
    WiFiClient client = server.available();

    if (client)
    {
      string currentLine = "";
      while (client.connected())
      {
        if (client.available())
        {
          string request = "";
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
          transform(request.begin(), request.end(), request.begin(), ::tolower);

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();


          client.print(" <a href=\"/on\">On</a><br />");
          client.print(" <a href=\"/off\">Off</a><br />");

          //client.print(" <a href=\"/H\">Click here to to turn the LED on pin 5 on.</a><br>");
          //client.print(" <a href=\"/L\">Click here to to turn the LED on pin 5 off.</a><br>");
          //client.print(" <a href=\"/RGB\">Click here to to RGB.</a><br>");
          //client.print(" <a href=\"/STROBO\">Click here to to STROBO.</a><br>");
          //client.print(" <a href=\"/WANDER\">Click here to to turn WANDER.</a><br>");
          //client.print(" <a href=\"/rgbwander\">Click here to to turn RGBWANDER.</a><br>");
          //client.print(" <a href=\"/rgbcycle\">Click here to to turn RGBCYCLE.</a><br>");
          //client.print(" <a href=\"/single\">Click here to to turn USERCOLOR.</a><br>");
          //client.print(" <a href=\"/reverse\">Click here to to turn REVERSE.</a><br>");
          //client.print(" <a href=\"/OFF\">Click here to  to turn OFF.</a><br>");

          client.println();

          Uri u = Uri::Parse(request);

          String header = u.Path.c_str();

          if (header == "/on")
          {
            setOn();
          }
          else if (header == "/off")
          {
            setOff();
          }

          break;
        }
      }
    }
    delay(10);
  }
}

vector<tuple<String, uint32_t>> getParams(string s)
{
vector<tuple<String, uint32_t>> params;
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
    val = strtoul(strVal.substring(1).c_str(), NULL, 16);
  else
    val = strtoul(strVal.c_str(), NULL, 10);

  params.push_back(make_tuple(paramName, val));
}
return params;
}
