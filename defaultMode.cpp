#include "pixelMode.h"

class DefaultMode : public PixelMode {
    public:
        DefaultMode() {

        }
        
        bool doUpdate(pixel_t* pixel, system_t* system_configuration) {
            return true; // did not change
        }
};
