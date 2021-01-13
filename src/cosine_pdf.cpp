//
// Created by VRLAB on 2020/12/6.
//
#include "../include/cosine_pdf.h"

double cosine_pdf::value(const vec3 &direction) const {
    auto cosine = dot(unit_vector(direction), uvw.w());
    return cosine <= 0.0 ? 0.0 : cosine / pi;
}

vec3 cosine_pdf::generate() const {
    return uvw.local(random_consine_direction());
}