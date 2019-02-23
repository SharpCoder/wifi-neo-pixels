#include "Arduino.h"
#include "pixelMode.h"

#define MIN 40
#define MAX 255

class PulseMode : public PixelMode {
    public:
        PulseMode() {

        }
        
        rgb_t doUpdate(pixel_t* pixel, system_t* sys_config) {
          const float delta = 0.02;
          rgb_t response;
          
          int b1 = sys_config->b1;
          if (b1 == false) {
            // Down
            sys_config->brightness = sys_config->brightness - delta;
          } else {
            // Up
            sys_config->brightness = sys_config->brightness + delta;
          }

          if (sys_config->brightness <= MIN) {
            sys_config->brightness = MIN + delta;
            b1 = true;
          } else if (sys_config->brightness >= MAX) {
            sys_config->brightness = MAX - delta;
            b1 = false;
          }
          
          sys_config->b1 = b1;
          return this->noop(pixel);
        }
};