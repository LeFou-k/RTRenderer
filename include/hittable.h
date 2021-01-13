#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"


class material;
struct hit_record{
    point3 p;
    vec3 normal;
    double t;    
    bool front_face;
    double u, v;
    shared_ptr<material> mat_ptr;
    inline void set_face_normal(const Ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal: -outward_normal;
    }
};

class hittable{
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
    virtual double pdf_value(const point3& o, const vec3& v) const{ return 0.0; }
    virtual vec3 random(const point3& o) const{ return vec3(1, 0, 0); }
};
#endif