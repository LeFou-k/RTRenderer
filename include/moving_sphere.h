#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H
#include "hittable.h"
#include "rtweekend.h"
class moving_sphere : public hittable{
public:
    moving_sphere() {}
    moving_sphere(point3 cen0, point3 cen1, double tm0, double tm1, double r, shared_ptr<material> m_ptr)
    : center0(cen0), center1(cen1), time0(tm0), time1(tm1), radius(r), mat_ptr(m_ptr){}
    point3 center(double time) const;
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

private:
    point3 center0, center1;
    double time0, time1;
    double radius;
    shared_ptr<material> mat_ptr;
};

#endif