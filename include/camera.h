#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera{
public:
    camera(point3 lookfrom, 
           point3 lookat, 
           vec vup,
           double vfov, 
           double aspect_ratio, 
           double aperture, 
           double focus_dis,
           double _time0 = 0,
           double _time1 = 0
    );
    Ray getRay(double s, double t) const;
private:
    point3 origin;
    point3 lower_left_corner;
    vec horizental;
    vec vertical;
    vec u, v, w;
    double len_radius;
    double time0, time1;
};
#endif