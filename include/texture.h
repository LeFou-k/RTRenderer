#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class texture{
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};


#endif