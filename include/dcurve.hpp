/// TODO: delete, unnecessary class. -
#ifndef DCURVE_HPP
#define DCURVE_HPP

#include "fit22.hpp"

class DCurve
{

public:

    Fit22* f;
    char d;

    DCurve(Fit22* f_,char direction) {
        f = f_;
        d = direction;
    }

    void Modulate();
    rowvec First();
    rowvec Second();
    vec Ext0();
    vec Ext1();
    float Fit(float x);
    float YFit(float y);
    bool PointInRange(rowvec p);
    bool IsLineFit();
    bool IsComplement(DCurve dc);

    void ModifyFit(std::pair<rowvec,rowvec> modif);
    void Display();

};

#endif