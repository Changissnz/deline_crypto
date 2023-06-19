#include "fit22.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(LogFit22_FitTest, LogFit22_FitTestCorrect) {
    rowvec r1 = {60.,80.};
    rowvec r2 = {30.,12.};
    LogFit22 lf = LogFit22(make_pair(r1,r2));
    
    float x1 = lf.Fit(60.);
    float x2 = lf.Fit(30.);
    float x3 = lf.Fit(45.);

    assert (x1 == 80.);
    assert (x2 == 12.);
    assert (FloatEquals(x3,62.3447,4));
}

TEST(LogFit22_YFitTest, LogFit22_YFitTestCorrect) {
    rowvec r1 = {60.,80.};
    rowvec r2 = {30.,12.};
    LogFit22 lf = LogFit22(make_pair(r1,r2));
    
    float x1 = lf.Fit(60.);
    float x2 = lf.Fit(30.);
    float x3 = lf.Fit(45.);

    assert (FloatEquals(lf.YFit(x1),60.,0)); 
    assert (FloatEquals(lf.YFit(x2),30.,0)); 
    assert (FloatEquals(lf.YFit(x3),45.,0)); 
}

TEST(SquareFit22_FitTest, SquareFit22_FitTestCorrect) {
    rowvec r1 = {60.,80.};
    rowvec r2 = {30.,12.};
    SquareFit22 lf = SquareFit22(make_pair(r1,r2));
    
    float x1 = lf.Fit(60.);
    float x2 = lf.Fit(30.);
    float x3 = lf.Fit(45.);

    assert (x1 == 80.);
    assert (x2 == 12.);
    assert (FloatEquals(x3,29.,0.));
}

TEST(SquareFit22_YFitTest, SquareFit22_YFitTestCorrect) {
    rowvec r1 = {60.,80.};
    rowvec r2 = {30.,12.};
    SquareFit22 lf = SquareFit22(make_pair(r1,r2));
    
    float x1 = lf.Fit(60.);
    float x2 = lf.Fit(30.);
    float x3 = lf.Fit(45.);

    assert (FloatEquals(lf.YFit(x1),60.,0)); 
    assert (FloatEquals(lf.YFit(x2),30.,0)); 
    assert (FloatEquals(lf.YFit(x3),45.,0)); 
}