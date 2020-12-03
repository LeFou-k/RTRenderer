#include "../include/yz_rect.h"

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