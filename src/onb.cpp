//
// Created by VRLAB on 2020/12/6.
//
#include "../include/onb.h"

vec onb::local(double a, double b, double c) const {
    return a * u() + b * v() + c * w();
}

vec onb::local(const vec &x) const {
    return x.x() * u() + x.y() * v() + x.z() * w();
}

void onb::build_from(const vec & n) {
    axis[2] = unit_vector(n);
    vec a = fabs(w().x()) > 0.9 ? vec(0, 1, 0) : vec(1, 0, 0);
    axis[1] = unit_vector(cross(w(), a));
    axis[0] = cross(w(), v());
}