#include "dcurve.hpp"

void DCurve::Modulate() {
    // case: no modulation
    if (IsLineFit()) {
        return; 
    }

    if ((f->type_descriptor).substr(0,3) == "log") {
        f = new SquareFit22(f->pts);
    } else {
        f = new LogFit22(f->pts);
    }
}

rowvec DCurve::First() {
    return (f->pts).first; 
}

rowvec DCurve::Second() {
    return (f->pts).second; 
}

vec DCurve::Ext0() {
    vec v = {First()(0),Second()(0)};
    return {v.min(),v.max()};
}

vec DCurve::Ext1() {
    vec v = {First()(1),Second()(1)};
    return {v.min(),v.max()};
}

float DCurve::Fit(float x) {
    vec v = Ext0();
    assert (RoundDecimalNPlaces(x,5) >= RoundDecimalNPlaces(v(0),5)
        && RoundDecimalNPlaces(x,5) <= RoundDecimalNPlaces(v(1),5));        
    return f->Fit(x);
}

float DCurve::YFit(float y) {
    vec v = Ext1();
    assert (RoundDecimalNPlaces(y,5) >= RoundDecimalNPlaces(v(0),5)
         && RoundDecimalNPlaces(y,5) <= RoundDecimalNPlaces(v(1),5));
    return f->YFit(y);
}

bool DCurve::PointInRange(rowvec p) {
    vec v = (d == 'l' || d == 'r') ? Ext1(): Ext0();
    int axis =  (d == 'l' || d == 'r') ? 1: 0;
    return p(axis) >= v(0) && p(axis) <= v(1);
}

bool DCurve::IsLineFit() {
    return ((f->type_descriptor).substr(0,4) == "line"); 
}

bool DCurve::IsComplement(DCurve dc) {
    assert (ComplementaryDirection(d) == dc.d);
    int axis = (dc.d == 'l' || dc.d == 'r') ? 1: 0; 

    /*
    col d = sort((f->PointsToMat()).col(axis));
    col d2 = sort((d.f->PointsToMat()).col(axis));
    */

    vec dx = conv_to<vec>::from((f->PointsToMat()).col(axis)); 
    vec dx2 = conv_to<vec>::from((dc.f->PointsToMat()).col(axis)); 
    dx = sort(dx);
    dx2 = sort(dx2);

    if (dx(0) >= dx2(0) && dx(0) <= dx2(1)) {
        return true;
    }

    if (dx(1) >= dx2(0) && dx(1) <= dx2(1)) {
        return true;
    }

    if (dx2(0) >= dx(0) && dx2(0) <= dx(1)) {
        return true;
    }

    if (dx2(1) >= dx(0) && dx2(1) <= dx(1)) {
        return true;
    }

    return false;
}

void DCurve::ModifyFit(pair<rowvec,rowvec> modif) {
    f->pts = modif;
}

void DCurve::Display() {
    cout << "\tcurvedir: " << d << endl;
    cout << "\tpoints:" << endl;
    cout << "= " << (f->pts).first << endl;
    cout << "= " << (f->pts).second << endl;
    cout << endl;
}