#ifndef DIFFUSE_LIGHT
#define DIFFUSE_LIGHT

#include "material.h"
#include "texture.h"
#include "solid_color.h"



class diffuse_light : public material{
public:
    diffuse_light(shared_ptr<texture> a): emit(a){}
    diffuse_light(const color& c): emit(make_shared<solid_color>(c)) {}

    virtual bool scatter(const Ray &ray_in, const hit_record &rec, scatter_record &srec) const override{
        return false;
    }

    virtual color emitted(const Ray& ray_in, const hit_record& rec, double u, double v, const point3& p) const override{
        return rec.front_face ? emit->value(u, v, p) : color(0, 0, 0);
    }
private:
    shared_ptr<texture> emit;
};
#endif