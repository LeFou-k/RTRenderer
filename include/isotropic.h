//
// Created by VRLAB on 2020/12/3.
//

#ifndef ISOTROPIC_H
#define ISOTROPIC_H
#include "material.h"
#include "texture.h"
#include "solid_color.h"
#include "hittable.h"

class isotropic : public material{
public:
    isotropic(color c): albedo(make_shared<solid_color>(c)){}
    isotropic(shared_ptr<texture> a): albedo(a) {}
    virtual bool scatter(const Ray &ray_in, const hit_record &rec, scatter_record &srec) const override{
//        scatter = Ray(rec.p, random_in_unit_sphere(), ray_in.time());
//        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
private:
    shared_ptr<texture> albedo;
};
#endif //RAYTRACINGTHENEXTWEEK_ISOTROPIC_H
