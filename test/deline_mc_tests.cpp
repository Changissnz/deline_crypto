#include "deline_mc.hpp"
#include "data_reader.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(Deline22MC_DelineateFull__TerminationCase1_Test, Deline22MC_DelineateFull__TerminationCase1_Test_Correct) { 
    mat m2 = basic_matrix_read("t4.csv");
    Deline22MC d = Deline22MC(m2);
    d.DelineateFull("nojag",true,false);
}

TEST(Deline22MC_DelineateFull__IsSuperSetOf_Test, Deline22MC_DelineateFull__IsSuperSetOf_Test_Correct) { 

    mat m2 = basic_matrix_read("t7.csv"); 
    Deline22MC dmc = Deline22MC(m2);
    dmc.DelineateFull("nojag",true,false);

    assert (dmc.dx22s.size() == 2);
    assert (!(dmc.dx22s[0]->IsSuperSetOf(dmc.dx22s[1])));
    assert (dmc.dx22s[1]->IsSuperSetOf(dmc.dx22s[0]));
}

TEST(Deline22MC__AssignHierarchy__Test, Deline22MC__AssignHierarchy__Test_Correct) { 

    mat m2 = basic_matrix_read("t7.csv"); 

    Deline22MC dmc = Deline22MC(m2);
    dmc.DelineateFull("nojag",true,false);

    // list parents and children
    set<string> s0 = {};
    set<string> s1 = {"D22_1"};
    assert ((dmc.dx22s[0]->dh).parent_names == s1); 
    assert ((dmc.dx22s[0]->dh).child_names == s0); 

    set<string> s2 = {"D22_0"};
    assert ((dmc.dx22s[1]->dh).parent_names == s0); 
    assert ((dmc.dx22s[1]->dh).child_names == s2); 
}

TEST(Deline22MC_DelineateFull__Deline22Score_Test, Deline22MC_DelineateFull__Deline22Score_Test_Correct) { 

    mat m2 = basic_matrix_read("t7.csv"); 

    Deline22MC dmc = Deline22MC(m2);
    dmc.DelineateFull("nojag",true,false);

    assert ((dmc.dx22s[0]->ds)->ltr == make_pair(4,4));
    assert ((dmc.dx22s[0]->ds)->ntr == make_pair(5,5));

    assert ((dmc.dx22s[1]->ds)->ltr == make_pair(5,5));
    assert ((dmc.dx22s[1]->ds)->ntr == make_pair(0,0));
}