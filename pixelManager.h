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
        long calcNextTime(long delay);

    public:
        PixelManager(Adafruit_NeoPixel* neoPixels);
        void setPixel(int index, color_t r, color_t g, color_t b);
        void setPixelVisibility(int index, bool visible);
        void setMode(display_mode mode);
        void setBrightness(short brightness);
        pixel_t* getPixel(int index);
        int getSize();
        void loop();
        void begin();
};

#endif
