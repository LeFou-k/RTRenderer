#include "../include/bvh.h"
#include <chrono>

bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const
{
    output_box = box;
    return true;
}

bool bvh_node::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
    if(!box.hit(r, t_min, t_max)) return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

bvh_node::bvh_node(const vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end, double time0, double time1)
{
    auto objects = src_objects;
    aabb total_box;
    for(int i = start; i < end; ++i){
        aabb temp_box;
        if(src_objects[i]->bounding_box(time0, time1, temp_box))
            total_box = surrounding_box(total_box, temp_box);
    }
    int axis = total_box.get_longest_axis();
    auto comparator = (axis == 0) ? box_x_compare :
                      (axis == 1) ? box_y_compare :
                                    box_z_compare ;
    size_t object_span = end - start;
    if(object_span == 1){
        left = right = objects[start];
    }
    else if(object_span == 2){
        if(comparator(objects[start], objects[start+1])){
            left = objects[start];
            right = objects[start+1];
        }
        else{
            left = objects[start+1];
            right = objects[start];
        }
    }
    else {
        std::sort(objects.begin()+start, objects.begin()+end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_l, box_r;

    if(!left->bounding_box(time0, time1, box_l) || !right->bounding_box(time0, time1, box_r))
        std::cerr << "No bounding box in bvh_node constructor.\n";
    
    box = surrounding_box(box_l, box_r);
}


bool box_x_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b)
{
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b)
{
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b)
{
    return box_compare(a, b, 2);
}

bool box_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b, int axis)
{
    aabb box1, box2;
    if(!a->bounding_box(0, 0, box1) || !b->bounding_box(0, 0, box2))
        std::cerr << "No bounding box in bvh node constructor.\n";

    return box1.min()[axis] < box2.min()[axis];
}