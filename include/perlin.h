#ifndef PERLIN_H
#define PERLIN_H
#include "rtweekend.h"

class perlin{
public:
    perlin();
    ~perlin();

    double noise(const point3& p) const;
    double turb(const point3& p, int depth=7) const;
private:
    static const int point_cnt = 256;
    double* ranfloat;
    vec3* ranvec;
    int* perm_x;
    int* perm_y;
    int* perm_z;

    static int* perlin_generate_perm();

    static void permute(int* p, int n);
    static double trilinear_interp(vec3 c[2][2][2], double u, double v, double w);
};

#endif