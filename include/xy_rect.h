#ifndef XY_RECT_H
#define XY_RECT_H
#include "material.h"
#include "hittable.h"

class xy_rect : public hittable{
public:
    xy_rect(){}
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat_ptr): 
        x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat_ptr){}
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

private:
    double x0, x1, y0, y1, k;
    shared_ptr<material> mat_ptr;
};

#endif