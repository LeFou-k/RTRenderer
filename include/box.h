//
// Created by VRLAB on 2020/12/2.
//

#ifndef BOX_H
#define BOX_H
#include "hittable.h"
#include "hittablelist.h"
class box : public hittable{
public:
    box(){}
    box(const point3& p0, const point3& p1, shared_ptr<material> m_ptr);
    virtual bool hit(const ray& ray, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
private:
    point3 box_min;
    point3 box_max;
    hittable_list sides;
};
#endif //RAYTRACINGTHENEXTWEEK_BOX_H
