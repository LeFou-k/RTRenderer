#include "../include/perlin.h"

perlin::perlin(){
    ranvec = new vec3[point_cnt];
    for(int i = 0; i < point_cnt; i++){
        ranvec[i] = unit_vector(vec3::random(-1, 1));
    }

    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();

}
perlin::~perlin(){
    delete[] ranvec;
    delete[] perm_x;
    delete[] perm_y;
    delete[] perm_z;
}

int* perlin::perlin_generate_perm(){
    auto p = new int[point_cnt];

    for(int i = 0; i < perlin::point_cnt; ++i)
        p[i] = i;

    permute(p, point_cnt);

    return p;
}

void perlin::permute(int *p, int n){
    for(int i = n - 1; i > 0; i--){
        int target = random_int(0, i);
        std::swap(p[i], p[target]);
    }
}

double perlin::noise(const point3& p) const{
    auto u = p.x() - floor(p.x());
    auto v = p.y() - floor(p.y());
    auto w = p.z() - floor(p.z());

    //Hermitian Smoothing

    auto i = static_cast<int>(floor(p.x()));
    auto j = static_cast<int>(floor(p.y()));
    auto k = static_cast<int>(floor(p.z()));

    vec3 c[2][2][2];

    for(int di = 0; di < 2; ++di){
        for(int dj = 0; dj < 2; ++dj){
            for(int dk = 0; dk < 2; ++dk){
                c[di][dj][dk] = ranvec[
                        perm_x[(i + di) & 255] ^
                        perm_y[(j + dj) & 255] ^
                        perm_z[(k + dk) & 255]
                    ];
            }
        }
    }
    return trilinear_interp(c, u, v, w);
}

double perlin::trilinear_interp(vec3 c[2][2][2], double u, double v, double w){
    auto accum = 0.0;
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);

    for(int i = 0; i < 2; ++i){
        for(int j = 0; j < 2; ++j){
            for(int k = 0; k < 2; ++k){
                vec3 weight_v(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) * 
                         (j * vv + (1 - j) * (1 - vv)) * 
                         (k * ww + (1 - k) * (1 - ww)) * dot(weight_v, c[i][j][k]);
            }
        }
    }
    return accum;
}

double perlin::turb(const point3& p, int depth) const{
    double weight = 1.0;
    auto accum = 0.0;
    auto temp_p = p;
    for(int i = 0; i < depth; ++i){
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return fabs(accum);
}