#include "../include/sphere.h"
#include "../include/onb.h"

bool sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const
{
    auto a = r.direction().norm2();
    auto b = dot(r.direction(), r.origin() - center);
    auto c = (r.origin() - center).norm2() - radius * radius;
    auto delta = b * b - a * c;
    if(delta < 0) return false;
    auto root = (-b - sqrt(delta)) / a;
    if(root < t_min || root > t_max)
    {  
        root = (-b + sqrt(delta)) / a;
        if(root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    get_sphere_uv(outward_normal, rec.u, rec.v);
    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& bounding_box) const
{
    bounding_box = aabb(center - vec3(radius, radius, radius),
                        center + vec3(radius, radius, radius));
    return true;
}

void sphere::get_sphere_uv(const point3& p, double& u,  double& v){
    double theta = acos(-p.y());
    double phi = atan2(-p.z(), p.x()) + pi;

    u = phi / 2 / pi;
    v = theta / pi;
}

double sphere::pdf_value(const point3 &o, const vec3 &v) const {
    hit_record rec;
    if(!this->hit(Ray(o, v), 0.001, infinity, rec))
        return 0.0;
    auto cos_theta_max = sqrt(1 - radius * radius / (center - o).norm2());
    auto solid_angle = 2 * pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

vec3 sphere::random(const point3 &o) const {
    auto dir = center - o;
    onb uvw;
    uvw.build_from(dir);
    return uvw.local(random_to_sphere(radius, dir.norm2()));
}