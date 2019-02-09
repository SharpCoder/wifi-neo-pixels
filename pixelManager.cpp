#include "Arduino.h"
#include "pixelManager.h"
#include "defaultMode.cpp"
#include "blinkMode.cpp"
#include "rainbowMode.cpp"
#include "pulseMode.cpp"

PixelMode* DEFAULT_MODE = new DefaultMode();
PixelMode* BLINK_MODE = new BlinkMode();
PixelMode* RAINBOW_MODE = new RainbowMode();
PixelMode* PULSE_MODE = new PulseMode();

PixelManager::PixelManager(int PIXEL_COUNT, Adafruit_NeoPixel* neoPixels) {
    this->neoPixels = neoPixels;
    this->PIXEL_COUNT = PIXEL_COUNT;
    this->pixels = (pixel_t*)malloc(sizeof(pixel_t) * PIXEL_COUNT);
    this->systemConfiguration = new system_t();
}

void PixelManager::begin() {
  
  Serial.println("rebooted");
  this->mode = PULSE_MODE;
  this->changed = true; // update pixels immediately
  // Set some default values for our newly allocated pixels
  this->systemConfiguration->brightness = 255;
  
  for (int i = 0; i < PIXEL_COUNT; i++) {
    pixel_t* pixel = this->getPixel(i);
    pixel->index = i;
    pixel->delay_max = 25;
    pixel->visible = true;
    pixel->PIXEL_COUNT = PIXEL_COUNT;
    
    this->setPixel(i, 15, 15, 180);
  }
}

void PixelManager::render() {
    for(int index = 0; index < this->PIXEL_COUNT; index++) {
      pixel_t* pixel = this->getPixel(index);
      if (pixel->visible == false) {
        this->neoPixels->setPixelColor(index, this->neoPixels->Color(0, 0, 0));
      } else {
        this->neoPixels->setPixelColor(index, this->neoPixels->Color(pixel->r, pixel->g, pixel->b));
      }
    }

    this->neoPixels->setBrightness(this->systemConfiguration->brightness);
    this->neoPixels->show();
    delay(2);
}

pixel_t* PixelManager::getPixel(int index) {
    return &*(this->pixels + index);
}

void PixelManager::setPixel(int index, color_t red, color_t g, color_t b) {
    pixel_t* pixel = this->getPixel(index);
    pixel->r = red;
    pixel->g = g;
    pixel->b = b;
    this->changed = true;
}

void PixelManager::setMode(DisplayMode mode) {
  
  // Reset all the registers and other attributes on the pixel models.
  for (int index = 0; index < this->PIXEL_COUNT; index++) {
    pixel_t* pixel = this->getPixel(index);
    pixel->r1 = 0;
    pixel->r2 = 0;
    pixel->r3 = 0;
    pixel->g1 = 0;
    pixel->g2 = 0;
    pixel->g3 = 0;
    pixel->b1 = 0;
    pixel->b2 = 0;
    pixel->b3 = 0;
    pixel->visible = true;  
  }
  
  if (mode == Rainbow) {
      this->mode = DEFAULT_MODE;
  } else {
      this->mode = DEFAULT_MODE;
  }

  this->changed = true;
}

void PixelManager::doUpdate(pixel_t* pixel) {
    if (this->mode->doUpdate(pixel, this->systemConfiguration)) {
        this->changed = true;
    }
}

void PixelManager::loop() {

    if (this->changed) {
        this->changed = false;
        this->render();
    }

    // Iterate over each pixel, increment the delay counter 
    // and determine whether it needs updated or not.
    for (int index = 0; index < this->PIXEL_COUNT; index++) {
        pixel_t* pixel = this->getPixel(index);
        if (pixel->delay_max > 0) {
            pixel->delay = pixel->delay + 1;
            if (pixel->delay >= pixel->delay_max) {
                pixel->delay = 0;
                this->doUpdate(pixel);
            }
        }
    }
    
    delay(2);
}
