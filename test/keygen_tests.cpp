#include "keygen.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(KeyGen_Out_Case1_DemoTest, KeyGen_Out_Case1_DemoTestCorrect) {
    DMDTraveller* dmdt = DMDTravellerCase1();
    LCG* lcg = new LCG(7,8,5,1000);

    KeyGen* kg = new KeyGen(lcg,nullptr,"","",dmdt->Info());
    kg->Out();
    kg->WriteToFile(make_pair("keyc1.txt","keyr1.txt"));
}

TEST(KeyGen_Out_Case2_DemoTest, KeyGen_Out_Case2_DemoTestCorrect) {
    DMDTraveller* dmdt = DMDTravellerCase1();
    StdRandGenerator* stdrg = new StdRandGenerator(71);
    
    KeyGen* kg = new KeyGen(stdrg,nullptr,"","",dmdt->Info());
    kg->Out();
    kg->WriteToFile(make_pair("keyc2.txt","keyr2.txt"));
}
