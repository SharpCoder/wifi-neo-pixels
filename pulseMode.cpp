#include "Arduino.h"
#include "pixelMode.h"

class PulseMode : public PixelMode {
    public:
        PulseMode() {

        }
        
        bool doUpdate(pixel_t* pixel, system_t* sys_config) {
          const float delta = 0.2;
          int b1 = sys_config->b1;
          if (b1 == false) {
            // Down
            sys_config->brightness = sys_config->brightness - delta;
          } else {
            // Up
            sys_config->brightness = sys_config->brightness + delta;
          }

          if (sys_config->brightness <= 0) {
            sys_config->brightness = 1;
            b1 = true;
          } else if (sys_config->brightness > 255) {
            sys_config->brightness = 255;
            b1 = false;
          }
          
          sys_config->b1 = b1;
          return true;
        }
};
