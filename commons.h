#ifndef __PIXEL_COMMONS_H_
#define __PIXEL_COMMONS_H_

typedef short color_t;

typedef struct rgb {
  color_t r;
  color_t g;
  color_t b;
} rgb_t;

typedef struct pixel {
    unsigned int index, PIXEL_COUNT;

    unsigned int color, next_color;
    
    color_t r;
    color_t g;
    color_t b;
    color_t r_min;
    color_t g_min;
    color_t b_min;
    color_t r_max;
    color_t g_max;
    color_t b_max;

    bool visible;
    int r1, r2, r3, g1, g2, g3, b1, b2, b3; // Random "registers"
    
    long delay, delay_max, delay_target;
} pixel_t;

typedef struct system_config {
  int r1, r2, r3;
  bool b1;
  float brightness;
  bool visible;
} system_t;

typedef enum display_mode {
    Rainbow,
    Default,
    Blink,
    Candy,
    Pulse
} display_mode;

#endif
