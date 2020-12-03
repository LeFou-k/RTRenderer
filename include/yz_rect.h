#ifndef YZ_RECT_H
#define YZ_RECT_H
#include "material.h"
#include "hittable.h"


class yz_rect : public hittable{
public:
    yz_rect(){}
    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> mat_ptr):
        y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat_ptr){}

    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
private: 
    double y0, y1, z0, z1, k;
    shared_ptr<material> mat_ptr;
};


#endif