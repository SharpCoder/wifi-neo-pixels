#include "pixelManager.h"
#include "defaultMode.cpp"

PixelMode* DEFAULT_MODE = new DefaultMode();

PixelManager::PixelManager(int PIXEL_COUNT, Adafruit_NeoPixel* neoPixels) {
    this->neoPixels = neoPixels;
    this->PIXEL_COUNT = PIXEL_COUNT;
    this->pixels = (pixel_t*)malloc(sizeof(pixel_t) * PIXEL_COUNT);
    this->mode = DEFAULT_MODE;
    this->changed = true; // update pixels immediately

    // Set some default values for our newly allocated pixels
    for (int i = 0; i < PIXEL_COUNT; i++) {
      this->setPixel(i, 0, 0, 255);
    }
}

void PixelManager::render() {
    for(int index = 0; index < this->PIXEL_COUNT; index++) {
      pixel_t* pixel = this->getPixel(index);
      this->neoPixels->setPixelColor(index, this->neoPixels->Color(pixel->g, pixel->r, pixel->b));
    }
    this->neoPixels->show();
}

pixel_t* PixelManager::getPixel(int index) {
    return &*(this->pixels + index);
}

void PixelManager::setPixel(int index, color_t r, color_t g, color_t b) {
    pixel_t* pixel = this->getPixel(index);
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
    this->changed = true;
}

void PixelManager::setMode(DisplayMode mode) {
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
}
