#include "dinst.hpp"
#include <gtest/gtest.h>
using namespace std;

/// DMDTraveller samples

DMDTraveller* DMDTravellerCase1() {
    StdRandGenerator* srg = new StdRandGenerator(9);
    mat m2 = basic_matrix_read("t6.csv"); 

    vector<pair<string,bool>> x1 = {
        make_pair("nojag",true),
        make_pair("nocross",true),        
        make_pair("nojag",false),
        make_pair("nodup",false)};
    
    vector<int> vi = {1,2};
    DelineInstr* di = new DelineInstr(x1,vi);
    DelineMD* dmd = new DelineMD(m2, di, true);
    dmd->DelineateFull();

    LCG* lcg = new LCG(4,5,6,7);
    return new DMDTraveller(dmd, lcg);
}


/////////////////////////////////////////////////////////

/// NOTE: the tests below demonstrate usage of <DInst> alongside
///       <DMDTraveller> and do not use any assertions.

TEST(DInst_MapMO__Case1_DemoTest, DInst_MapMO__Case1_DemoTest_Correct) {
    DMDTraveller* dt = DMDTravellerCase1();
    
    string inst = "MO";
    /// possible commands:
    /// CAUTION: for proper usage, use lower case!
    vector<string> vs = {"5","2","3","L.R.T.B.B.R","0.5_1_0.75_0.5_1_0.2","0"};
///    vector<string> vs = {"5","2","3","b.r.r.b.b.r","0.5_1_0.75_0.5_1_0.2","0"};
///    vector<string> vs = {"5","0","3","L.R.T.B.B.R","5._1_0.12_5_1._0.2","0"};
///    vector<string> vs = {"5","0","3","T.L.","5._1.75","0"};
///    vector<string> vs = {"5","0","3","b.r.b","5._1.75_0.2","0"}; // "1._2._1."
///    vector<string> vs = {"5","0","3","r",".5","0"};

    DInst dinst = DInst(inst,vs);
    dinst.MapMO(dt);
    cout << "travel points" << endl;
    cout << dt->travel_points << endl; 
}

TEST(DInst_MapBG__Case1_DemoTest, DInst_MapBG__Case1_DemoTest_Correct) {
    /// case 1:
    DMDTraveller* dt = DMDTravellerCase1();
    string inst = "BG";
    vector<string> vs = {"ALTER","5","2","4","-0.75_-5.0"};

    DInst dinst = DInst(inst,vs);
    dinst.MapBG(dt);

    Deline22* d22ss = dt->SelectD22(2,4);

    cout << "MATRIX AFTER #1" << endl;
    d22ss->Display();

    /// case 2:
    DMDTraveller* dt2 = DMDTravellerCase1();
    string inst2 = "BG";
    vector<string> vs2 = {"JITTER","5","2","4","-0.75_-5.0"};

    DInst dinst2 = DInst(inst2,vs2);
    dinst2.MapBG(dt);
    d22ss = dt2->SelectD22(2,4);
    cout << "MATRIX AFTER #2" << endl;
    d22ss->Display();
}

TEST(DInst_MapMSK_FOX__Case1_DemoTest, DInst_MapMSK_FOX__Case1_DemoTest) {

    /// CASE: w/o mask
    DMDTraveller* dt = DMDTravellerCase1();
    vector<string> comm = {"MSK,FOX,3_4_13_6_1_12_2_4_5_10_7_3_3_2_3_25",
    "MO,3,2,3,L.R.T.B.B.R,0.5_1_0.75_0.5_1_0.15,1",
    "MO,5,3,5,T.B.B.R.L,2_1_0.5_0.25_0.1,0",
    "MO,7,4,2,T.B.T.R,0.3_0.7_0.6_0.8,0"};

    DInstSeq dis = DInstSeq(comm);
    dis.ParseIntoDInst();
    dis.Map(dt);
    cout << "OUTPUT MATRIX #1" << endl;
    cout << dis.output << endl;

    /// CASE: w/ mask
    DMDTraveller* dt2 = DMDTravellerCase1();
    vector<string> comm2 = {"MSK,FOX,3_4_13_6_1_12_2_4_5_10_7_3_3_2_3_25",
    "HOP,3,5,lcg 1 6 3 17",
    "MO,3,2,3,L.R.T.B.B.R,0.5_1_0.75_0.5_1_0.15,1",
    "MO,5,3,5,T.B.B.R.L,2_1_0.5_0.25_0.1,0",
    "MO,7,4,2,T.B.T.R,0.3_0.7_0.6_0.8,0"};
    DInstSeq dis2 = DInstSeq(comm2);
    dis2.ParseIntoDInst();
    dis2.Map(dt);
    cout << "OUTPUT MATRIX #2" << endl;
    cout << dis2.output << endl;

    /// alternative code
    /*
    ostringstream ss;
    dis.output.raw_print(ss); 
    cout << ss.str() << endl;
    */
}

/*
/// print-test
TEST(DInstSeq_Map__Case1_DemoTest, DInstSeq_Map__Case1_DemoTest_Correct) {

    vector<string> comm = {"MSK,FOX,3_4_13_6_1_12_2_4_5_10_7_3_3_2_3_25",
    "MO,3,2,3,L.R.T.B.B.R,0.5_1_0.75_0.5_1_0.15,1",
    "MO,5,3,5,T.B.B.R.L,2_1_0.5_0.25_0.1,0",
    "MO,7,4,2,T.B.T.R,0.3_0.7_0.6_0.8,0"};

    DInstSeq dis = DInstSeq(comm);
    dis.ParseIntoDInst();

    cout << "OUTPUT MATRIX" << endl;

    ostringstream ss;

    dis.output.raw_print(ss); 
    cout << ss.str() << endl;

    assert(true);
}
*/