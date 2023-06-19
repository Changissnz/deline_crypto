#include "extras.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(IndicesToComplementarySubArmaMat_Case1_Test, IndicesToComplementarySubArmaMat_Case1_Test_Correct) {
    mat B = {{-3,4,5},{0,3,10},{-5,-1,-2},
        {-3,4,5},{0,3,10},{-5,-1,-2},
        {-3,4,5},{0,3,10},{-5,-1,-2},
        {-3,4,5},{0,3,10},{-5,-1,-2}};

    ivec v = {0,2,7,9};

    mat B2 = IndicesToComplementarySubArmaMat(B,v);

    mat B3 = {
    {0,3,10},
    {-3,4,5},
    {0,3,10},
    {-5,-1,-2},
    {-3,4,5},
    {-5,-1,-2},
    {0,3,10},
    {-5,-1,-2}};

    assert(approx_equal(B2,B3,"absdiff",0.02));

}
