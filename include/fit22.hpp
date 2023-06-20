// a few data structures used to fit 2 points in 2 dimensions
#ifndef FIT22_HPP
#define FIT22_HPP

#include <utility>
#include <assert.h>
#include <vector>
//#include <functional>
#include "extras.hpp"
#include <limits>

#include <iostream>
using namespace std; 

mat PointDestinationsToAdditives(mat pds,rowvec p,vec pointPace);
rowvec ApplyPointDestinations(mat pds,rowvec p,vec pointPace);

class Fit22
{

public:

    std::string type_descriptor;
    std::pair<rowvec,rowvec> pts;
    std::pair<int,int> direction;

    Fit22(std::pair<rowvec,rowvec> pts_) {
        pts = std::make_pair(pts_.first,pts_.second);
        if (pts.first(1) > pts.second(1)) {
            direction = std::make_pair(1,0);
        } else {
            direction = std::make_pair(0,1);
        }
    }

    mat PointsToMat() {
        mat pts_(2,2,fill::zeros);
        pts_.row(0) = pts.first;
        pts_.row(1) = pts.second;
        return pts_;
    }

    virtual float Fit(float x)=0;
    virtual float YFit(float y)=0;
};

class LogFit22: public Fit22
{
    /// inherit constructor
    ///using Fit22::Fit22;
public:

    LogFit22(std::pair<rowvec,rowvec> pts_) : Fit22(pts_) {
        type_descriptor = "logfit";
    }

    LogFit22(std::pair<rowvec,rowvec> pts_,std::string suffix) : Fit22(pts_) { 
        type_descriptor = "logfit_" + suffix;
    }    

    float Fit(float x);
    float YFit(float y);
};

class SquareFit22: public Fit22
{
    /// inherit constructor
    ///using Fit22::Fit22; 
public:
    SquareFit22(std::pair<rowvec,rowvec> pts_) : Fit22(pts_) {
        type_descriptor = "squarefit";
    }


    SquareFit22(std::pair<rowvec,rowvec> pts_,std::string suffix) : Fit22(pts_) { 
        type_descriptor = "squarefit_" + suffix;
    }

    float Fit(float x); 

    float YFit(float y);
};

class LineFit: public Fit22 
{

public:

    float m;
    float b;
    
    LineFit(std::pair<rowvec,rowvec> pts_) : Fit22(pts_) {
        // calculate slope
        float m1,m2;
        m1 = pts_.first(0) - pts_.second(0);
        m2 = pts_.first(1) - pts_.second(1);
        if (m2 == 0.) {
            m = std::numeric_limits<float>::max();
        } else {
            m = m2/ m1;
        }

        b = pts_.first(1) - pts_.first(0) * m;

        type_descriptor = "linefit";
    }

    LineFit(std::pair<rowvec,rowvec> pts_,std::string suffix) : LineFit(pts_) {
        type_descriptor = "linefit_" + suffix;
    }


    float Fit(float x);
    float YFit(float y);
    rowvec PointByAxialRatio(float r);
}; 

#endif