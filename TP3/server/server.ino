//90-a2-da-0f-f9-fd
#include <Ethernet2.h>
#include <Adafruit_BME280.h>
#include <ChainableLED.h>

#define SEALEVELPRESSURE_HPA (1013.25)

EthernetServer server(80);

Adafruit_BME280 bme; // I2C
ChainableLED led(3, 4, 1);
byte R = 255;
byte G = 255;
byte B = 255;

void setup()
{
  byte ip[] = {192, 168, 0, 1};
  byte mac[] = {0x90, 0xa2, 0xda, 0x0f, 0xf9, 0xfd};
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());
  bme.begin();
  led.init();
}

void loop()
{
  EthernetClient client = server.available();
  if (client)
  {
    char method[8] = {0};
    int methodCur = 0;
    bool methodCheck = false;

    char path[128] = {0};
    int pathCur = 0;
    bool pathCheck = false;

    char lastLine[128] = {0};
    int lastLineCur = 0;
    int lastLineMax = 128;

    while (client.connected())
    {
      char c = client.read();
     
      if(c == -1)
      {
        Serial.write("\n");
        if (strcmp(method, "GET") == 0)
        {
          if (strcmp(path, "/") == 0)
          {
            printMainPage(client);
          }
          else if (strcmp(path, "/t") == 0)
          {
            printInfo(client, "Temperature", String(bme.readTemperature()));
          }
          else if (strcmp(path, "/h") == 0)
          {
            printInfo(client, "Humidity", String(bme.readHumidity()));
          }
          else if (strcmp(path, "/p") == 0)
          {
            printInfo(client, "Pressure", String(bme.readPressure() / 100.0F));
          }
          else if (strcmp(path, "/a") == 0)
          {
            printInfo(client, "Altitude", String(bme.readAltitude(SEALEVELPRESSURE_HPA)));
          }
          else if (strcmp(path, "/rgb") == 0)
          {
            printInfo(client, "RGB", "R : " + String(R) + " / G : " + String(G) + " / B : " + String(B));
          }
          else
          {
            printInfo(client, "Error", "404");
          }
        }
        else if (strcmp(method, "POST") == 0)
        {
          if (strcmp(path, "/rgb") == 0)
          {
            int rr = -1;
            int gg = -1;
            int bb = -1;
            char * pch;
            pch = strtok (lastLine,"&");
            while (pch != NULL)
            {
              if(strstr(pch, "=") == (pch + 1)){
                if(pch[0] == 'R'){
                  rr = strtol(pch + 2, NULL, 10);
                }
                else if(pch[0] == 'G'){
                  gg = strtol(pch + 2, NULL, 10);
                }
                else if(pch[0] == 'B'){
                  bb = strtol(pch + 2, NULL, 10);
                }
              }
              pch = strtok(NULL, "&");
            }
            if(rr < 0 || gg < 0 || bb < 0 || rr > 255 || gg > 255 || bb > 255){
              printInfo(client, "Error", "402");
            }
            else
            {
               setRGB((byte)rr, (byte)gg, (byte)bb);
               printMainPage(client);
            }
          }
          else{
           printInfo(client, "Error", "404");
          }
        }
        else
        {
          printInfo(client, "Error", "404");
        }
        break;
      }
      else{
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

        if(c == '\n'){
          for(int i = 0; i < lastLineMax; i++)
            lastLine[i] = 0;
          lastLineCur = 0;
        }
        else if(lastLineCur < lastLineMax)
          lastLine[lastLineCur++] = c;
      }
    }
    delay(1);
    client.stop();
    Serial.println("Disconnected");
  }
}

void setRGB(byte rr, byte gg, byte bb){
  R = rr;
  G = gg;
  B = bb;
  led.setColorRGB(0, R, G, B);
}

void printMainPage(EthernetClient client)
{
  Serial.println("Printing main page");
  client.println("<html><body><h3>GET</h3><a href='/t'>Temp</a><br /><a href='/h'>Hum</a><br /><a href='/p'>Press</a><br /><a href='/a'>Alt</a><br /><a href='/rgb'>LED</a><hr /><h3>POST</h3><form action='/rgb' method='POST'>R:<br><input type='text' name='R'><br>G:<br><input type='text' name='G'><br>B:<br><input type='text' name='B'><br><input type='submit' value='Submit'></form></body></html>");
}

void printInfo(EthernetClient client, String title, String value)
{
  Serial.print("Printing info page ");
  client.print("<html><body>");
  client.print(title);
  client.print(" is ");
  client.print(value);
  client.println("<hr/><a href='/'><h1>Back</h1></a></body></html>");
}
