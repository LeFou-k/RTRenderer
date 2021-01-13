#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"
#include "pdf.h"

struct hit_record;

struct scatter_record{
    Ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

class material{
public:
    //ray in, hit_record for input, attenuation and scatter for output
    virtual bool scatter(const Ray& ray_in, const hit_record& rec, scatter_record& srec) const {
        return false;
    }
    virtual double scatter_pdf(const Ray& ray_in, const hit_record& rec, const Ray& scatter) const{
        return 0;
    }
    virtual color emitted(const Ray& ray_in, const hit_record& record, double u, double v, const point3& p) const {
        return color(0, 0, 0);
    }
};
#endif