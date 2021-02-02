#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec.h"

class sphere : public hittable{
public:
    sphere() {}
    sphere(point3 c, double r, shared_ptr<material> m): center(c), radius(r), mat_ptr(m){}
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual double pdf_value(const point3 &o, const vec &v) const override;
    virtual vec random(const point3 &o) const override;
private:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
    static void get_sphere_uv(const point3& p, double& u, double& v);
};

#endif