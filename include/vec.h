#ifndef VEC_H
#define VEC_H


#include<cmath>
#include<iostream>
#include<algorithm>
#include<cassert>
#include "rtweekend.h"

using std::sqrt;
using std::fabs;
using std::fmin;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - vec<n>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
template<int n>
class vec {
public:
    vec() = default;

    vec<n> operator-() const {
        vec<n> res;
        for (int i = 0; i < n; ++i)
            res[i] = -v[i];
        return res;
    }

    double operator[](int i) const {
        assert(i >= 0 && i < n);
        return v[i];
    }

    double &operator[](int i) {
        assert(i >= 0 && i < n);
        return v[i];
    }

    vec<n> &operator+=(const vec<n> &e) {
        for (int i = 0; i < n; ++i)
            v[i] += e[i];
        return *this;
    }

    vec<n> &operator*=(const double t) {
        for (int i = 0; i < n; ++i)
            v[i] *= t;
        return *this;
    }

    vec<n> &operator/=(const double t) {
        assert(t != 0);
        return *this *= 1 / t;
    }

    inline double norm2() const {
        return (*this) * (*this);
    }

    inline double norm() const {
        return sqrt(norm2());
    }

    inline vec<n> normalize() const {
        *this /= norm();
        return *this;
    }

    inline bool near_zero() const {
        const double e = 1e-8;
        for (int i = 0; i < n; ++i)
            if (fabs(v[i]) > e) return false;
        return true;
    }

    //generate random vector
    inline static vec<n> random() {
        vec<n> res;
        for (int i = 0; i < n; ++i)
            res[i] = random_double();
        return res;
    }

    inline static vec<n> random(double min, double max) {
        vec<n> res;
        for (int i = 0; i < n; ++i)
            res[i] = random_double(min, max);
        return res;
    }

private:
    double v[n] = {0};
};

/*- - - - - - - - - - - - - - - - - - - - - - - - - - vecn utils - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

template<int n>
inline std::ostream &operator<<(std::ostream &out, const vec<n> &v) {
    out << v[0];
    for (int i = 1; i < n; ++i) {
        out << " " << v[i];
    }
    return out;
}

template<int n>
inline vec<n> operator+(const vec<n> &u, const vec<n> &v) {
    vec<n> res;
    for (int i = 0; i < n; i++)
        res[i] = u[i] + v[i];
    return res;
}

template<int n>
inline vec<n> operator-(const vec<n> &u, const vec<n> &v) {
    return u + (-v);
}

template<int n>
inline vec<n> operator*(const vec<n> &u, const vec<n> &v) {
    double res = 0;
    for (int i = 0; i < n; ++i) {
        res += u[i] * v[i];
    }
    return res;
}

template<int n>
inline vec<n> operator*(const double t, const vec<n> &v) {
    vec<n> res;
    for (int i = 0; i < n; ++i)
        res[i] = t * v[i];
    return res;
}

template<int n>
inline vec<n> operator*(const vec<n> &v, const double t) {
    return t * v;
}

template<int n>
inline vec<n> operator/(const vec<n> &v, const double t) {
    assert(t != 0);
    return (1 / t) * v;
}

template<int n>
inline double dot(const vec<n> &u, const vec<n> &v) {
    double res = 0.0;
    for (int i = 0; i < n; ++i)
        res += u[i] * v[i];
    return res;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - type define - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;
using point3 = vec3;
using color = vec3;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - vec2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
template<>
class vec<2> {
public:
    vec() = default;

    vec(double x, double y) : v{x, y} {}

    double x() const { return v[0]; }

    double y() const { return v[1]; }

    vec2 operator-() const {
        return vec2(-v[0], -v[1]);
    }


    double operator[](const int i) const {
        assert(i >= 0 && i < 2);
        return v[i];
    }

    double &operator[](const int i) {
        assert(i >= 0 && i < 2);
        return v[i];
    }

    vec2 &operator+=(const vec2 &e) {
        v[0] += e[0];
        v[1] += e[1];
        return *this;
    }

    vec2 &operator-=(const vec2 &e) {
        *this += -e;
        return *this;
    }

    vec2 &operator*=(const double t) {
        v[0] *= t;
        v[1] *= t;
        return *this;
    }

    vec2 &operator/=(const double t) {
        *this *= (1 / t);
        return *this;
    }


    inline double norm2() { return v[0] * v[1]; }

    inline double norm() { return sqrt(norm2()); }

    inline vec2 normalize() {
        (*this) /= norm();
        return *this;
    }

private:
    double v[2];
};
/*- - - - - - - - - - - - - - - - - - - - - - - - - vec2 utils - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - vec3 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
template<>
class vec<3> {
public:
    vec() = default;

    vec(double x, double y, double z) : v{x, y, z} {}

    double x() const { return v[0]; }

    double y() const { return v[1]; }

    double z() const { return v[2]; }

    double operator[](const int i) const {
        assert(i >= 0 && i < 3);
        return v[i];
    }

    double &operator[](const int i) {
        assert(i >= 0 && i < 3);
        return v[i];
    }

    vec<3> operator-() const {
        return vec3(-v[0], -v[1], -v[2]);
    }

    vec3 &operator+=(const vec3 &e) {
        v[0] += e[0];
        v[1] += e[1];
        v[2] += e[2];
        return *this;
    }

    vec3 &operator-=(const vec3 &e) {
        *this += (-e);
        return *this;
    }

    vec3 &operator*=(const double t) {
        v[0] *= t;
        v[1] *= t;
        v[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t) {
        return *this *= 1 / t;
    }

    inline double norm2() { return v[0] * v[0] + v[1] * v[1] + v[2] * v[2]; }

    inline double norm() { return sqrt(norm2()); }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    inline vec3 normalize() {
        *this = (*this) / norm();
        return *this;
    }

private:
    double v[3];
};


/*- - - - - - - - - - - - - - - - - - - - - - - - vec3 utils - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u[1] * v[2] - u[2] * v[1], v[0] * u[2] - v[2] * u[0], u[0] * v[1] - u[1] * v[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v.normalize();
}

inline vec3 sqrt(vec3 v) {
    return vec3(sqrt(v.x()), sqrt(v.y()), sqrt(v.z()));
}

//get a random vector in unit sphere space:
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.norm2() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3 &normal) {
    auto in_unit_sphere = random_in_unit_sphere();
    return dot(in_unit_sphere, normal) > 0 ? in_unit_sphere : -in_unit_sphere;
}

inline vec3 random_in_unit_disk() {
    while (true) {
        point3 p(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.norm2() >= 1) continue;
        return p;
    }
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &in, const vec3 &n, double etai_over_etat) {
    //?why use min(dot, 1.0)
    auto cos_theta = fmin(dot(-in, n), 1.0);
    vec3 output_vertical = etai_over_etat * (in + n * cos_theta);
    vec3 output_parallel = -sqrt(fabs(1 - output_vertical.norm2())) * n;
    return output_vertical + output_parallel;
}

inline vec3 random_consine_direction() {
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = std::sqrt(1 - r2);
    auto phi = 2 * pi * r1;
    auto x = cos(phi) * sqrt(r2);
    auto y = sin(phi) * sqrt(r2);

    return vec3(x, y, z);
}

inline vec3 random_to_sphere(double radius, double distance_squared) {
    auto cos_theta = sqrt(1 - radius * radius / distance_squared);
    auto r1 = random_double();
    auto r2 = random_double();

    auto z = 1 + r2 * (cos_theta - 1);
    auto x = cos(2 * pi * r1) * sqrt(1 - z * z);
    auto y = sin(2 * pi * r1) * sqrt(1 - z * z);

    return vec3(x, y, z);
}


#endif