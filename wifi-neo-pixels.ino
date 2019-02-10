#include <Adafruit_NeoPixel.h>
#include "pixelManager.h"
#include "webServer.h"
#include "router.h"

#define NUM_LEDS 50
#define DATA_PIN 5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_RGB + NEO_KHZ800);
PixelManager pixelManager(&pixels);
WebServer server = WebServer(&pixelManager);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("hello, world!");

  // Connect to WiFi and stage http server
  server.begin();

  // Begin controlling pixel strands
  pixels.begin();

  // Initialize brightness
  pixels.setBrightness(255);

  // Default show
  pixels.show();

  // Begin the routine for controlling the pixels
  pixelManager.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixelManager.loop();
  server.loop();
}