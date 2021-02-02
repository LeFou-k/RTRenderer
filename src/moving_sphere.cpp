#include "../include/moving_sphere.h"

point3 moving_sphere::center(double time) const{
    return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
}

bool moving_sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    auto oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto delta = b * b - a * c;
    if(delta < 0) return false;
    auto root = (-b - sqrt(delta)) / a;
    if(root < t_min || root > t_max)
    {
        root = (-b + sqrt(delta)) / a;
        if(root < t_min || root > t_max) 
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    auto outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool moving_sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
    aabb box0(center(time0) - vec(radius, radius, radius),
              center(time0) + vec(radius, radius, radius));
    
    aabb box1(center(time1) - vec(radius, radius, radius),
              center(time1) + vec(radius, radius, radius));
    
    output_box = surrounding_box(box0, box1);
    return true;
}