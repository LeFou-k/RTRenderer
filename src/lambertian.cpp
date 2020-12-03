#include "../include/lambertian.h"
#include "../include/hittable.h"


bool lambertian::scatter(const Ray& ray_in, const hit_record& rec, color& attenuation, Ray& scatter) const
{
    auto scatter_direction = rec.normal + random_unit_vector();
    if(scatter_direction.near_zero()){
        scatter_direction = rec.normal;
    }
    scatter = Ray(rec.p, scatter_direction, ray_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}