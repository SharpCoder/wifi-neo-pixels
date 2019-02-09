#include "pixelManager.h"
#include "defaultMode.cpp"
#include "blinkMode.cpp"

PixelMode* DEFAULT_MODE = new DefaultMode();
PixelMode* BLINK_MODE = new BlinkMode();

PixelManager::PixelManager(int PIXEL_COUNT, Adafruit_NeoPixel* neoPixels) {
    this->neoPixels = neoPixels;
    this->PIXEL_COUNT = PIXEL_COUNT;
    this->pixels = (pixel_t*)malloc(sizeof(pixel_t) * PIXEL_COUNT);
    this->mode = BLINK_MODE;
    this->changed = true; // update pixels immediately

    // Set some default values for our newly allocated pixels
    for (int i = 0; i < PIXEL_COUNT; i++) {
      this->setPixel(i, 98, 0, 226);
      this->getPixel(i)->delay_max = 1000;
      this->getPixel(i)->visible = true;
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
      delay(1);
    }
    
    this->neoPixels->show();
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
    if (this->mode->doUpdate(pixel)) {
        this->changed = true;
    }
}

void PixelManager::loop() {

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

    if (this->changed) {
        this->changed = false;
        this->render();
    }

    delay(1);
}
