#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H

#include "rtweekend.h"
#include "perlin.h"
#include "texture.h"

#include <iostream>

class image_texture: public texture{
public:
    const int bytes_per_pixel = 3;
    image_texture(): data(nullptr), width(0), height(0){}
    image_texture(const char* filename);
    ~image_texture(){ delete data; }
    virtual color value(double u, double v, const point3& p) const override;
private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;
};
#endif