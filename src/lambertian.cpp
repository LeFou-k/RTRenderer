#include "../include/lambertian.h"
#include "../include/hittable.h"
#include "../include/onb.h"
#include "../include/cosine_pdf.h"

bool lambertian::scatter(const Ray &ray_in, const hit_record &rec, scatter_record &srec) const
{
    srec.is_specular = false;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);

    return true;
}

double lambertian::scatter_pdf(const Ray &ray_in, const hit_record &rec, const Ray &scatter) const {
    double cosine = dot(unit_vector(scatter.direction()), rec.normal);
    return cosine < 0.0 ? 0.0 : cosine / pi; //return 0 if scatter Ray is under the face
}