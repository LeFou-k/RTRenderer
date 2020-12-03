#ifndef METAL_H
#define METAL_H
#include "material.h"
#include "rtweekend.h"


class metal : public material{
public:
    metal(const color& a, const double& f): albedo(a), fuzz(f < 1 ? f : 1) {}
    bool scatter(const Ray& ray_in, const hit_record& rec, color& attenuation, Ray& scatter) const;
private:
    color albedo;
    double fuzz;
};
#endif