#include "randumb.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(PermLCG__PRIntInRange_Test, PermLCG__PRIntInRange_TestCorrect) {
    set<int> si = {4,5,6,7,8,9,10};
    set<int> sj;
    PermLCG plcg = PermLCG(3, 3, 1, 10);
    while (!plcg.finished_stat) {
        int i = plcg.PRIntInRange(make_pair(4,10));

        if (!plcg.finished_stat) {
            sj.insert(i);
        }
    }

    assert(si == sj);
}

TEST(AbstractPRNG__CycleOne_Test, AbstractPRNG__CycleOne_TestCorrect) {
    /// CASE: LCG
    LCG* lcg = new LCG(11,3,2,13);
    vector<float> vf = lcg->CycleOne(false,100);
    vec ans1 = {11,9,3};
    assert(approx_equal(ans1,conv_to<vec>::from(vf),"absdiff", 0.002)); 
    vf = lcg->CycleOne(false,100);
    vec ans2 = {9,3,11};
    assert(approx_equal(ans2,conv_to<vec>::from(vf),"absdiff", 0.002)); 

    /// CASE: PermLCG
    PermLCG* plcg = new PermLCG(11,3,2,13);
    plcg->SetRangeData(make_pair(3,12)); 
    vf = plcg->CycleOne(false,100);
    vec ans3 = {11,9,5,10,8,4,12,7,6,3};
    assert(approx_equal(ans3,conv_to<vec>::from(vf),"absdiff", 0.002));

    /// CASE: LCG2H
    ivec i1 = {3,4,5,43};
    ivec i2 = {3,5,1,97};
    LCG2H* lcg2h = new LCG2H(i1,i2,10);
    vf = lcg2h->CycleOne(false,100);
    assert(vf.size() == 100);

    /// CASE: StdRandGenerator
    StdRandGenerator* srg = new StdRandGenerator(10);
    srg->pfr = make_pair(50,100);
    
    vf = srg->CycleOne(false,100);
    assert(vf.size() > 9);
}