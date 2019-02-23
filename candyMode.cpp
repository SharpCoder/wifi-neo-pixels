#include "Arduino.h"
#include "pixelMode.h"

class CandyMode : public PixelMode {
    public:
        CandyMode() {

        }
        
        rgb_t doUpdate(pixel_t* pixel, system_t* sys_config) {
          if (pixel->normalized_index % 2 == 0) {
            rgb_t next = this->noop(pixel);
            next.r = (next.r + 128) % 256;
            next.g = (next.g + 128) % 256;
            next.b = (next.b + 128) % 256;
            return next;
          }          
          return this->noop(pixel);
        }
};