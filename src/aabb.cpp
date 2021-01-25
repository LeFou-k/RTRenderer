#include "../include/aabb.h"


bool aabb::hit(const ray &r, double t_min, double t_max) const {
    const int dim = 3;
    for (int d = 0; d < dim; ++d) {
        auto inv_D = 1.0f / r.direction()[d];
        double t0 = (minimum[d] - r.origin()[d]) * inv_D;
        double t1 = (maximum[d] - r.origin()[d]) * inv_D;

        if (inv_D < 0.0f) std::swap(t0, t1);

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_min >= t_max) return false;
    }
    return true;
}

int aabb::get_longest_axis() const {
    auto diff = maximum - minimum;
    int axis = 0;
    auto longest = -infinity;
    for(int i = 0; i < 3; ++i){
        if (diff[i] > longest){
            longest = diff[i];
            axis = i;
        }
    }
    return axis;
}

aabb surrounding_box(aabb &box0, aabb &box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small, big);
}
