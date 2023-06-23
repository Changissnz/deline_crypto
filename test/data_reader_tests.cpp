#include "data_reader.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(DataReader_ReadNextChar__Case1_Test, DataReader_ReadNextChar__Case1_Test_Correct) {
    DataReader dr = DataReader("yoquistas.txt",20);
    string x = string(dr.ReadNextChar(3));
    //assert (x == "Thi");
    //assert(!string(x).compare("Thi"));

    char* x2 = dr.ReadNextChar(4);
    string x3 = string(x2).substr(0,4);
    assert (string(x3) == string("s is"));
    dr.inp.close();
}

TEST(DataReader_ReadNextLine__Case1_Test, DataReader_ReadNextLine__Case1_Test_Correct) {
    DataReader dr = DataReader("yoquistas.txt",20);
    string l = dr.ReadNextLine();    
    assert(l == "This is a letterianco about Mary Watters, a much beloved");
    dr.inp.close();
}

TEST(DataReader_BatchNConversion__Case1_Test, DataReader_BatchNConversion__Case1_Test_Correct) {

    DataReader dr = DataReader("t1.csv",2);
    dr.ReadEntire();
    dr.BatchNConversion();

    mat mt = {{5.0,15.0,0.0},
        {5.0,12.0,0.0},
        {5.0,9.0,0.0},
        {5.0,6.0,0.0},
        {5.0,0.0,0.0},
        {15.0,12.0,0.0},
        {15.0,9.0,0.0},
        {20.0,15.0,0.0},
        {20.0,0.0,0.0},
        {25.0,7.5,0.0}};

    assert (approx_equal(dr.ndata,mt,"absdiff",0.00002));
    dr.inp.close();
}