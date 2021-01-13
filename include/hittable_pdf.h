//
// Created by VRLAB on 2020/12/6.
//

#ifndef HITTABLE_PDF_H
#define HITTABLE_PDF_H
#include "pdf.h"
#include "hittable.h"

class hittable_pdf : public pdf{
public:
    //origin is the not the hittable point, but the ray origin point
    hittable_pdf(shared_ptr<hittable> p, const point3& origin): ptr(p), o(origin){}
    virtual double value(const vec3 &direction) const override{
        return ptr->pdf_value(o, direction);
    }
    virtual vec3 generate() const override{
        return ptr->random(o);
    }
private:
    point3 o;
    shared_ptr<hittable> ptr;
};


#endif //RAYTRACINGTHENEXTWEEK_HITTABLE_PDF_H
