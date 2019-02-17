#ifndef __PIXEL_MODE_H_
#define __PIXEL_MODE_H_

#include "commons.h"

class PixelMode {
    protected:
      // This method will take a pixel input and return
      // an rgb_t object which represents the current pixel state
      // with no changes applied. Hence noop.
      rgb_t noop(pixel_t* pixel) {
          rgb_t noop;
          noop.r = pixel->r;
          noop.g = pixel->g;
          noop.b = pixel->b;
          return noop; 
      }
  
    public:
        PixelMode() { }

        virtual rgb_t doUpdate(pixel_t* pixel, system_t* sys_config) { 
          return this->doUpdate(pixel); 
        };
        
        virtual rgb_t doUpdate(pixel_t* pixel) { return this->noop(pixel); };
};

#endif
