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

PixelManager::PixelManager(Adafruit_NeoPixel* neoPixels) {
    this->neoPixels = neoPixels;
    this->PIXEL_COUNT = neoPixels->numPixels();
    this->pixels = (pixel_t*)malloc(sizeof(pixel_t) * PIXEL_COUNT);
    this->systemConfiguration = new system_t();
}

long PixelManager::calcNextTime(long delay) {
    return millis() + delay;
}

void PixelManager::begin() {
  this->mode = DEFAULT_MODE;
  this->changed = true; // update pixels immediately
  // Set some default values for our newly allocated pixels
  this->systemConfiguration->brightness = 255;
  
  for (int i = 0; i < PIXEL_COUNT; i++) {
    pixel_t* pixel = this->getPixel(i);
    pixel->index = i;
    pixel->color = -1;
    pixel->delay_max = 35;
    pixel->visible = true;
    pixel->PIXEL_COUNT = PIXEL_COUNT;
    
    this->setPixel(i, 200, 0, 175);
  }
}

void PixelManager::render() {
  bool shouldUpdate = false;
  for(int index = 0; index < this->PIXEL_COUNT; index++) {
    pixel_t* pixel = this->getPixel(index);
    if (pixel->visible == false) {
      this->neoPixels->setPixelColor(index, this->neoPixels->Color(0, 0, 0));
      shouldUpdate = true;
      delay(1);
    } else {
      // compute the next color and only update if we are actually changing some aspect of it.
      pixel->next_color = this->neoPixels->Color(pixel->r, pixel->g, pixel->b);
      if (pixel->next_color != pixel->color || true) {
        shouldUpdate = true;
        this->neoPixels->setPixelColor(index, pixel->next_color);
        pixel->color = pixel->next_color;
      }
    }
  }
  
  
  this->neoPixels->setBrightness(this->systemConfiguration->brightness);
  this->neoPixels->show();
  }

pixel_t* PixelManager::getPixel(int index) {
    return &*(this->pixels + index);
}

int PixelManager::getSize() {
  return this->PIXEL_COUNT;
}

void PixelManager::setPixel(int index, color_t r, color_t g, color_t b) {
    pixel_t* pixel = this->getPixel(index);
    pixel->r = r % 256;
    pixel->g = g % 256;
    pixel->b = b % 256;
    this->changed = true;
}

void PixelManager::setPixelVisibility(int index, bool visible) {
  pixel_t* pixel = this->getPixel(index);
  pixel->visible = visible;
  this->changed = true;
}

void PixelManager::setBrightness(short brightness) {
  this->systemConfiguration->brightness = brightness;
  this->changed = true;
}

void PixelManager::setMode(display_mode mode) {
  
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
    this->mode = RAINBOW_MODE;
  } else if (mode == Pulse) {
    this->mode = PULSE_MODE;
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
    long ms = millis();
    for (int index = 0; index < this->PIXEL_COUNT; index++) {
        pixel_t* pixel = this->getPixel(index);
        if (ms >= pixel->delay_target) {
            pixel->delay_target = this->calcNextTime(pixel->delay_max);
            this->doUpdate(pixel);
        }
    }
}
