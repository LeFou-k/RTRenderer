#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "hittable.h"
#include "aabb.h"


using std::shared_ptr;
using std::make_shared;
using std::vector;

//cpp默认为private继承，只有public继承才能实现Derived-to-Base Conversion
class hittable_list : public hittable{
public:
    hittable_list(){}
    hittable_list(shared_ptr<hittable> object){ add(object); }
    void clear() {objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    virtual double pdf_value(const point3 &o, const vec3 &v) const override;

    virtual vec3 random(const point3 &o) const override;
    vector<shared_ptr<hittable>> objects;
};


#endif