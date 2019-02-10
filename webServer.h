// ***************
// Configurable constants!!!
// ***************
#define SERVICE_PORT 80
#define WIFI_SSID "<omitted>"
#define WIFI_PASSWORD "<omitted>"
// ***************

#ifndef __WEB_SERVER_H_
#define __WEB_SERVER_H_

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include "pixelManager.h"
#include "router.h"

WiFiServer http_server(SERVICE_PORT);
Router router;

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

      client.setTimeout(1000); // default is 1000
      
      // If we reach here, we've got a new client connection!
      String resource = client.readStringUntil('\r');
      route_info rinf = router.parse(resource);

      // drain the buffer
      while (client.available()) client.read();

      // respond with some nonsense
      client.print(F("HTTP/1.1 200 OK\r\nContent-Type: application/json;\r\n\r\n{\"alive\":\"true\"}\r\n"));

      // actually process request
      
      // /led/index/color/r/g/b
      // /led/index/visibility/1
      // /led/all/color/r/g/b
      // /led/all/visibility/1
      // /system/brightness/250
      // /system/mode/Rainbow
      if (resource.indexOf(F("/led/all/color/")) != -1) {
        if (rinf.paths == 6) {
          // good!
          short r = atoi(rinf.components[3].c_str());
          short g = atoi(rinf.components[4].c_str());
          short b = atoi(rinf.components[5].c_str());

          int i = 0;
          const int total = this->pixelManager->getSize();
          for (; i < total; i++) {
            pixelManager->setPixel(i, r, g, b);
          }

          // Set mode to DEFAULT
          this->pixelManager->setMode(Pulse);
        }
      } else if (resource.indexOf(F("/led/all/visibility/")) != -1) {
        if (rinf.paths == 4) {
          bool visible = rinf.components[3].equals("1");
          int i = 0;
          const int total = this->pixelManager->getSize();
          for (; i < total; i++) {
            this->pixelManager->setPixelVisibility(i,visible);
          }
        }
      } else if (resource.indexOf(F("/system/brightness/")) != -1) {
        if (rinf.paths == 3) {
          // /system/brightness/130
          short brightness = rinf.components[2].toInt();
          this->pixelManager->setBrightness(brightness);
        }
      } else if (resource.indexOf(F("/system/mode/")) != -1) {
        if (rinf.paths == 3) { 
          // /system/mode/Rainbow 
          String system_mode = rinf.components[2];
          if (system_mode.equals("Rainbow")) {
            this->pixelManager->setMode(Rainbow);
          } else if (system_mode.equals("Pulse")) {
            this->pixelManager->setMode(Pulse);
          } else {
            this->pixelManager->setMode(Default);
          }
        }
      } else if (resource.indexOf(F("/led/")) != -1) {
        // Individual lights
        // - Supported Routes -
        // /led/index/color/r/g/b
        // /led/index/visibility/1 or 0
        
        if (rinf.paths > 3) {
          // Check for known sub-route
          String sub_route = rinf.components[2];
          if (sub_route.equals("color") && rinf.paths == 6) {
            int index = rinf.components[1].toInt();
            short r = atoi(rinf.components[3].c_str());
            short g = atoi(rinf.components[4].c_str());
            short b = atoi(rinf.components[5].c_str());            
            this->pixelManager->setPixel(index, r, g, b);
          } else if (sub_route.equals("visibility") && rinf.paths == 4) {
            int index = rinf.components[1].toInt();
            short visibility = rinf.components[3].toInt();
            this->pixelManager->setPixelVisibility(index, visibility == 1);
          }
        }
        
      }
    }
};

#endif
