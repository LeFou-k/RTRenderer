#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"


struct hit_record;
class material{
public:
    //ray in, hit_record for input, attenuation and scatter for output
    virtual bool scatter(const Ray& ray_in, const hit_record& rec, color& attenuation, Ray& scatter) const = 0;
    virtual color emitted(double u, double v, const point3& p) const {
        return color(0, 0, 0);
    }
};
#endif