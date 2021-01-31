//
// Created by VRLAB on 2020/12/3.
//

#ifndef ROTATE_H
#define ROTATE_H

#include "hittable.h"
class rotate : public hittable{
public:
    rotate(shared_ptr<hittable> p, double angle, int axis);
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb &output_box) const override;
private:
    shared_ptr<hittable> ptr;
    int axis;
    double angle;
    aabb box;
    bool hasbox;

    inline point3 rotate_theta(const point3& point, double _angle) const;
};
#endif //RAYTRACINGTHENEXTWEEK_ROTATE_H
