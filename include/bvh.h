#ifndef BVH_H
#define BVH_H

#include <vector>


#include "hittablelist.h"

using std::vector;

class bvh_node : public hittable{
public:
    bvh_node() {}
    bvh_node(const hittable_list& list, double time0, double time1): bvh_node(list.objects, 0, list.objects.size(), time0, time1){}

    bvh_node(const vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end, double time0, double time1);
    
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;

    aabb box;
};

inline bool box_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b, int axis);

bool box_x_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b);

bool box_y_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b);

bool box_z_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b);

#endif