#include "accessory_functions.hpp"
#include <gtest/gtest.h>
using namespace std;

/// NOTE: tests are designed to demonstrate the output of each modification
///       pattern.

TEST(FoxModificationPattern_Output__Case1_Test, FoxModificationPattern_Output__Case1_Test_Correct) {

    mat mx = {{3.0000,   4.0000},
        {3.0000,   4.0000},
        {3.0000,   4.0000},
        {2.0000,   512.00},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000},
        {5.0000,   9.0000}};

    ivec i1 = {3,4,13};
    ivec i2 = {6,1,12};
    ivec i3 = {2,4,5};
    ivec i4 = {10,7,3};
    ivec i5 = {3,2,3,25};
    FoxModificationPattern* fmp = new FoxModificationPattern(3,i1,i2,i3,i4,i5);

    mat m = {{10,15},
        {12,8.},
        {4.,13.}
    }; 

    mat m2 = {{3,4},
        {3,4},
        {2,1},
        {1,2},
        {4.,2},
        {5.,9}};

    ivec cind = {2,4,6};
    mat m3 = fmp->Output(m,m2,cind);
    
    assert(approx_equal(mx,m3,"absdiff",0.002));
}

TEST(SnakeModificationPattern_Output__Case1_Test, SnakeModificationPattern_Output__Case1_Test_Correct) {
    
   mat mx = {{4.0000,13.0000},
   {10.0000,15.0000},
   {10.0000,15.0000},
   {10.0000,15.0000},
   {12.0000,8.0000},
   {12.0000,8.0000},
   {10.0000,15.0000},
   {12.0000,8.0000},
   {10.0000,15.0000},
   {10.0000,15.0000},
   {12.0000,8.0000},
   {10.0000,15.0000},
   {12.0000,8.0000},
   {12.0000,8.0000},
   {10.0000,15.0000}};
    
    mat m = {{10,15},
        {12,8.},
        {4.,13.}
    }; 

    ivec i1 = {3,4,13,25,13};
    ivec i2 = {6,1,12,23,3};
    ivec i3 = {2,4,5,7};
    SnakeModificationPattern* smp = new SnakeModificationPattern(i1,i2,i3);
    mat m2 = smp->Output(m);
    assert(approx_equal(mx,m2,"absdiff",0.002));

}

TEST(HawkModificationPattern_Output__Case1_Test, HawkModificationPattern_Output__Case1_Test_Correct) {

    mat m = {{10,15},
        {12,8.},
        {4.,13.}
    }; 
    ivec i1 = {2,4,3};
    ivec i2 = {1,3,7};
    HawkModificationPattern* hmp = new HawkModificationPattern(i1,i2);

    mat m2 = hmp->Output(m);

    mat mx = {{9.0000,14.0000},
   {13.0000,9.0000},
    {3.0000,12.0000}};

    assert(approx_equal(mx,m2,"absdiff",0.002));
}