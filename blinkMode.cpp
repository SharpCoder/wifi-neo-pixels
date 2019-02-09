#include "pixelMode.h"

class BlinkMode : public PixelMode {
    private:
      int count = 0;
      
    public:
        BlinkMode() {

        }
        
        bool doUpdate(pixel_t* pixel) {
          bool visible = pixel->g3;
          
          if (visible) {
            pixel->r1 = pixel->r;
            pixel->g1 = pixel->g;
            pixel->b1 = pixel->b;
            pixel->r = 0;
            pixel->g = 0;
            pixel->b = 0;
          } else {
            if (pixel->r == 0) {
              pixel->r = pixel->r1;
            }
            if (pixel->g == 0) {
              pixel->g = pixel->g1;
            }
            if (pixel->b == 0) {
              pixel->b = pixel->b1;
            }
          }
          
          pixel->g3 = !visible;
          return true; // did change
        }
};
