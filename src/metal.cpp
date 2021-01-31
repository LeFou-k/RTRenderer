#include "../include/metal.h"
#include "../include/hittable.h"

bool metal::scatter(const Ray &ray_in, const hit_record &rec, scatter_record &srec) const{
    vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
    srec.specular_ray = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
    srec.is_specular = true;
    srec.attenuation = albedo;
    srec.pdf_ptr = nullptr;
    return true;
}