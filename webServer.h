// ***************
// Configurable constants!!!
// ***************
#define SERVICE_PORT 80
#define WIFI_SSID "<redacted>"
#define WIFI_PASSWORD "<redacted>"
// ***************

#ifndef __WEB_SERVER_H_
#define __WEB_SERVER_H_

#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "pixelManager.h"
#include "router.h"

short packet_index = 0;
char packet[80];
char packet_tmp = '\0';
WiFiServer http_server(SERVICE_PORT);
Router router;

int s_i = 0;
short s_r = 0, s_g = 0, s_b = 0, s_bright = 0, s_delay = 0;

class WebServer {
  private:
    PixelManager* pixelManager;
    
  public:
    WebServer(PixelManager* pixelManager) {
      this->pixelManager = pixelManager;
    }

    void begin() {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      http_server.begin();
      while (WiFi.status() != WL_CONNECTED)  delay(500);

      http_server.begin();
      String ipAddr = WiFi.localIP().toString();
      Serial.println("Identified IP address as " + ipAddr);
    }

    void loop() {
      WiFiClient client = http_server.available();
      if (!client) {
        return;
      }

      client.setTimeout(800); // default is 1000
      
      // If we reach here, we've got a new client connection!
      
      // reset variables
      packet_index = 0, s_i = 0, s_r = 0, s_g = 0, s_b = 0, s_bright = 0, s_delay = 0, packet_tmp = '\0';
      
      // delay for reasons unknown. this appears to fix some kind of timeout problem.
      delay(10);
      
      while (client.available() && packet_index < 80) {
        packet_tmp = (char)client.read();
        Serial.print(packet_tmp);
        if (packet_tmp == '\r' || packet_tmp == '\0') break;
        packet[packet_index++] = packet_tmp;
      }

      // drain the buffer
      // note: maybe don't want to do this? could be hacked with infinite buffer size :P
      while (client.connected() && client.available()) {
        Serial.print((char)client.read());
        client.read();
      }
      
      route_info rinf = router.parse(packet);

      // respond with some nonsense
      client.print(F("HTTP/1.1 200 OK\r\nContent-Type: application/json;\r\n\r\n{\"alive\":\"true\"}\r\n"));
      delay(1);
      
      if (client.connected()) {
        client.stop();
      }
      
      Serial.println("[Client Disconnected]");
      
      // actually process request
      Serial.print("request incoming with ");
      Serial.print(rinf.paths);
      Serial.println(" paths");
      Serial.println(packet);
      
      // /led/index/color/r/g/b
      // /led/index/visibility/1
      // /led/all/color/r/g/b
      // /led/all/visibility/1
      // /system/delay/150
      // /system/brightness/250
      // /system/mode/Rainbow
      if (rinf.paths == 6) {
        if (strcmp(rinf.components[0], "led") == 0 && 
            strcmp(rinf.components[1], "all") == 0 &&
            strcmp(rinf.components[2], "color") == 0) {
              
          // good!
          s_r = atoi(rinf.components[3]);
          s_g = atoi(rinf.components[4]);
          s_b = atoi(rinf.components[5]);
          s_i = 0;
          const int total = this->pixelManager->getSize();
          for (; s_i < total; s_i++) {
            this->pixelManager->setPixel(s_i, s_r, s_g, s_b);
          }
        } else if (strcmp(rinf.components[0], "led") == 0 &&
                   strcmp(rinf.components[2], "color") == 0) {

          // good!
          s_i = atoi(rinf.components[1]);
          s_r = atoi(rinf.components[3]);
          s_g = atoi(rinf.components[4]);
          s_b = atoi(rinf.components[5]);
          this->pixelManager->setPixel(s_i, s_r, s_g, s_b);          
        }
      } else if (rinf.paths == 4) {
        if (strcmp(rinf.components[0], "led") == 0 &&
            strcmp(rinf.components[1], "all") == 0 &&
            strcmp(rinf.components[2], "visibility") == 0) {
          
          // Good!
          bool visible = atoi(rinf.components[3]) == 1;
          const int total = this->pixelManager->getSize();
          for (s_i = 0; s_i < total; s_i++) {
           this->pixelManager->setPixelVisibility(s_i,visible);
          }
        } else if (strcmp(rinf.components[0], "led") == 0 &&
                   strcmp(rinf.components[2], "visibility") == 0) {

          // good!
          s_i = atoi(rinf.components[1]);
          this->pixelManager->setPixelVisibility(s_i, atoi(rinf.components[3]) == 1);
        }
      } else if (rinf.paths == 3) {
        if (strcmp(rinf.components[0], "system") == 0 &&
            strcmp(rinf.components[1], "mode") == 0) {

          // good!
          // /system/mode/Rainbow 
          if (strcmp(rinf.components[2], "Rainbow") == 0) {
            this->pixelManager->setMode(Rainbow);
          } else if (strcmp(rinf.components[2], "Pulse") == 0) {
            this->pixelManager->setMode(Pulse);
          } else if (strcmp(rinf.components[2], "Blink") == 0) {
            this->pixelManager->setMode(Blink);
          } else if (strcmp(rinf.components[2], "Candy") == 0) {
            this->pixelManager->setMode(Candy);
          } else {
            this->pixelManager->setMode(Default); 
          }
        } else if (strcmp(rinf.components[0], "system") == 0 &&
                   strcmp(rinf.components[1], "brightness") == 0) {
          // good!
          // /system/brightness/130
          s_bright = atoi(rinf.components[2]);
          this->pixelManager->setBrightness(s_bright);
        } else if (strcmp(rinf.components[0], "system") == 0 &&
                   strcmp(rinf.components[1], "delay") == 0) {
          
          s_delay = atoi(rinf.components[2]);
          s_r = this->pixelManager->getSize();
          for (s_i = 0; s_i < s_r; s_i++) {
            this->pixelManager->setPixelDelay(s_i, s_delay);
          }
        }
      }
      
      // clear out packet (probs not needed)
      for (s_i = 0; s_i < 80; s_i++) {
        packet[s_i] = '\0';
      }
    }
};

#endif
