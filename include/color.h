#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtweekend.h"
#include <fstream>

void write_color(FILE* fp, color pixel_color, int samples_per_pixel)
{
    //Divide color by the number of samples;
    auto scale = 1.0 / samples_per_pixel; //multiple is faster than division
    // r *= scale;
    // g *= scale;
    // b *= scale;

    pixel_color = sqrt(pixel_color * scale);
    //write the translated[0, 255] value of each color component
    
    static unsigned char w_color[3];
    w_color[0] = (unsigned char)(256 * clamp(pixel_color.x(), 0.0, 0.999));
    w_color[1] = (unsigned char)(256 * clamp(pixel_color.y(), 0.0, 0.999));
    w_color[2] = (unsigned char)(256 * clamp(pixel_color.z(), 0.0, 0.999));

    
    fwrite(w_color, 1, 3, fp);
}

#endif