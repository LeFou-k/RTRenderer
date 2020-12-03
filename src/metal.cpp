#include "../include/metal.h"
#include "../include/hittable.h"

bool metal::scatter(const Ray& ray_in, const hit_record& rec, color& attenuation, Ray& scatter) const{
    vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
    scatter = Ray(rec.p, reflected + random_in_unit_sphere() * fuzz, ray_in.time());
    attenuation = albedo;
    // return dot(reflected, rec.normal) > 0;
    return true;
}