#include "utgs.hpp"
#include <gtest/gtest.h>
using namespace std;

UTGraph UTGraphSwapCase1DOT1() {
    UTGraph utg1 = UTGraph();
    utg1.AddNode(make_pair("n1","n13"));

    utg1.AddEdge(make_pair("n1","n13"),make_pair("n3","n9"));

    utg1.AddEdge(make_pair("n2","n15"),make_pair("n3","n9"));
    utg1.AddEdge(make_pair("n2","n15"),make_pair("n11","n1"));

    utg1.AddEdge(make_pair("n3","n9"),make_pair("n4","n12"));
    utg1.AddEdge(make_pair("n3","n9"),make_pair("n7","n3"));

    utg1.AddEdge(make_pair("n4","n12"),make_pair("n5","n16"));

    utg1.AddEdge(make_pair("n5","n16"),make_pair("n6","n10"));

    utg1.AddEdge(make_pair("n6","n10"),make_pair("n7","n3"));
    utg1.AddEdge(make_pair("n6","n10"),make_pair("n8","n4"));

    utg1.AddEdge(make_pair("n7","n3"),make_pair("n9","n17"));

    utg1.AddEdge(make_pair("n9","n17"),make_pair("n10","n7"));
    utg1.AddEdge(make_pair("n9","n17"),make_pair("n18","n8"));

    utg1.AddEdge(make_pair("n10","n7"),make_pair("n11","n1"));
    utg1.AddEdge(make_pair("n10","n7"),make_pair("n12","n19"));

    utg1.AddEdge(make_pair("n12","n19"),make_pair("n16","n6"));

    utg1.AddEdge(make_pair("n13","n5"),make_pair("n14","n11"));
    utg1.AddEdge(make_pair("n13","n5"),make_pair("n15","n2"));

    utg1.AddEdge(make_pair("n14","n11"),make_pair("n18","n8"));

    utg1.AddEdge(make_pair("n15","n2"),make_pair("n17","n14"));

    utg1.AddEdge(make_pair("n16","n6"),make_pair("n17","n14"));

    utg1.AddEdge(make_pair("n18","n8"),make_pair("n19","n18"));

    return utg1;
}


TEST(UTGSwapper_InitialRunTypeRoute_Case1DOT1_CompletionTest, UTGSwapper_InitialRunTypeRoute_Case1DOT1_CompletionTestCorrect) {
UTGraph utg = UTGraphSwapCase1DOT1();
    LCG lc = LCG(3,4,1,1000);
    bool record = true;

    UTGraph* utg1;
    utg1 = &utg;
    LCG* lc1 = &lc; 
    UTGSwapper utgs = UTGSwapper(utg1,lc1,record);
    utgs.Preprocess();
    utgs.InitialRunTypeRoute();

    for (auto n : utg1->nodes) {
        assert((utgs.usi)->BijectiveTokenDistance(n.first) == 0);
    }
}

TEST(UTGSwapper_InitialRunTypeRoute_Case1DOT1_PerformanceTest, UTGSwapper_InitialRunTypeRoute_Case1DOT1_PerformanceTestCorrect) {

    LCG* lcg = new LCG(3,6,12,500);
    UTGraph utg = UTGraphSwapCase1DOT1();
    UTGSwapper* utgs = new UTGSwapper(&utg,lcg,true);
    utgs->Preprocess();
    utgs->InitialRunTypeRoute();
    cout << "SWAP COUNT: " << ((utgs->usi)->sh)->swap_count << endl;
    assert(((utgs->usi)->sh)->swap_count == 83);
    // ((utgs->usi)->sh)->Summarize(true);
}

TEST(UTGSwapper_SwapOneTypeRoute_Case1DOT1_PerformanceTest, UTGSwapper_SwapOneTypeRoute_Case1DOT1_PerformanceTest) {

    LCG* lcg = new LCG(3,6,12,500);
    UTGraph utg = UTGraphSwapCase1DOT1();
    UTGSwapper* utgs = new UTGSwapper(&utg,lcg,true);
    utgs->Preprocess();
    utgs->PreloadInitialRunTypeRoute();
    while (!utgs->type_route_stat) {
        utgs->SwapOneTypeRoute();
    }
    assert(((utgs->usi)->sh)->swap_count == 85);
}
