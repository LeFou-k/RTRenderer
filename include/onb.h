//
// Created by VRLAB on 2020/12/6.
//

#ifndef ONB_H
#define ONB_H

#include "rtweekend.h"

class onb{
public:
    onb(){}
    inline vec3 operator[] (const int i) const{ return axis[i]; };
    vec3 u() const{ return axis[0]; }
    vec3 v() const{ return axis[1]; }
    vec3 w() const{ return axis[2]; }
    vec3 local(double a, double b, double c) const;
    vec3 local(const vec3& x) const;
    void build_from(const vec3& );
private:
    vec3 axis[3];
};
#endif //RAYTRACINGTHENEXTWEEK_ONB_H
