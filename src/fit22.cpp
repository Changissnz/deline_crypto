#include "fit22.hpp"
using namespace std;

mat PointDestinationsToAdditives(mat pds,rowvec p,vec pointPace) {
    assert (pds.n_rows == pointPace.size());

    mat mxx(0,0,fill::zeros);
    rowvec rx;
    for (int i = 0; i < pds.n_rows; i++) {
        LineFit lf = LineFit(make_pair(p,pds.row(i)));
        rx = lf.PointByAxialRatio(pointPace(i));
        mxx.insert_rows(mxx.n_rows,rx - p);
    }

    return mxx;
}

rowvec ApplyPointDestinations(mat pds,rowvec p,vec pointPace) {
    mat mxx = PointDestinationsToAdditives(pds,p,pointPace);
    for (int i = 0; i < mxx.n_rows; i++) {
        p = p + mxx.row(i);
    }
    return p; 
} 

float LogFit22::Fit(float x) {
    mat pts_ = PointsToMat();
    float r1 = abs(x - pts_(direction.first,0)) / abs(pts_(0,0) - pts_(1,0));
    float r2 = log(r1 * 9. + 1.) / log(10.);
    return pts_(direction.first,1) + r2 * abs(pts_(0,1) - pts_(1,1)); 
}

float LogFit22::YFit(float y) {
    y = RoundDecimalNPlaces(y,5); 
    mat pts_ = PointsToMat();
    float r1 = log(10.) * (y - pts_(direction.first,1)) / abs(pts_(0,1) - pts_(1,1)); 
    float r2 = (exp(r1) - 1.) / 9.;
    float r3 = r2 * abs(pts_(1,0) - pts_(0,0));
    float x1 = RoundDecimalNPlaces(pts_(direction.first,0) - r3,5);
    float x2 = RoundDecimalNPlaces(pts_(direction.first,0) + r3,5);

    float minumum = RoundDecimalNPlaces(pts_.col(0).min(),5);
    float maximum = RoundDecimalNPlaces(pts_.col(0).max(),5);
    if (x1 >= minumum && x1 <= maximum) {
        return x1;
    }

    if (x2 >= minumum && x2 <= maximum) {
        return x2;
    }

    assert(false);
    return -1;
}

float SquareFit22::Fit(float x) {
    mat pts_ = PointsToMat();
    float r1 = abs(x - pts_(direction.first,0)) / abs(pts_(0,0) - pts_(1,0));
    float r2 = pow(r1,2.);
    return pts_(direction.first,1) + r2 * abs(pts_(0,1) - pts_(1,1));
}

float SquareFit22::YFit(float y) {
    y = RoundDecimalNPlaces(y,5);
    mat pts_ = PointsToMat();
    float r1 = (y - pts_(direction.first,1)) / abs(pts_(0,1) - pts_(1,1));
    float r2 = pow((pts_(direction.first,0) - pts_(direction.second,0)),2.);
    float r3 = sqrt(r1 * r2);

    float x1 = RoundDecimalNPlaces(pts_(direction.first,0) - r3,5);
    float x2 = RoundDecimalNPlaces(pts_(direction.first,0) + r3,5);

    float minumum = RoundDecimalNPlaces(pts_.col(0).min(),5);
    float maximum = RoundDecimalNPlaces(pts_.col(0).max(),5);

    if (x1 >= minumum && x1 <= maximum) {
        return x1;
    }

    if (x2 >= minumum && x2 <= maximum) {
        return x2;
    }

    assert(false);
    return -1;
}

float LineFit::Fit(float x) {
    x = RoundDecimalNPlaces(x,5);
    if (m != numeric_limits<float>::infinity()) {
        return RoundDecimalNPlaces(m * x + b,5);
    }
    return pts.first(1);
}

float LineFit::YFit(float y) {
    y = RoundDecimalNPlaces(y,5);
    if (m == 0) {
        return pts.first(0);
    }

    if (m != numeric_limits<float>::infinity()) {
        return RoundDecimalNPlaces((y - b) / m,5);
    }
    
    return pts.first(0);
}

/// TODO: wrong
rowvec LineFit::PointByAxialRatio(float r) {

    if (m == numeric_limits<float>::infinity()) {
        return {pts.first(0), pts.first(1) + r * (pts.second(1) - pts.first(1))};
    }

    if (m == 0.) {
        return {pts.first(0) + r * (pts.second(0) - pts.first(0)),pts.first(1)};        
    }

    // get the difference in x-distance
    float d = (pts.second(0) - pts.first(0)) * r;
    return {d,m * (pts.first(0) + d) + b};
}
