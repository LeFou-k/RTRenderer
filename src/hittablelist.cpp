#include "../include/hittablelist.h"


//hittable_list::hit returns whether ray hit objects and the nearest hit_record
bool hittable_list::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for(const auto& object : objects)
    {
        //from t_min to closest_so_far, if hit, then update
        if(object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const
{
    aabb temp_box;
    bool isFirst = true;
    for(const auto& object : objects){
        if(!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = isFirst ? temp_box : surrounding_box(output_box, temp_box);
        isFirst = false;
    }
    
    return true;
}