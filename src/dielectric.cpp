#include "../include/dielectric.h"
#include "../include/hittable.h"


bool dielectric::scatter(const Ray& ray_in, const hit_record& rec, color& attenuation, Ray& scatter) const
{
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? 1.0 / ir : ir;
    vec3 unit_direction = unit_vector(ray_in.direction());
    
    auto cos_theta = fmin(dot(-unit_direction,rec.normal), 1.0);
    auto sin_theta = sqrt(1 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 dir;
    if(cannot_refract || reflectance(cos_theta,refraction_ratio) > random_double()) dir = reflect(unit_direction, rec.normal);
    else dir = refract(unit_direction, rec.normal, refraction_ratio);
    scatter = Ray(rec.p, dir, ray_in.time());
    return true;
}

double dielectric::reflectance(double cosine, double ref_idx){
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}