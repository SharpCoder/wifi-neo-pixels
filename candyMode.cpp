#include "Arduino.h"
#include "pixelMode.h"

class CandyMode : public PixelMode {
    public:
        CandyMode() {

        }
        
        rgb_t doUpdate(pixel_t* pixel, system_t* sys_config) {
          const float delta = 0.02;          
          int b1 = sys_config->b1;
          if (b1 == false) {
            // Down
            sys_config->brightness = sys_config->brightness - delta;
          } else {
            // Up
            sys_config->brightness = sys_config->brightness + delta;
          }

          if (sys_config->brightness <= 5) {
            sys_config->brightness = 5 + delta;
            b1 = true;
          } else if (sys_config->brightness >= 250) {
            sys_config->brightness = 250 - delta;
            b1 = false;
          }
          
          sys_config->b1 = b1;
          
          if (pixel->index % 2 == 0) {
            rgb_t next = this->noop(pixel);
            next.r = (next.r + 128) % 256;
            next.g = (next.g + 128) % 256;
            next.b = (next.b + 128) % 256;
            return next;
          } 
          
          return this->noop(pixel);
        }
};
