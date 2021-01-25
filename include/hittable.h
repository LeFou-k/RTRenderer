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
    inline void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal: -outward_normal;
    }
};

class hittable{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
    virtual double pdf_value(const point3& o, const vec3& v) const{ return 0.0; }
    virtual vec3 random(const point3& o) const{ return vec3(1, 0, 0); }
};

class xy_rect : public hittable{
public:
    xy_rect(){}
    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat_ptr):
            x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat_ptr){}
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

private:
    double x0, x1, y0, y1, k;
    shared_ptr<material> mat_ptr;
};

class xz_rect : public hittable{
public:
    xz_rect(){}
    xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> mat_ptr):
            x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat_ptr){}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    virtual double pdf_value(const point3 &o, const vec3 &v) const override;
    virtual vec3 random(const point3 &) const override;
private:
    double x0, x1, z0, z1, k;
    shared_ptr<material> mat_ptr;
};

class yz_rect : public hittable{
public:
    yz_rect(){}
    yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<material> mat_ptr):
            y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat_ptr){}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
private:
    double y0, y1, z0, z1, k;
    shared_ptr<material> mat_ptr;
};
#endif