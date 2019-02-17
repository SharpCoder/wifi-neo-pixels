#include "pixelMode.h"

class BlinkMode : public PixelMode {
    private:
      int count = 0;
      
    public:
        BlinkMode() {

        }
        
        rgb_t doUpdate(pixel_t* pixel) {
          bool visible = pixel->g3;
          pixel->visible = visible;
          pixel->g3 = !visible;
          return this->noop(pixel); // did change
        }
};
