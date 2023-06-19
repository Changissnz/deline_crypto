#include "deline.hpp"
#include "data_reader.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(Deline_Delineate__Equals__Case1_Test, Deline_Delineate__Equals__Case1_Test_Correct) {
    mat d1 = basic_matrix_read("t2.csv");
    Deline22 d = Deline22(d1);
    d.Preprocess();
    d.Delineate("nojag",true);

    d1 = basic_matrix_read("t1.csv");
    Delineator d2 = Delineator(d1,true,"nojag");
    d2.CROps();

    assert(approx_equal(d.del->edgeL,d2.edgeL,"absdiff", 0.002)); 
    assert(approx_equal(d.del->edgeR,d2.edgeR,"absdiff", 0.002)); 
    assert(approx_equal(d.del->edgeT,d2.edgeT,"absdiff", 0.002)); 
    assert(approx_equal(d.del->edgeB,d2.edgeB,"absdiff", 0.002)); 
}