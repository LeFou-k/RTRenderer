#ifndef CHECKER_H
#define CHECKER_H
#include "texture.h"
#include "solid_color.h"


class checker_texture : public texture{
public:
    checker_texture(){}
    checker_texture(shared_ptr<texture> _odd, shared_ptr<texture> _even) : odd(_odd), even(_even) {}
    checker_texture(const color& c1, const color& c2): odd(make_shared<solid_color>(c1)), even(make_shared<solid_color>(c2)) {}
    virtual color value(double u, double v, const point3& p) const override;
private:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};
#endif