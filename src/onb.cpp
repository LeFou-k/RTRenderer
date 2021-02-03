//
// Created by VRLAB on 2020/12/6.
//
#include "../include/onb.h"

vec3 onb::local(double a, double b, double c) const {
    return a * u() + b * v() + c * w();
}

vec3 onb::local(const vec3 &x) const {
    return x.x() * u() + x.y() * v() + x.z() * w();
}

void onb::build_from(const vec3 & n) {
    axis[2] = unit_vector(n);
    vec3 a = fabs(w().x()) > 0.9 ? vec3(0, 1, 0) : vec3(1, 0, 0);
    axis[1] = unit_vector(cross(w(), a));
    axis[0] = cross(w(), v());
}