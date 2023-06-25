/// mask functions and their helpers
#ifndef ACCESSORYFUNCTIONS_HPP
#define ACCESSORYFUNCTIONS_HPP

#include "randumb.hpp"
#include <vector>
#include <string>

////////////////////////////////

class FoxModificationPattern {
public: 
    // indices to insert|modify
    LCG* l1;
    // modify indices, reset for every call
    LCG* l2;
    // boolean: add or multiply
    LCG* l3;
    // boolean: insert or modify at activated index
    LCG* l4;
    // number to use in modification
    LCG* l5; 

    //ivec l1da;
    ivec l2da;

    //ivec l5da;
    std::vector<int> v1;

    /// l := number of rows of the target matrix
    /// l1da := size 3
    /// l2da := size 3 
    /// l3da := size 3
    /// l4da := size 3
    /// l5da :=  size 4
    FoxModificationPattern(int l, ivec l1da,ivec l2da, ivec l3da, ivec l4da,ivec l5da) {

        // check the index length of each
        assert (l1da.size() == 3);
        assert (l2da.size() == 3);
        assert (l3da.size() == 3);
        assert (l4da.size() == 3);
        assert (l5da.size() == 4);

        l1 = new LCG(int(l1da(0) % l),int(l1da(1)),int(l1da(2)),l);
        l3 = new LCG(int(l3da(0) % 2),int(l3da(1)),int(l3da(2)),2);
        l4 = new LCG(int(l4da(0) % 2),int(l4da(1)),int(l4da(2)),2);
        l5 = new LCG(int(l5da(0) % int(l5da(3))),int(l5da(1)),int(l5da(2)),int(l5da(3)));

        this->l2da = l2da;
        //One();
        RunGens();
    }

    void RunGens();
    mat Output(mat m,mat m2,ivec cind); 
    mat ModAtIndex(mat m2, ivec cind, int v1i);
    mat InsertionMod(mat rp);
    rowvec AMMod(mat rp);
};

class SnakeModificationPattern {

public:

    // repeater
    LCG* l1;
    // palindromic repeater
    LCG* l2;
    // permutation
    LCG* l3;
    int s1;
    int s2;

    ////X 
    int s3;

    ivec r3;

    std::vector<int> v1;
    std::vector<int> v2;
    std::vector<int> v3;

    SnakeModificationPattern(ivec r1, ivec r2, ivec r3) {
        assert (r1.size() == r2.size());
        assert(r1.size() == 5);
        assert(r3.size() == 4);

        l1 = new LCG(int(r1(0)),int(r1(1)),int(r1(2)),int(r1(3)));
        l2 = new LCG(int(r2(0)),int(r2(1)),int(r2(2)),int(r2(3)));
        this->r3 = r3;

        s1 = r1(4);
        s2 = r2(4);
        s3 = r3(3);
    }

    void RunGens();
    mat Output(mat m); 
    mat ModAtIndex(mat m, int i);
    mat Repeater(mat m, int i,bool pmode);
};

//// TODO: Hawk
class HawkModificationPattern {

public:

    LCG* l1;
    LCG* l2;
    ivec v2;

    HawkModificationPattern(ivec r1, ivec r2) {
        assert (r1.size() == r2.size());
        assert (r1.size() == 3);
        l1 = new LCG(int(r1(0)),int(r1(1)),int(r1(2)),5);
        v2 = r2;
    }

    mat Output(mat m);
};


////////////////////////////////////////////////////////

std::vector<ivec> ParseStringToIVecs(std::string smp_args,ivec indices);

// m1 := matrix of points travelled
// m2 := matrix of points considered
mat FoxMask(mat m1, mat m2, ivec iv, std::string smp_args);
mat SnakeMask(mat m, std::string smp_args);
mat HawkMask(mat m1, std::string smp_args);

#endif