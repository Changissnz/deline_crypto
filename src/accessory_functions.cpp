#include "accessory_functions.hpp"
using namespace std;

void FoxModificationPattern::RunGens() {
    bool stat = true;
    int prev = 0;
    int post = 0;
    while (stat) {
        prev = post;
        post = l1->DefaultPRIntInRange();
        stat = !(post < prev);

        if (stat) {
            v1.push_back(post);
        }
    }
}

/// outputs the matrix mapped from `m`. The pattern goes as such:
///
///     Declare an empty matrix m2.
///     Iterate through each row of `m`.
///     For each of the rows `r`, get the submatrix in `m2`
///         corresponding to it based on the indexer `cind`.
///         
///         If the index `i` of of `r` is found in `v1`, insert the modification
///             `ModAtIndex(m2,cind,i)` into m2. Otherwise, insert `r` into m2.
mat FoxModificationPattern::Output(mat m,mat m2,ivec cind) {
    mat mx(0,0,fill::zeros);

    int j = 0;
    for (int i = 0; i < m.n_rows; i++) {
        if (v1[j] == i) {
            mx.insert_rows(mx.n_rows,ModAtIndex(m2,cind,i));
            j += 1;
        } else {
            mx.insert_rows(mx.n_rows,m.row(i));
        }
    }

    return mx;
}

/// modifies the matrix `m2` in the segment `cind(vli -1,vli)`
/// depending on boolean generator `l4`.
/// If `l4` outputs 0, then modify by InsertionMod. Otherwise, modify by AMMod.
mat FoxModificationPattern::ModAtIndex(mat m2,ivec cind, int v1i) {
    // get the max and min indices of m2
    int minu,maxi;
    if (v1i == 0) {
        minu = 0;
    } else {
        minu = cind(v1i - 1);
    }

    maxi = cind(v1i) - 1;
    mat m3 = IndiceRangeToSubArmaMat(m2,make_pair(minu,maxi));

    if (l4->DefaultPRIntInRange() == 0) {
        return InsertionMod(m3);
    }

    rowvec rv = AMMod(m3);
    mat mx(0,0,fill::zeros);
    mx.insert_rows(0,rv);
    return mx;
}

/// Inserts `q` elements selected by the `l2` generator into an empty matrix,
/// where `q` is a number outputted by `l5`. 
mat FoxModificationPattern::InsertionMod(mat rp) {
    int q = l5->DefaultPRIntInRange();
    l2 = new LCG(int(l2da(0) / rp.n_rows),int(l2da(1)),int(l2da(2)),rp.n_rows);
    mat mx(0,0,fill::zeros);

    for (int i = 0; i < q; i++) { 
        mx.insert_rows(mx.n_rows,rp.row(l2->DefaultPRIntInRange()));
    }

    return mx;
}

/// add/mult mod
/// TODO: 
rowvec FoxModificationPattern::AMMod(mat rp) {
    int q = l5->DefaultPRIntInRange();
    l2 = new LCG(int(l2da(0) / rp.n_rows),int(l2da(1)),int(l2da(2)),rp.n_rows);
    rowvec r = rp.row(l2->DefaultPRIntInRange());
    rowvec r2;
    for (int i = 1; i < q; i++) {
        r2 = rp.row(l2->DefaultPRIntInRange());

        if (l3->DefaultPRIntInRange() == 1) {
            r = Str8MultOnVecs(r,r2);
            continue;
        }
        r = r + r2;
    }

    return r;
}

////////////////////////////////////////////////////////////////////////////////////


/// preprocessing function before calling `Output`. 
void SnakeModificationPattern::RunGens() {

    for (int i = 0; i < s1; i++) {
        v1.push_back(l1->DefaultPRIntInRange());
    }

    for (int i = 0; i < s2; i++) {
        v2.push_back(l2->DefaultPRIntInRange());
    }

    for (int i = 0; i < s3; i++) {
        v3.push_back(l3->DefaultPRIntInRange());
    }
}


mat SnakeModificationPattern::Output(mat m) {
    l3 = new LCG(int(r3(0)),int(r3(1)),int(r3(2)),m.n_rows);
    RunGens();
    mat mx(0,0,fill::zeros);
    for (int i = 0; i < m.n_rows; i++) {
        mat mx2 = ModAtIndex(m,i);
        mx.insert_rows(mx.n_rows,mx2);
    }

    return mx;
}

/// snake modification scheme for matrix `m` at row `i`.
///         * description *
///     for the index `i`, determine its corresponding value in the
///     vectors `v1`,`v2`, and `v3`. For any vector that is size smaller
///     than `i`, set the corresponding value as -1.
///
///     Set the order R by the ordered indices of those corresponding values
///     {c0,c1,c2}, in which cx in [0,2].
///
///     For index 0, use `Repeater(m,i,false)` if c0 != -1.
///     For index 1, use `Repeater(m,i,true)` if c0 != -1.
///     For index 2, select the row at index `v3[i]` that takes values from `l3`.
///
///     Iterate through R, and for each index `j`. If `vx(j)
mat SnakeModificationPattern::ModAtIndex(mat m, int i) {
    // checks the index `i` alongside `s1`,`s2`,and `s3`. 
    // if `i` is in the scope of vx, select vx[i], otherwise -1.
    int i1 = (i < v1.size()) ? v1[i]: -1;  
    int i2 = (i < v2.size()) ? v2[i]: -1;  
    int i3 = (i < v3.size()) ? v3[i]: -1;  
    ivec iv = {i1,i2,i3};
    
    /// get the sorted indices
    uvec uv = sort_index(iv);

    mat mx(0,0,fill::zeros);
    mat q;
    for (auto uv_: uv) {
        // case: repeater
        if (uv_ == 0 && iv(uv_) != -1) {
            q = Repeater(m,i,false);
            mx.insert_rows(mx.n_rows,q);
        // case: palindromic repeater
        } else if (uv_ == 1 && iv(uv_) != -1) {
            q = Repeater(m,i,true);
            mx.insert_rows(mx.n_rows,q);
        // case: permuted element
        } else if (uv_ == 2 && iv(uv_) != -1) {
            rowvec r = m.row(v3[i]);
            mx.insert_rows(mx.n_rows,r);
        }
    }

    return mx;
}

///  
mat SnakeModificationPattern::Repeater(mat m, int i,bool pmode) {
    mat mx(0,0,fill::zeros);

    if (i >= v1.size() - 1) {
        return mx;
    }

    pair<int,int> p = make_pair(v1[i] % m.n_rows,v1[i+1] % m.n_rows); 
    if (pmode) {
        return IndiceRangeToPalindromicSubArmaMat(m,p);
    }
    return IndiceRangeToSubArmaMat(m,p);
}

///////////////////////////////////////////////

/// * special case: 
/// if calculated matrix m' from m is NULL, then set
/// m' as [m(0),m(-1)]. 
mat HawkModificationPattern::Output(mat m) {

    l2 = new LCG(int(v2(0)),int(v2(1)),int(v2(2)),m.n_rows);

    mat m2(0,0,fill::zeros);

    int j;
    int j2;
    rowvec rv;
    for (int i = 0; i < m.n_rows; i++) {
        j = l1->DefaultPRIntInRange();
        if (j == 0) {
            continue;
        }

        j2 = l2->DefaultPRIntInRange();

        if (j == 1) {
            rv = m.row(i) + float(j2);
        } else if (j == 2) {
            rv = m.row(i) - float(j2);
        } else if (j == 3) {
            rv = m.row(i) * float(j2);
        } else {
            rv = m.row(i) / float(j2);
        }
        m2.insert_rows(m2.n_rows, rv);
    }

    if (m2.n_rows == 0) {
        m2.insert_rows(m2.n_rows,m.row(0));
        m2.insert_rows(m2.n_rows,m.row(m.n_rows -1));
    }

    return m2;
}

//////////////////////////////////////////////////////////


vector<ivec> ParseStringToIVecs(string smp_args,ivec indices) {
    vec av = NumStringToArmaVec(smp_args,"_");
    assert(av.size() == indices(indices.size() - 1));
    int i = 0;

    vector<ivec> v;
    vector<int> x;

    for (auto ins: indices) {
        x.clear();

        for (int j = i; j < ins; j++) {
            x.push_back(av(j));
        }

        v.push_back(conv_to<ivec>::from(x));
        i = ins;
    }

    return v;
}

/// m1 := matrix, 2-d that is the target of modification
/// m2 := matrix, sequence of related points to those of `m1`.
/// iv := indices corresponding to the subsequences of `m2` 
///       pertinent to those elements of `m1`. 
/// smp_args := format of the string is as follows:
///              16 integers
///             [0,3): LCG #1
///             [3,6): LCG #2
///             [6,9): LCG #3
///             [9,12): LCG #4
///             [12,16): LCG #5
///             
///             *see documentation in `modif_patterns.hpp` and `modif_patterns.cpp`
///              for more information.*
mat FoxMask(mat m1, mat m2, ivec iv, string smp_args) {
    vector<ivec> vi = ParseStringToIVecs(smp_args,{3,6,9,12,16});
    FoxModificationPattern fmp = FoxModificationPattern(m1.n_rows, vi[0],vi[1], vi[2], vi[3], vi[4]);
    return fmp.Output(m1,m2,iv);
}

/// m := matrix, 2-d that is the target of modification
/// smp_args := format of the string is as follows:
///              14 integers
///             [0,5): LCG #1
///             [5,10): LCG #2
///             [10,14): LCG #3
mat SnakeMask(mat m, string smp_args) {
    vector<ivec> vi = ParseStringToIVecs(smp_args,{5,10,14});
    SnakeModificationPattern smp = SnakeModificationPattern(vi[0], vi[1], vi[2]);
    mat m2 = smp.Output(m);
    return m2;
}

/// m := matrix, 2-d that is the target of modification
/// smp_args := format of the string is as follows:
///              16 integers
///             [0,3): LCG #1
///             [4,6): LCG #2
mat HawkMask(mat m, string smp_args) {
    vector<ivec> vi = ParseStringToIVecs(smp_args,{3,6});
    HawkModificationPattern hmp = HawkModificationPattern(vi[0], vi[1]);
    return hmp.Output(m);
}
