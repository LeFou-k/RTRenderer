#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "rtweekend.h"
#include "material.h"
#include "texture.h"
#include "solid_color.h"


class lambertian : public material{
public:
    lambertian(const color& c): albedo(make_shared<solid_color>(c)) {}
    lambertian(const shared_ptr<texture> a): albedo(a) {}
    virtual bool scatter(const ray &ray_in, const hit_record &rec, scatter_record &srec) const override;
    virtual double scatter_pdf(const ray &ray_in, const hit_record &rec, const ray &scatter) const override;

private:
    shared_ptr<texture> albedo;
};
#endif