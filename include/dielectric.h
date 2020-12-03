#ifndef DIELECTRIC_H
#define DIELECTRIC_H
#include "material.h"


class dielectric : public material{
public:
    dielectric(const double index_of_refraction): ir(index_of_refraction){}
    bool scatter(const Ray& ray_in, const hit_record& rec, color& attenuation, Ray& scatter) const;
private:
    double ir;
    static double reflectance(double cosine, double ref_idx);
};
#endif