#include "gmech.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(UTGraph_WriteToFileANDReadFromFile__Case1_Test, UTGraph_WriteToFileANDReadFromFile__Case1_Test) {

    UTGraph utg1 = UTGraph();

    utg1.AddNode(make_pair("n1","t2"));

    utg1.AddEdge(make_pair("n1","t2"),make_pair("n3","t1"));
    utg1.AddEdge(make_pair("n1","t2"),make_pair("n4","t5"));

    utg1.WriteToFile("catastr.txt");
    cout << "displaying graph#2" << endl;

    UTGraph utg2 = UTGraph::FromFile("catastr.txt");
    assert(utg2.ToString() == utg1.ToString());
}