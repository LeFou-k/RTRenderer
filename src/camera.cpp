#include "../include/camera.h"

camera::camera(point3 lookfrom, 
    point3 lookat, 
    vec3 vup,
    double vfov, 
    double aspect_ratio, 
    double aperture, 
    double focus_dis,
    double _time0,
    double _time1
){
    auto theta = degree_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h * focus_dis;
    auto viewport_width = viewport_height * aspect_ratio;
    time0 = _time0;
    time1 = _time1;

    origin = lookfrom;
    w = unit_vector(lookfrom - lookat);
    v = unit_vector(cross(vup, w));
    u = cross(w, v);
    horizental = viewport_width * v;
    vertical = viewport_height * u;
    lower_left_corner = origin - horizental / 2 - vertical / 2 - w * focus_dis;
    len_radius = aperture / 2;
}

Ray camera::getRay(double s, double t) const{
    auto rd = random_in_unit_disk() * len_radius;
    auto offset = u * rd.y() + v * rd.x();
    return Ray(origin + offset, lower_left_corner + s * vertical + t * horizental - origin - offset, random_double(time0, time1));
}