#include "deline_md.hpp"
#include "data_reader.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(Deline22MD_DelineateFull__TerminationCase1_Test, Deline22MD_DelineateFull__TerminationCase1_Test_Correct) { 

    mat m2 = basic_matrix_read("t5.csv"); 

    vector<pair<string,bool>> x1 = {
        make_pair("nojag",true),
        make_pair("nocross",true),        
        make_pair("nojag",false),
        make_pair("nodup",false)};
    
    vector<int> vi = {1,2};
    DelineInstr* di = new DelineInstr(x1,vi);

    DelineMD dmd = DelineMD(m2, di, 1);
    dmd.DelineateFull();
}