//
// Created by VRLAB on 2020/12/3.
//

#ifndef TRANSLATE_H
#define TRANSLATE_H
#include "hittable.h"

class translate : public hittable{
public:
    translate(shared_ptr<hittable> p, const vec3& displacement): ptr(p), offset(displacement){}
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
private:
    shared_ptr<hittable> ptr;
    vec3 offset;
};
#endif //RAYTRACINGTHENEXTWEEK_TRANSLATE_H
