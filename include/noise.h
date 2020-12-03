#ifndef NOISE_H
#define NOISE_H

#include "texture.h"
#include "perlin.h"
class noise_texture : public texture{
public:
    noise_texture(){}
    noise_texture(double _sc): scale(_sc){}
    virtual color value(double u, double v, const point3& p) const override;
private:
    perlin noise;
    double scale;
};
#endif