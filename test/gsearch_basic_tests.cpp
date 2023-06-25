
#include "gsearch_basic.hpp"
#include <gtest/gtest.h>
using namespace std;

UTGraph UTGraphN1() {
    UTGraph utg1 = UTGraph();
    utg1.AddNode(make_pair("n0","t"));

    utg1.AddEdge(make_pair("n0","t"),make_pair("n1","t"));
    utg1.AddEdge(make_pair("n0","t"),make_pair("n2","t"));

    utg1.AddEdge(make_pair("n1","t"),make_pair("n3","t"));
    utg1.AddEdge(make_pair("n1","t"),make_pair("n4","t"));

    utg1.AddEdge(make_pair("n2","t"),make_pair("n3","t"));

    utg1.AddEdge(make_pair("n4","t"),make_pair("n5","t"));
    utg1.AddEdge(make_pair("n4","t"),make_pair("n6","t"));
    utg1.AddEdge(make_pair("n6","t"),make_pair("n7","t"));

    utg1.AddEdge(make_pair("n7","t"),make_pair("n6","t"));
    return utg1;
}


UTGraph UTGraphN2() {
    UTGraph utg1 = UTGraph();
    utg1.AddNode(make_pair("n0","t"));

    utg1.AddEdge(make_pair("n0","t"),make_pair("n2","t"));
    utg1.AddEdge(make_pair("n0","t"),make_pair("n3","t"));
    utg1.AddEdge(make_pair("n0","t"),make_pair("n4","t"));
    utg1.AddEdge(make_pair("n0","t"),make_pair("n7","t"));

    utg1.AddEdge(make_pair("n1","t"),make_pair("n2","t"));
    utg1.AddEdge(make_pair("n1","t"),make_pair("n3","t"));

    utg1.AddEdge(make_pair("n2","t"),make_pair("n9","t"));
    utg1.AddEdge(make_pair("n2","t"),make_pair("n0","t"));

    utg1.AddEdge(make_pair("n4","t"),make_pair("n5","t"));
    utg1.AddEdge(make_pair("n4","t"),make_pair("n6","t"));

    utg1.AddEdge(make_pair("n5","t"),make_pair("n6","t"));
    utg1.AddEdge(make_pair("n5","t"),make_pair("n8","t"));

    utg1.AddEdge(make_pair("n6","t"),make_pair("n8","t"));
    return utg1;
}

TEST(GSearch_BFSOnNodeBestMPathsPerNode_Case1_Test,GSearch_BFSOnNodeBestMPathsPerNode_Case1_Test_Correct) {

    UTGraph utg1 = UTGraphN1();
    GSearch gs = GSearch(&utg1);
    auto res = gs.BFSOnNodeBestMPathsPerNode("n0",2);

    cout << res->ToString();
    string s = "best paths for: n0\n\
size: 1\n\
node id: n0  parent id:  distance: 0.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n1\n\
size: 1\n\
node id: n1  parent id: n0 distance: 1.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n2\n\
size: 1\n\
node id: n2  parent id: n0 distance: 1.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n3\n\
size: 2\n\
node id: n3  parent id: n2 distance: 2.000000\n\
\n\
node id: n3  parent id: n1 distance: 2.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n4\n\
size: 1\n\
node id: n4  parent id: n1 distance: 2.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n5\n\
size: 1\n\
node id: n5  parent id: n4 distance: 3.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n6\n\
size: 1\n\
node id: n6  parent id: n4 distance: 3.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n7\n\
size: 1\n\
node id: n7  parent id: n6 distance: 4.000000\n\
\n\
%&&%&&%&&\n\
\n\
";
    assert(res->ToString() == s);
}

TEST(GSearch_BFSOnNodeBestMPathsPerNode_Case2_Test,GSearch_BFSOnNodeBestMPathsPerNode_Case2_Test_Correct) {

    UTGraph utg1 = UTGraphN2();

    GSearch gs = GSearch(&utg1);
    auto res = gs.BFSOnNodeBestMPathsPerNode("n0",5);

    string s = "best paths for: n0\n\
size: 1\n\
node id: n0  parent id:  distance: 0.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n1\n\
size: 2\n\
node id: n1  parent id: n3 distance: 2.000000\n\
\n\
node id: n1  parent id: n2 distance: 2.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n2\n\
size: 1\n\
node id: n2  parent id: n0 distance: 1.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n3\n\
size: 1\n\
node id: n3  parent id: n0 distance: 1.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n4\n\
size: 1\n\
node id: n4  parent id: n0 distance: 1.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n5\n\
size: 1\n\
node id: n5  parent id: n4 distance: 2.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n6\n\
size: 2\n\
node id: n6  parent id: n5 distance: 3.000000\n\
\n\
node id: n6  parent id: n4 distance: 2.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n7\n\
size: 1\n\
node id: n7  parent id: n0 distance: 1.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n8\n\
size: 2\n\
node id: n8  parent id: n6 distance: 3.000000\n\
\n\
node id: n8  parent id: n5 distance: 3.000000\n\
\n\
%&&%&&%&&\n\
\n\
best paths for: n9\n\
size: 1\n\
node id: n9  parent id: n2 distance: 2.000000\n\
\n\
%&&%&&%&&\n\
\n\
";

    assert(res->ToString() == s);

    
}

TEST(GInfo_EccentricitiesANDCenter_Case1_Test,GInfo_Eccentricities_Case1_Test_Correct) {
    UTGraph utg1 = UTGraphN2();
    GSearch gs = GSearch(&utg1);
    auto res = gs.BFSOnNodeBestMPathsPerNode("n0",5);

    cout << res->ToString();
    GInfo* gi = gs.RunGInfo();
    
    auto gec = gi->Eccentricities();

    map<string,float> gec_ans;
    gec_ans["n0"] = 3;
    gec_ans["n1"] = 5;
    gec_ans["n2"] = 4;
    gec_ans["n3"] = 4;
    gec_ans["n4"] = 3;
    gec_ans["n5"] = 4;
    gec_ans["n6"] = 4;
    gec_ans["n7"] = 4;
    gec_ans["n8"] = 5;
    gec_ans["n9"] = 5;

    for (auto ga: gec) {
        assert (ga.second == gec_ans[ga.first]);
    }

    auto centers = gi->Center();
    set<string> cc = {"n0","n4"};

    assert (centers == cc);
}

TEST(GInfo_BackTrace_Case1_Test,GInfo_BackTrace_Case1_Test_Correct) {
    UTGraph utg1 = UTGraphN2();
   GSearch gs = GSearch(&utg1);
    GInfo* gi = gs.RunGInfo();

    // do backtrace
    BestPaths* bp = gi->nbp["n0"];
    deque<GPath> dgp = bp->BackTrace("n8");

    GPath g1 = {"n0","n4","n5","n8"};
    GPath g2 = {"n0","n4","n6","n8"};
    GPath g3 = {"n0","n4","n5", "n6","n8"};

    assert (g1 == dgp[0]);
    assert (g2 == dgp[1]);
    assert (g3 == dgp[2]);
}

TEST(BestPaths_BestPathsByNodesetComplement_Case1_Test,BestPaths_BestPathsByNodesetComplement_Case1_Test_Correct) {
    UTGraph utg1 = UTGraphN2();

    GSearch gs = GSearch(&utg1);
    auto res = gs.BFSOnNodeBestMPathsPerNode("n8",5);
    auto res2 = res->BestPathsByNodesetComplement({"n7","n4"});

    
    set<string> ns1;
    for (auto rss: res2->bp) {
        ns1.insert(rss.first);
    }
    set<string> ns1sol = {"n5","n6","n8"};
    assert((ns1 == ns1sol));

    auto res3 = res->BestPathsByNodesetComplement({"n2","n3"});
    set<string> ns2;
    for (auto rss: res3->bp) {
        ns2.insert(rss.first);
    }
        
    set<string> ns2sol = {"n0","n4","n5","n6","n7","n8"};
    assert((ns2 == ns2sol));
    

    GSearch gs2 = GSearch(&utg1);
    res = gs2.BFSOnNodeBestMPathsPerNode("n9",5);
    res2 = res->BestPathsByNodesetComplement({"n1","n2"});

    ns1.clear();
    for (auto rss: res2->bp) {
        ns1.insert(rss.first);
    }

    ns1sol = {"n9"};
    assert((ns1 == ns1sol));

}