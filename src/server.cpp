#include <server.h>
#include <led_strip.h>
#include <led_color.h>

#include <html/pages.h>

#include <WiFi.h>
#include <uri_parser.h>
#include <vector>
#include <tuple>
#include <inttypes.h>
#include <stdio.h>

using namespace std;

WiFiServer server(server_port);

vector<tuple<String, String>> getParams(string s);

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

          Uri u = Uri::Parse(request);

          String header = u.Path.c_str();

          bool backgroundColorIsSet = false;
          rgbwColor backgroundColor;

          if (u.QueryString.length() > 0)
          {
            vector<tuple<String, String>> strArr = getParams(u.QueryString);
            String s;
            for (vector<tuple<String, String>>::iterator it = strArr.begin(); it != strArr.end(); ++it)
            {
              s = get<0>(*it);
              client.println(s);
              if (s == "backgroundcolor")
              {
                String rawColor = get<1>(*it);
                backgroundColor = parseColor(rawColor);
                backgroundColorIsSet = true;
              }
            }
          }

          if (header == "/index")
          {
            client.print(indexPage);;
          }
          else if (header == "/option")
          {
            if (backgroundColorIsSet)
              setBackgroundColor(backgroundColor);
            client.print(redirectPage);
          }
          else if (header == "/on")
          {
            if (backgroundColorIsSet)
              setOn(backgroundColor);
            else
              setOn();

            client.print(redirectPage);
          }
          else if (header == "/off")
          {
            setOff();
            client.print(redirectPage);
          }
          else
          {
            //client.print(indexPage);
          }

          client.println();

          break;
        }
      }
    }
    delay(10);
  }
}

vector<tuple<String, String>> getParams(string s)
{
vector<tuple<String, String>> params;
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


  String strVal = str.substring(locInStr + 1, locInStr2);

  params.push_back(make_tuple(paramName, strVal));
  /*
  uint32_t val;
  if(strVal[0] == 'x')
    val = strtoul(strVal.substring(1).c_str(), NULL, 16);
  else
    val = strtoul(strVal.c_str(), NULL, 10);
  */

}
return params;
}
