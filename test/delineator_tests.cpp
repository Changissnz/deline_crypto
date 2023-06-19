#include "delineator.hpp"
#include "data_reader.hpp"
#include <gtest/gtest.h>
using namespace std;

mat Delineator_Dataset1() {
    DataReader dr = DataReader("t1.csv",2);
    dr.ReadEntire();
    dr.BatchNConversion();
    return dr.ndata;
}

TEST(Delineator_CarveEdge__Case1_Test, Delineator_CarveEdge__Case1_Test_Correct) {
    //mat data = Delineator_Dataset1();
    mat data = basic_matrix_read("t1.csv");
    Delineator d = Delineator(data,true,"nocross");

    d.CarveEdge('t');
    d.CarveEdge('b');

    mat T = {{5.0000,15.0000,0},
    {15.0000,12.0000,0},
    {20.0000,15.0000,0},
    {25.0000,7.5000,0}};

    mat B =  {{5.0000,0,0},
   {15.0000,9.0000,0},
   {20.0000,0,0},
   {25.0000,7.5000,0}};

   assert (approx_equal(T, d.edgeT, "absdiff", 0.002)); 
   assert (approx_equal(B, d.edgeB, "absdiff", 0.002)); 
}

/// data: t1.csv
/// method: nocross
TEST(Delineator_CROps__Case11_Test, Delineator_CROps__Case11_TestCorrect) {
    mat data = Delineator_Dataset1();

    Delineator d = Delineator(data,true,"nocross");
    d.CROps(); 
    mat L = {{5.0000,0,0},
    {5.0000,6.0000,0},
    {5.0000,9.0000,0},
    {5.0000,12.0000,0},
    {5.0000,15.0000,0}};

    mat R = {{20.0000,0,0},
   {25.0000,7.5000,0},
   {15.0000,9.0000,0},
   {15.0000,12.0000,0},
   {20.0000,15.0000,0}};

    mat T = {{5.0000,15.0000,0},
   {15.0000,12.0000,0},
   {20.0000,15.0000,0}};

    mat B = {{5.0000,0,0},
   {15.0000,9.0000,0},
   {20.0000,0,0},
   {25.0000,7.5000,0}};

    assert (approx_equal(L, d.edgeL, "absdiff", 0.002)); 
    assert (approx_equal(R, d.edgeR, "absdiff", 0.002)); 
    assert (approx_equal(T, d.edgeT, "absdiff", 0.002)); 
    assert (approx_equal(B, d.edgeB, "absdiff", 0.002)); 

}

/// data: t1.csv
/// method: nojag
TEST(Delineator_CROps__Case12_Test, Delineator_CROps__Case12_TestCorrect) {
    mat data = Delineator_Dataset1();

    Delineator d = Delineator(data,true,"nojag");
    d.CROps(); 

    mat L = {{5.0000,0,0},
    {5.0000,6.0000,0},
    {5.0000,9.0000,0},
    {5.0000,12.0000,0},
    {5.0000,15.0000,0}};

    mat R =  {{20.0000,0,0},
   {25.000,7.5000,0},
   {20.0000,15.0000,0}};

    mat T = {{5.0000,15.0000,0},
   {20.0000,15.0000,0},
   {25.0000,7.5000,0}}; 

    mat B = {{5.0000,0,0},
   {20.0000,0,0},
   {25.0000,7.5000,0}};

    assert (approx_equal(L, d.edgeL, "absdiff", 0.002)); 
    assert (approx_equal(R, d.edgeR, "absdiff", 0.002)); 
    assert (approx_equal(T, d.edgeT, "absdiff", 0.002)); 
    assert (approx_equal(B, d.edgeB, "absdiff", 0.002)); 
}

/// data: t1.csv
/// method: nodup
TEST(Delineator_CROps__Case13_Test, Delineator_CROps__Case13_TestCorrect) {
    mat data = Delineator_Dataset1();

    Delineator d = Delineator(data,true,"nodup");
    d.CROps(); 
    mat L = {{5.0000,0,0},
    {5.0000,6.0000,0},
    {5.0000,9.0000,0},
    {5.0000,12.0000,0},
    {5.0000,15.0000,0}};

    mat R = {{20.0000,0,0},
   {25.0000,7.5000,0},
   {15.0000,9.0000,0},
   {15.0000,12.0000,0},
   {20.0000,15.0000,0}};

    mat T = {{5.0000,15.0000,0},
   {15.0000,12.0000,0},
   {20.0000,15.0000,0}};

    mat B = {{5.0000,0,0},
   {15.0000,9.0000,0},
   {20.0000,0,0},
   {25.0000,7.5000,0}};

    assert (approx_equal(L, d.edgeL, "absdiff", 0.002)); 
    assert (approx_equal(R, d.edgeR, "absdiff", 0.002)); 
    assert (approx_equal(T, d.edgeT, "absdiff", 0.002)); 
    assert (approx_equal(B, d.edgeB, "absdiff", 0.002)); 

}
