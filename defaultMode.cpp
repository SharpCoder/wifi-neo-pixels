#include "pixelMode.h"

class DefaultMode : public PixelMode {
    public:
        DefaultMode() {

        }
        
        bool doUpdate(pixel_t* pixel) {
            return false; // did not change
        }
};
