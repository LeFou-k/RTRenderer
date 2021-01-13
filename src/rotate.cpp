//
// Created by VRLAB on 2020/12/3.
//

#include "../include/rotate.h"

rotate::rotate(shared_ptr<hittable> p, double angle, int axis) : ptr(p), axis(axis), angle(angle) {
    if(axis < 0 || axis > 3) std::cerr << "Wrong rotate axis!" << std::flush;
    hasbox = ptr->bounding_box(0, 1, box);
    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for(int i = 0; i < 2; ++i){
        for(int j = 0; j < 2; ++j){
            for(int k = 0; k < 2; ++k){
                auto x = i * box.min().x() + (1 - i) * box.max().x();
                auto y = j * box.min().y() + (1 - j) * box.max().y();
                auto z = k * box.min().z() + (1 - k) * box.max().z();

                point3 temp = rotate_theta(point3(x, y, z), angle);
                for(int d = 0; d < 3; ++d){
                    min[d] = fmin(min[d], temp[d]);
                    max[d] = fmax(max[d], temp[d]);
                }
            }
        }
    }
    box = aabb(min, max);
}

bool rotate::hit(const Ray &ray, double t_min, double t_max, hit_record &rec) const {
    auto origin = rotate_theta(ray.origin(), -angle);
    auto dir = rotate_theta(ray.direction(), -angle);

    Ray rotate_r(origin, dir, ray.time());
    if(!ptr->hit(rotate_r, t_min, t_max, rec))
        return false;

    rec.p = rotate_theta(rec.p, angle);
    rec.normal = rotate_theta(rec.normal, angle);
    rec.set_face_normal(rotate_r, rec.normal);

    return true;
}

bool rotate::bounding_box(double time0, double time1, aabb &output_box) const {
    output_box = box;
    return hasbox;
}

point3 rotate::rotate_theta(const point3 &point, double _angle) const {
    auto x = point.x(), y = point.y(), z = point.z();
    auto cosine_theta = cos(degree_to_radians(_angle));
    auto sine_theta = sin(degree_to_radians(_angle));
    if(axis == 1) return {cosine_theta * x + sine_theta * z, y, -sine_theta * x + cosine_theta * z};
    else if(axis == 0) return {x, cosine_theta * y - sine_theta * z, sine_theta * y + cosine_theta * z};
    else return {cosine_theta * x - sine_theta * y, sine_theta * x + cosine_theta * y, z};
}