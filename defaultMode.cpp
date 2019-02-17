#include "pixelMode.h"

class DefaultMode : public PixelMode {
    public:
        DefaultMode() {

        }
        
        rgb_t doUpdate(pixel_t* pixel, system_t* system_configuration) {
            return this->noop(pixel); // did not change
        }
};
