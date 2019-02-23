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
        
        rgb_t doUpdate(pixel_t* pixel) {
          int i = pixel->normalized_index;
          int j = pixel->g3;
          
          rgb_t rgb = this->Wheel((((i * 255) / (pixel->normalized_pixel_count - 1)) + j) & 255);
          
          j = j + 1;
          if (j >= 255 * 5) {
            j = 0;
          }
          
          pixel->g3 = j;
          return rgb;
        }
};