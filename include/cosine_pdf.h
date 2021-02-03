//
// Created by VRLAB on 2020/12/6.
//

#ifndef COSINE_PDF_H
#define COSINE_PDF_H
#include "pdf.h"
#include "onb.h"

class cosine_pdf : public pdf{
public:
    cosine_pdf(const vec3& w){ uvw.build_from(w); }
    virtual double value(const vec3 &direction) const override;
    virtual vec3 generate() const override;

private:
    onb uvw;
};
#endif //RAYTRACINGTHENEXTWEEK_COSINE_PDF_H
