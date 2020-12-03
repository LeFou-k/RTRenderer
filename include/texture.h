#ifndef TEXTTURE_H
#define TEXTTURE_H
#include "rtweekend.h"

class texture{
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};


#endif