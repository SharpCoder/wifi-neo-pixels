#include <Adafruit_NeoPixel.h>
#include "pixelManager.h"

#define NUM_LEDS 50
#define DATA_PIN 13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
PixelManager pixelManager(NUM_LEDS, &pixels);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixelManager.loop();
  delay(10);  
}
