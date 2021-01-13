#ifndef RTWEEKEND_H
#define RTWEEKEND_H
//set portable utilizable usings, constants, functions

#include <cmath>
#include <memory>
#include <limits>
#include <random>

//Common Headers

//using 
using std::shared_ptr;
using std::make_shared;
using std::sqrt;


//constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double EPSILON = 0.0001;

//Utility functions
inline double degree_to_radians(const double degree)
{
    return degree * pi / 180.0;
}

//return a random real number in range [0,1)
inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
//return a random double number in range [min, max)
inline double random_double(double min, double max)
{
    return min + random_double() * (max - min);
}


inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max + 1));
}

//clamp x in range(min, max)
inline double clamp(double x, double min, double max)
{
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

//先定义变量再包括.h头文件
//common headers 
#include "ray.h"
#include "vec3.h"


#endif