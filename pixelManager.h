#ifndef __PIXEL_MANAGER_H_
#define __PIXEL_MANAGER_H_

#include <Adafruit_NeoPixel.h>
#include "commons.h"
#include "pixelMode.h"

class PixelManager {
    private:
        Adafruit_NeoPixel* neoPixels;
        PixelMode* mode;
        int PIXEL_COUNT;
        pixel_t* pixels;
        system_t* systemConfiguration;
        bool changed;
        void render();
        void doUpdate(pixel_t* pixel);

    public:
        PixelManager(int PIXEL_COUNT, Adafruit_NeoPixel* neoPixels);
        void setPixel(int index, color_t r, color_t g, color_t b);
        void setMode(DisplayMode mode);
        pixel_t* getPixel(int index);
        void loop();
        void begin();
};

#endif
