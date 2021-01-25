#ifndef METAL_H
#define METAL_H
#include "material.h"
#include "rtweekend.h"


class metal : public material{
public:
    metal(const color& a, const double& f): albedo(a), fuzz(f < 1 ? f : 1) {}
    virtual bool scatter(const ray &ray_in, const hit_record &rec, scatter_record &srec) const override;
    virtual double scatter_pdf(const ray &ray_in, const hit_record &rec, const ray &scatter) const override{
        return 1.0;
    }
private:
    color albedo;
    double fuzz;
};
#endif