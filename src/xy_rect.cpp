#include "../include/xy_rect.h"
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
