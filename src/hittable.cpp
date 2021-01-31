//
// Created by LiuKe on 2021/1/25.
//
#include "../include/hittable.h"

bool xy_rect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
    double inD = 1.0 / r.direction().z();
    double t = (k - r.origin().z()) * inD;

    if(t < t_min || t > t_max) return false;
    auto p = r.at(t);
    double x = p.x(), y = p.y();
    if(x < x0 || x > x1 || y < y0 || y > y1) return false;


    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.p = p;
    rec.t = t;

    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool xy_rect::bounding_box(double time0, double time1, aabb& output_box) const{
    output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
    return true;
}


bool xz_rect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
    double inD = 1.0 / r.direction().y();
    double t = (k - r.origin().y()) * inD;

    if(t < t_min || t > t_max) return false;

    auto p = r.at(t);
    double x = p.x(), z = p.z();

    if(x < x0 || x > x1 || z < z0 || z > z1) return false;

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.p = p;
    rec.t = t;

    auto outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool xz_rect::bounding_box(double time0, double time1, aabb& output_box) const{
    output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
    return true;
}

//origin为视线与该平面的交点
double xz_rect::pdf_value(const point3 &origin, const vec3 &v) const {
    hit_record rec;
    if(!this->hit(Ray(origin, v), 0.001, infinity, rec))
        return 0.0;
    double area = (x1 - x0) * (z1 - z0);
    auto dis_square = rec.t * rec.t * v.length_squared();
    auto cosine = fabs(dot(rec.normal, unit_vector(v)));
    return dis_square / cosine / area;
}

vec3 xz_rect::random(const point3 &origin) const {
    return point3(random_double(x0, x1), k, random_double(z0, z1)) - origin;
}

bool yz_rect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
    double inD = 1.0 / r.direction().x();
    double t = (k - r.origin().x()) * inD;

    if(t < t_min || t > t_max) return false;
    auto p = r.at(t);
    double y = p.y(), z = p.z();
    if(y < y0 || y > y1 || z < z0 || z > z1) return false;

    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.p = p;
    rec.t = t;

    auto outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool yz_rect::bounding_box(double time0, double time1, aabb& output_box) const{
    output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
    return true;
}