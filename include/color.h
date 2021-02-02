#ifndef COLOR_H
#define COLOR_H

#include "vec.h"
#include "rtweekend.h"
#include <fstream>

void write_color(FILE* fp, color pixel_color, int samples_per_pixel)
{
    //Divide color by the number of samples;
    auto scale = 1.0 / samples_per_pixel; //multiple is faster than division
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    if(r != r) r = 0.0;
    if(g != g) g = 0.0;
    if(b != b) b = 0.0;

    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    //write the translated[0, 255] value of each color component
    
    static unsigned char w_color[3];
    w_color[0] = (unsigned char)(256 * clamp(r, 0.0, 0.999));
    w_color[1] = (unsigned char)(256 * clamp(g, 0.0, 0.999));
    w_color[2] = (unsigned char)(256 * clamp(b, 0.0, 0.999));

    
    fwrite(w_color, 1, 3, fp);
}

#endif