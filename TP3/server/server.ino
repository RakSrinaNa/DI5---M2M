//90-a2-da-0f-f9-fd
#include <SPI.h>
#include <Ethernet2.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "rgb_lcd.h"

#define SEALEVELPRESSURE_HPA (1013.25)

EthernetServer server(80);

Adafruit_BME280 bme; // I2C

void setup()
{
  byte ip[] = {192, 168, 0, 1};
  byte mac[] = {0x90, 0xa2, 0xda, 0x0f, 0xf9, 0xfd};
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  bme.begin();
}

void loop()
{
  EthernetClient client = server.available();
  if (client)
  {
    bool currentLineIsBlank = true;

    char method[8] = {0};
    int methodCur = 0;
    bool methodCheck = false;

    char path[128] = {0};
    int pathCur = 0;
    bool pathCheck = false;

    while (client.connected())
    {
      char c = client.read();
      Serial.write(c);

      if (c == ' ' && !methodCheck)
      {
        methodCheck = true;
      }
      else if (!methodCheck)
      {
        method[methodCur++] = c;
      }

      else if (c == ' ' && !pathCheck)
      {
        pathCheck = true;
      }
      else if (!pathCheck)
      {
        path[pathCur++] = c;
      }

      if (c == '\n' && currentLineIsBlank)
      {
        if (strcmp(method, "GET") == 0)
        {
          if (strcmp(path, "/temperature") == 0)
          {
            printInfo(client, "Temperature", getTemp());
          }
          else if (strcmp(path, "/humidity") == 0)
          {
            printInfo(client, "Humidity", getHumid());
          }
          else if (strcmp(path, "/pressure") == 0)
          {
            printInfo(client, "Pressure", getPres());
          }
          else if (strcmp(path, "/altitude") == 0)
          {
            printInfo(client, "Altitude", getAlt());
          }
          else if (strstr(path, "/rgb?") != null)
          {
            char *r = strstr(path, "?") + 1;
          }
          else
          {
            printMainPage(client);
          }
        }
        else if (strcmp(method, "POST") == 0)
        {
          if (strstr(path, "/rgb?") != null)
          {
          }
          printMainPage(client);
        }
        else
        {
          printMainPage(client);
        }

        break;
      }
      if (c == '\n')
        currentLineIsBlank = true;
      else if (c != '\r')
        currentLineIsBlank = false;
    }
    delay(1);
    client.stop();
    Serial.println("Disconnected");
  }
}

float getTemp()
{
  return bme.readTemperature();
}

float getHumid()
{
  return bme.readHumidity();
}

float getPres()
{
  return bme.readPressure() / 100.0F;
}

float getAlt()
{
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}

void printMainPage(EthernetClient client)
{
  Serial.println("Printing main page");
  client.println("<html><head><title>Main</title></head><body><div><h3>GET</h3><a href=\"/temperature\">Temperature</a><br /><a href=\"/humidity\">Humidity</a><br /><a href=\"/pressure\">Pressure</a><br /><a href=\"/altitude\">Altitude</a></div><hr /><div><h3>POST</h3><form action=\"/rgb\" method=\"POST\">R:<br><input type=\"text\" name=\"R\"><br>G:<br><input type=\"text\" name=\"G\"><br>B:<br><input type=\"text\" name=\"B\"><br><input type=\"submit\" value=\"Submit\"></form></div></body></html>");
}

void printInfo(EthernetClient client, char *title, float value)
{
  Serial.println("Printing info page");
  client.print("<html><head><title>");
  client.print(title);
  client.print("</title></head><body><div>");
  client.print(title);
  client.print(" is ");
  client.print(value);
  client.println("</div></body></html>");
}
