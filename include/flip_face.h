//
// Created by VRLAB on 2020/12/7.
//

#ifndef FLIP_FACE_H
#define FLIP_FACE_H
#include "hittable.h"

class flip_face : public hittable{
public:
    flip_face(shared_ptr<hittable> p) : ptr(p){}
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
    virtual double pdf_value(const point3 &o, const vec3 &v) const override{
        return ptr->pdf_value(o, v);
    }
    virtual vec3 random(const point3 &o) const override{
        return ptr->random(o);
    }
private:
    shared_ptr<hittable> ptr;
};
#endif //RAYTRACINGTHENEXTWEEK_FLIP_FACE_H
