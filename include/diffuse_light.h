#ifndef DIFFUSE_LIGHT
#define DIFFUSE_LIGHT

#include "material.h"
#include "texture.h"
#include "solid_color.h"



class diffuse_light : public material{
public:
    diffuse_light(shared_ptr<texture> a): emit(a){}
    diffuse_light(const color& c): emit(make_shared<solid_color>(c)) {}

    virtual bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override{
        return false;
    }

    virtual color emitted(double u, double v, const point3& p) const override{
        return emit->value(u, v, p);
    }
private:
    shared_ptr<texture> emit;
};
#endif