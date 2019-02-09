#ifndef __PIXEL_MODE_H_
#define __PIXEL_MODE_H_

#include "commons.h"

class PixelMode {
    public:
        PixelMode() { }
        virtual bool doUpdate(pixel_t* pixel) { return false; };
};

#endif
