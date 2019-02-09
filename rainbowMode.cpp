#include "Arduino.h"
#include "commons.h"
#include "pixelMode.h"

class RainbowMode : public PixelMode {
    private:
      rgb_t Wheel(unsigned int WheelPos) {
        WheelPos = 255 - WheelPos;        
        rgb_t response;
        
        if(WheelPos < 85) {
          response.r = 255 - (WheelPos * 3);
          response.g = 0;
          response.b = WheelPos * 3;
          return response;
        }
        
        if(WheelPos < 170) {
          WheelPos -= 85;
          response.r = 0;
          response.g = WheelPos * 3;
          response.b = 255 - WheelPos * 3;
          return response;
        }
        
        WheelPos -= 170;
        response.r = WheelPos * 3;
        response.g = 255 - WheelPos * 3;
        response.b = 0;
        return response;
      }

    public:
        RainbowMode() {

        }
        
        bool doUpdate(pixel_t* pixel) {
          int i = pixel->index;
          int j = pixel->g3;
          
          rgb_t rgb = this->Wheel((((i * 256) / (pixel->PIXEL_COUNT - 1)) + j) & 255);
          
          pixel->r = rgb.r;
          pixel->g = rgb.g;
          pixel->b = rgb.b;

          j = j + 1;
          if (j >= 256 * 5) {
            j = 0;
          }
          
          pixel->g3 = j;
          return true;
        }
};
