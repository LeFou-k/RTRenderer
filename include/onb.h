//
// Created by VRLAB on 2020/12/6.
//

#ifndef ONB_H
#define ONB_H

#include "rtweekend.h"

class onb{
public:
    onb(){}
    inline vec operator[] (const int i) const{ return axis[i]; };
    vec u() const{ return axis[0]; }
    vec v() const{ return axis[1]; }
    vec w() const{ return axis[2]; }
    vec local(double a, double b, double c) const;
    vec local(const vec& x) const;
    void build_from(const vec& );
private:
    vec axis[3];
};
#endif //RAYTRACINGTHENEXTWEEK_ONB_H
