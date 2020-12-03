#ifndef AABB_H
#define AABB_H
#include "rtweekend.h"

class aabb{
public:
    aabb() {}
    aabb(const point3& min, const point3& max): minimum(min), maximum(max){}
    point3 min() const{ return minimum; }
    point3 max() const{ return maximum; }
    bool hit(const Ray& r, double t_min, double t_max) const;
private:
    point3 minimum;
    point3 maximum;
};
aabb surrounding_box(aabb& box0, aabb& box1);
#endif