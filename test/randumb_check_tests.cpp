#include "randumb_check.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(APRNGGauge__MeasureCycle_Test, APRNGGauge__MeasureCycle_TestCorrect) {

    StdRandGenerator* stdrg = new StdRandGenerator(62);
    stdrg->pfr = make_pair(0,31);
    APRNGGauge* ag = new APRNGGauge(stdrg,make_pair(0,31),0.5);

    ag->MeasureCycle(true);
    cout << ag->cycle_scores << endl;
    assert(ag->cycle_scores(0,0) == 1.);
}

TEST(NormalizedUWPD_Test, NormalizedUWPD_TestCorrect) {
    vec v1 = {0,10,0,10,0,10,0};
    vec v2 = {0,10,0,10,0,10};
    vec v3 = {0,0,0,0};
    vec v4 = {10,10,10,10,10};
    vec v5 = {5,5,5,5,5};
    vec v6 = {0,5,0,5,0,5};
    vec v7 = {0,2.5,0,2.5,0,2.5};

    float f1 = NormalizedUWPD(v1, make_pair(0,10));
    float f2 = NormalizedUWPD(v2, make_pair(0,10));
    float f3 = NormalizedUWPD(v3, make_pair(0,10));
    float f4 = NormalizedUWPD(v4, make_pair(0,10));
    float f5 = NormalizedUWPD(v5, make_pair(0,10));
    float f6 = NormalizedUWPD(v6, make_pair(0,10));
    float f7 = NormalizedUWPD(v7, make_pair(0,10));

    assert(f1 == 1.);
    assert(f2 == 1.);
    assert(f3 == 0.);
    assert(f4 == 0.);
    assert(f5 == 0.);
    assert(f6 == 0.5);
    assert(f7 == 0.25);
}