#ifndef DIELECTRIC_H
#define DIELECTRIC_H
#include "material.h"


class dielectric : public material{
public:
    dielectric(const double index_of_refraction): ir(index_of_refraction){}
    virtual bool scatter(const ray& ray_in, const hit_record& rec, scatter_record& srec) const override;
private:
    double ir;
    static double reflectance(double cosine, double ref_idx);
};
#endif