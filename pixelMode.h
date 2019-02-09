#ifndef __PIXEL_MODE_H_
#define __PIXEL_MODE_H_

#include "commons.h"

class PixelMode {
    public:
        PixelMode() { }
        virtual bool doUpdate(pixel_t* pixel, system_t* sys_config) { return this->doUpdate(pixel); };
        virtual bool doUpdate(pixel_t* pixel) { return false; };
};

#endif
