#include "data_reader.hpp"
#include "dmd_traveller.hpp"
#include <gtest/gtest.h>
using namespace std;

bool IsGreaterEdge(mat m1,mat m2,int axis, bool l) {
    assert (m1.n_rows == m2.n_rows);
    
    colvec c1 = m1.col(axis);
    colvec c2 = m2.col(axis);
    uvec c3;
    if (!l) {
        c3 = unique(c1 >= c2);
    } else {
        c3 = unique(c1 <= c2);
    }

    if (c3.size() != 1) {
        return false;
    }

    if (c3(0) == 1) {
        return true;
    }

    return false;
}

bool CompareAlteredDeline22s(Deline22* d1,Deline22* d2) {

    int x = -1;

    mat m1 = (d1->del)->Edge('l');
    mat m2 = (d2->del)->Edge('l');
    bool l = IsGreaterEdge(m1,m2,0,true);

    m1 = (d1->del)->Edge('r');
    m2 = (d2->del)->Edge('r');
    bool r = IsGreaterEdge(m1,m2,0,false);

    m1 = (d1->del)->Edge('t');
    m2 = (d2->del)->Edge('t');
    bool t = IsGreaterEdge(m1,m2,1,false);

    m1 = (d1->del)->Edge('b');
    m2 = (d2->del)->Edge('b');
    bool b = IsGreaterEdge(m1,m2,1,true);

    return l && r && t && b;
}

TEST(DMDTraveller_AlterDeline22__Case1_Test, DMDTraveller_AlterDeline22__Case1_Test_Correct) {

    StdRandGenerator* srg = new StdRandGenerator(9);
    mat m2 = basic_matrix_read("t8.csv"); 

    vector<pair<string,bool>> x1 = {
        make_pair("nojag",true),
        make_pair("nocross",true),        
        make_pair("nojag",false),
        make_pair("nodup",false)};
    
    vector<int> vi = {1,2};
    DelineInstr* di = new DelineInstr(x1,vi);
    DelineInstr* di2 = new DelineInstr(x1,vi);

    DelineMD* dmd = new DelineMD(m2, di, 1);
    dmd->DelineateFull();
    
    DelineMD* dmd2 = new DelineMD(m2, di2, 1);
    dmd2->DelineateFull();

    Deline22MC dmc = (dmd->dxs)[0][0];
    Deline22* d22 = dmc.dx22s[0];

    DMDTraveller* dt = new DMDTraveller(dmd, srg);
    dt->AlterDeline22(d22, make_pair(17.5,20.), true);

    Deline22MC dmc2 = (dmd2->dxs)[0][0];
    Deline22* d222 = dmc2.dx22s[0];

    assert(d22->IsSuperSetOf(d222));
    assert(!d222->IsSuperSetOf(d22));
}

TEST(DMDTraveller_AlterDeline22__Case2_Test, DMDTraveller_AlterDeline22__Case2_Test_Correct) {
    StdRandGenerator* srg = new StdRandGenerator(9);
    mat m2 = basic_matrix_read("t8.csv"); 

    vector<pair<string,bool>> x1 = {
        make_pair("nojag",true),
        make_pair("nocross",true),        
        make_pair("nojag",false),
        make_pair("nodup",false)};
    
    vector<int> vi = {1,2};
    DelineInstr* di = new DelineInstr(x1,vi);
    DelineInstr* di2 = new DelineInstr(x1,vi);

    DelineMD* dmd = new DelineMD(m2, di, 1);
    dmd->DelineateFull();
    
    DelineMD* dmd2 = new DelineMD(m2, di2, 1);
    dmd2->DelineateFull();

    Deline22MC dmc = (dmd->dxs)[0][0];
    Deline22* d22 = dmc.dx22s[0];

    DMDTraveller* dt = new DMDTraveller(dmd, srg);
    dt->AlterDeline22(d22, make_pair(2.,4.), false);

    Deline22MC dmc2 = (dmd2->dxs)[0][0];
    Deline22* d222 = dmc2.dx22s[0];

    assert (!CompareAlteredDeline22s(d22,d222));    
}
