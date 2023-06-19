#include "ffact.hpp"
#include <gtest/gtest.h>
using namespace std;

UTGraph UTGraphN3() {
    UTGraph utg1 = UTGraph();
    utg1.AddNode(make_pair("n0","t"));

    utg1.AddEdge(make_pair("n0","t"),make_pair("n1","t"));
    utg1.AddEdge(make_pair("n0","t"),make_pair("n3","t"));
    utg1.AddEdge(make_pair("n0","t"),make_pair("n6","t"));

    utg1.AddEdge(make_pair("n1","t"),make_pair("n2","t"));
    utg1.AddEdge(make_pair("n1","t"),make_pair("n4","t"));

    utg1.AddEdge(make_pair("n2","t"),make_pair("n5","t"));
    utg1.AddEdge(make_pair("n2","t"),make_pair("n7","t"));

    utg1.AddEdge(make_pair("n3","t"),make_pair("n4","t"));
    utg1.AddEdge(make_pair("n3","t"),make_pair("n8","t"));

    utg1.AddEdge(make_pair("n4","t"),make_pair("n5","t"));

    utg1.AddEdge(make_pair("n5","t"),make_pair("n9","t"));

    return utg1;
}


TEST(UGModule_Case1_Test, UGModule_Case1_Test_Correct) {
    UTGraph utg1 = UTGraphN3();
    
    LCG* l = new LCG(0, 3, 6, 100);
    UGModule* ugm = new UGModule(&utg1,l,4);
    ugm->Preprocess();
    ugm->Split();
    string ugmans = "components are:\n\
* n2 n5 n7 n9 \n\
* n1 n4 \n\
* n0 n6 \n\
* n3 n8 \n";

    assert (ugm->ToString() == ugmans);
}

TEST(UTComponentGraph_ProcessUGModule_Case1_Test, UTComponentGraph_ProcessUGModule_Case1_Test_Correct) {
    UTGraph utg1 = UTGraphN3();

    LCG* l = new LCG(0, 3, 6, 100);
    UGModule* ugm = new UGModule(&utg1,l,4);
    ugm->Preprocess();
    ugm->Split();


    UTComponentGraph utcg = UTComponentGraph();
    utcg.ProcessUGModule(ugm); 

    cout << "^^^" << endl;
    cout << utcg.ToString();
    string sol = "* 0 n2 n5 n7 n9 \n\
* 1 n1 n4 \n\
* 2 n0 n6 \n\
* 3 n3 n8 \n";

    assert (sol == utcg.ToString()); 
}



