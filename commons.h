#ifndef __PIXEL_COMMONS_H_
#define __PIXEL_COMMONS_H_

typedef short color_t;

typedef struct pixel {
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
    
    long delay;
    long delay_max;
} pixel_t;

typedef enum display_mode {
    Rainbow,
    Default
} DisplayMode;

#endif
