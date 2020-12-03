#ifndef VEC3_H
#define VEC3_H


#include<cmath>
#include<iostream>
#include<algorithm>

#include "rtweekend.h"
using std::sqrt;
using std::fabs;
using std::fmin;

class vec3 {
public:
    vec3() : v{0.0, 0.0, 0.0}{}
    vec3(double v0, double v1, double v2): v{v0, v1, v2} {}

    double x() const{ return v[0]; }
    double y() const{ return v[1]; }
    double z() const{ return v[2]; }

    vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
    double operator[](int i) const{ return v[i]; }
    double& operator[](int i) { return v[i]; }
    vec3& operator+=(const vec3& e) {
        v[0] += e[0];
        v[1] += e[1];
        v[2] += e[2];
        return *this;
    }
    vec3& operator*=(const double t){
        v[0] *= t;
        v[1] *= t;
        v[2] *= t;
        return *this;
    }
    vec3& operator/=(const double t){
        return *this *= 1/t;
    }
    inline double length_squared() const{
        return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }
    inline double length() const{
        return sqrt(length_squared());
    }
    inline bool near_zero() const{
        const double e = 1e-8;
        return fabs(v[0]) < e && fabs(v[1]) < e && fabs(v[2]) < e;
    }
    //generate random vector
    inline static vec3 random()
    {
        return vec3(random_double(),random_double(),random_double());
    }
    inline static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max),random_double(min, max),random_double(min, max));
    }
private:
    double v[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v){
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}
inline vec3 operator+(const vec3& u, const vec3& v){
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v){
    return u + (-v);
}
inline vec3 operator*(const vec3& u, const vec3& v){
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}
inline vec3 operator*(const double t, const vec3& v){
    return vec3(t * v[0], t * v[1], t * v[2]);
}
inline vec3 operator*(const vec3& v, const double t){
    return t * v;
}
inline vec3 operator/(const vec3& v, const double t){
    return (1/t) * v;
}
inline double dot(const vec3& u, const vec3& v){
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}
inline vec3 cross(const vec3& u, const vec3& v){
    return vec3(u[1] * v[2] - u[2] * v[1], v[0] * u[2] - v[2] * u[0], u[0] * v[1] - u[1] * v[0]);
}
inline vec3 unit_vector(vec3 v){
    return v / v.length();
}
inline vec3 sqrt(vec3 v){
    return vec3(sqrt(v.x()), sqrt(v.y()), sqrt(v.z()));
}

//get a random vector in unit sphere space:
inline vec3 random_in_unit_sphere() {
    while(true){
        auto p = vec3::random(-1, 1);
        if(p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector(){
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal){
    auto in_unit_sphere = random_in_unit_sphere();
    return dot(in_unit_sphere, normal) > 0 ? in_unit_sphere : -in_unit_sphere;
}

inline vec3 random_in_unit_disk(){
    while(true){
        point3 p(random_double(-1,1), random_double(-1,1), 0);
        if(p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& in, const vec3& n, double etai_over_etat)
{
    //?why use min(dot, 1.0)
    auto cos_theta = fmin(dot(-in, n), 1.0);
    vec3 output_vertical = etai_over_etat * (in + n * cos_theta);
    vec3 output_parallel = -sqrt(fabs(1 - output_vertical.length_squared())) * n;
    return output_vertical + output_parallel;
}
#endif