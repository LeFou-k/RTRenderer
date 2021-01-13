#ifndef XZ_RECT_H
#define XZ_RECT_H

#include "material.h"
#include "hittable.h"

class xz_rect : public hittable{
public:
    xz_rect(){}
    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> mat_ptr):
        x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat_ptr){}

    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual double pdf_value(const point3 &o, const vec3 &v) const override;
    virtual vec3 random(const point3 &) const override;
private:
    double x0, x1, z0, z1, k;
    shared_ptr<material> mat_ptr;
};
#endif