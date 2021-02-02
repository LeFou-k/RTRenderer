//
// Created by VRLAB on 2020/12/6.
//

#ifndef MIXTURE_PDF_H
#define MIXTURE_PDF_H
#include "pdf.h"
class mixture_pdf : public pdf{
public:
    mixture_pdf(shared_ptr<pdf> p0, shared_ptr<pdf> p1){
        p[0] = p0;
        p[1] = p1;
    }
    virtual double value(const vec &direction) const override{
        return 0.5 * (p[0]->value(direction) + p[1]->value(direction));
    }
    virtual vec generate() const override{
        return random_double() < 0.5 ? p[0]->generate() : p[1]->generate();
    }
private:
    shared_ptr<pdf> p[2];
};
#endif //RAYTRACINGTHENEXTWEEK_MIXTURE_PDF_H
