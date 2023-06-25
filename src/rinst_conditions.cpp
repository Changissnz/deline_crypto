#include "rinst_conditions.hpp"
using namespace std; 

/// NOTE: code in this file should be refactored if more
///       code is to be added. 

/// *format of the descriptor*
///     f0_f1
/// in which f0 <= f1 and f0,f1 in [0.,1.]. The range [f0,f1] specifies the qualifying
/// ratio that the points,in the traveller's previous sequence, must be in the delineation
/// as specified by `di`.
bool C1__IN_BOUNDS_OF_DELINEATION(DMDTraveller* dmdt,DInstSeq* di, string desc) {
    ivec iv = C1__IN_BOUNDS_OF_DELINEATION_(dmdt,di);
    assert(iv.size() > 0);
    double dx = accu(iv) / iv.size();

    vector<string> vs = SplitStringToVector(desc,"_");
    assert(vs.size() == 2);
    float f1 = stof(vs[0]);
    float f2 = stof(vs[1]);
    return ((dx >= f1) && (dx <= f2));
}

ivec C1__IN_BOUNDS_OF_DELINEATION_(DMDTraveller* dmdt,DInstSeq* di) {

    // get the MO category
    vector<DInst*> vdi = di->DInstCategory("MO");

    // get the last sequence
    mat pseq = (di->MOsequence(dmdt)).first;
    int i = 0;
    int j = 0;
    int k = 0;

    vector<int> vi;

    // iterate through each point and classify it as 1 (in delineation) or 0
        // get the first delineation
    Deline22* d22;
    int c;
    while (i < pseq.n_rows) {

        // get the delineation
        int dmci = stoi(vdi[j]->inst_args[1]);
        int d22i = stoi(vdi[j]->inst_args[2]);

        // classify the point
        d22 = dmdt->SelectD22(dmci,d22i);
        c = d22->ClassifyPoint(pseq.row(i));

        if (c != -1) {
            vi.push_back(1);
        } else {
            vi.push_back(0);
        }

        i += 1;
        k += 1;
        int l = stoi(vdi[j]->inst_args[0]);

        if (k == l) {
            j += 1;
            k = 0;
        }
    }

    return conv_to<ivec>::from(vi); 
}

/// *format of the descriptor*
///     (+|-|/|*)*n,modulo,modulo range
/// 
///   in which the sequence of arithmetic operators S is to be cycled
///   if the previous sequence P of `dmdt` is shorter than it, otherwise
///   iteration of S terminates at the length of P. 
/// 
/// * example *
///     +-/*+-++, 4,2_3
bool C2__ARITHMETICSEQ_IN_MOD_RANGE(DMDTraveller* dmdt, DInstSeq* di,string desc) {
    vec vx = C2__ARITHMETICSEQ_IN_MOD_RANGE_(dmdt,di,desc);
    vector<string> vs = SplitStringToVector(desc,",");
    assert(vs.size() == 3);
    vector<string> vs2 = SplitStringToVector(vs[2],"_");
    assert(vs2.size() == 2);
    float f1 = stof(vs2[0]);
    float f2 = stof(vs2[1]);

    bool stat1 = (vx(0) >= f1) && (vx(0) <= f2);
    bool stat2 = (vx(1) >= f1) && (vx(1) <= f2);
    return stat1 && stat2;
}

vec C2__ARITHMETICSEQ_IN_MOD_RANGE_(DMDTraveller* dmdt, DInstSeq* di,string desc) {

    vector<string> vs = SplitStringToVector(desc,",");
    string aop = vs[0];
    int i = 0;

    // get the last sequence
    mat pseq = (di->MOsequence(dmdt)).first;
    rowvec rx = pseq.row(0);

    for (int j = 1; j < pseq.n_rows; j++) {
        string aop_ = aop.substr(i,1);
        if (aop_ == "+") {
            rx = rx + pseq.row(j);
        } else if (aop_ == "-") {
            rx = rx - pseq.row(j);
        } else if (aop_ == "*") {
            rowvec rx2 = pseq.row(j);
            rx = Str8MultOnVecs(rx,rx2);
        } else {
            rowvec rx2 = pseq.row(j);
            rx = Str8DivOnVecs(rx,rx2);
        }

        i = (i + 1) % aop.size();
    }

    // perform the mod
    float f = stof(vs[1]);
    rx = {fmod(rx(0),f),fmod(rx(0),f)}; 
    return conv_to<vec>::from(rx);
} 

/// *format of the descriptor*
///     f0_f1
/// in which f0 <= f1 and f0,f1 in [0.,1.]. The range [f0,f1] specifies the qualifying
/// ratio that the points,in the traveller's previous sequence, must be in the span 
/// of the 2nd to last sequence taken by the traveller. 
bool C3__POINTSEQ_INTERSECTS_W_PREVIOUS_SPAN(DMDTraveller* dmdt,DInstSeq* di,string desc) {
    ivec vi = C3__POINTSEQ_INTERSECTS_W_PREVIOUS_SPAN_(dmdt,di);

    vector<string> vs = SplitStringToVector(desc,"_");
    assert(vs.size() == 2);
    float f1 = stof(vs[0]);
    float f2 = stof(vs[1]);
    double dx = accu(vi) / vi.size();

    return (dx >= f1) && (dx <= f2);
}

ivec C3__POINTSEQ_INTERSECTS_W_PREVIOUS_SPAN_(DMDTraveller* dmdt,DInstSeq* di) {
    mat pseq = (di->MOsequence(dmdt)).first;

    // get the previous point-sequence of equal length to pseq
    mat m2 = PreviousSequence(dmdt,pseq.n_rows);

    double min1 = m2.col(0).min();
    double max1 = m2.col(0).max();
    double min2 = m2.col(1).min();
    double max2 = m2.col(1).max();

    vector<int> vi;
    for (int i = 0; i < pseq.n_rows; i++) {
        bool stat1 = (pseq(i,0) >= min1) && (pseq(i,0) <= max1);
        bool stat2 = (pseq(i,1) >= min2) && (pseq(i,1) <= max2);
        vi.push_back(int(stat1 && stat2));
    }

    return conv_to<ivec>::from(vi); 
}


/// *format of the descriptor*
///     m_f0_f1
/// in which m is the classification scheme set to hierarchical if True, otherwise base.
/// And f0 <= f1 and f0,f1 in [0.,1.], and serve as the activation range. 
/// The function uses the ratio that the points,in the traveller's previous sequence, must be in the span 
/// of the 2nd to last sequence taken by the traveller. 
bool C4__POINTSEQ_MULTICLASS_MEASURE(DMDTraveller* dmdt, DInstSeq* di, string desc) {
    vector<string> vs = SplitStringToVector(desc,"_");
    ivec vi = C4__POINTSEQ_MULTICLASS_MEASURE_(dmdt,di,bool(stoi(vs[0])));
    assert(vi.size() > 0);
    double dx = accu(vi) / vi.size();

    float f1 = stof(vs[1]);
    float f2 = stof(vs[2]);
    assert (f1 <= f2);
    return (dx >= f1) && (dx <= f2);
}

ivec C4__POINTSEQ_MULTICLASS_MEASURE_(DMDTraveller* dmdt, DInstSeq* di,bool mode_h) {

    vector<DInst*> vdi = di->DInstCategory("MO");
    mat pseq = (di->MOsequence(dmdt)).first;

    int i = 0;
    int j = 0;
    int k = 0;
    vector<int> vi;

    // iterate through each point and classify it as 1 (in delineation) or 0
        // get the first delineation
    Deline22MC* dmc; 
    while (i < pseq.n_rows) {

        // get the delineation
        int dmci = stoi(vdi[j]->inst_args[1]);

        // classify the point
        dmc = dmdt->SelectDMC(dmci);

        int sz = 0;
        if (mode_h) {
            sz = dmc->HierarchicalClassification(pseq.row(i)).size();
        } else {
            sz = dmc->BaseClassification(pseq.row(i)).size();
        }

        if (sz > 1) {
            vi.push_back(1);
        } else {
            vi.push_back(0);
        }

        i += 1;
        k += 1;
        int l = stoi(vdi[j]->inst_args[0]);

        if (k == l) {
            j += 1;
            k = 0;
        }
    }

    return conv_to<ivec>::from(vi);
}

/// *format of the descriptor*
///     f0_f1
/// in which f0 is 1 for greater than or 0 for less than,
/// and f1 is the threshold ratio for the mean sequential
/// Euclidean distance of the last sequence divided by the
/// second-to-last sequence.
bool C5__SEQUENTIAL_EUCLIDEAN_DISTANCE_MEASURE(DMDTraveller* dmdt, DInstSeq* di, string desc) {
    vector<string> vs = SplitStringToVector(desc,"_");
    assert(vs.size() == 2);
    bool f1 = stoi(vs[0]);
    float f2 = stof(vs[1]);
    float fx = C5__SEQUENTIAL_EUCLIDEAN_DISTANCE_MEASURE_(dmdt,di);

    if (f1) {
        return (fx >= f2);
    }
    return (fx <= f2);
}

float C5__SEQUENTIAL_EUCLIDEAN_DISTANCE_MEASURE_(DMDTraveller* dmdt, DInstSeq* di) {

    mat pseq = (di->MOsequence(dmdt)).first;
    mat pseq2 = PreviousSequence(dmdt,pseq.n_rows);
    
    float f1 = MeanSequentialEuclideanDistance(pseq);
    float f2 = MeanSequentialEuclideanDistance(pseq2);
    float f = (f2 == 0.) ? 0.: f1 / f2;
    return f;
}

mat PreviousSequence(DMDTraveller* dmdt,int l) {
    int e = (dmdt->travel_points).n_rows - 1 - l;
    
    // case: no previous points
    if (e == -1) {
        mat m = {{0,0}};
        return m; 
    }

    int s = e - l;
    if (s < 0) {
        s = 0;
    }

    return IndiceRangeToSubArmaMat(dmdt->travel_points,make_pair(s,e));
}

/// *format of the descriptor*
///     d_b_f0
///
/// d is a direction of l,r,t,or b 
/// b is a integer boolean
/// f0 is the float threshold, min if b is 1 otherwise max.
bool C6__BOOLEAN_DELTA_MEASURE_ALONG_AXIS(DMDTraveller* dmdt,DInstSeq* di,string desc) {

    vector<string> vs = SplitStringToVector(desc,"_");

    assert(vs.size() == 3);
    char d = *(vs[0].c_str());
    bool f1 = stoi(vs[1]);
    float f2 = stof(vs[2]);
    cout << "getting boolean delta" << endl;
    float vi = C6__BOOLEAN_DELTA_MEASURE_ALONG_AXIS(dmdt,di,d);
    cout << "GOT IT" <<  endl;
    if (f1) {
        return (vi >= f2);
    }

    return (vi <= f2);
}

float C6__BOOLEAN_DELTA_MEASURE_ALONG_AXIS(DMDTraveller* dmdt,DInstSeq* di,char direction) {
    mat pseq = (di->MOsequence(dmdt)).first;
    if (pseq.n_rows == 0) {
        return 0.; 
    }

    colvec cx(pseq.n_rows,fill::zeros); 
    pseq.insert_cols(2,cx);
    return float(JaggedEdges(pseq,direction).size()) / float(pseq.n_rows); 
}

bool RCONDITIONAL(DMDTraveller* dmdt, DInstSeq* di,string conditional_id, string desc) {
    //cout << "conditional id: " << conditional_id << endl;
    //cout << "desc: " << desc << endl;
    assert(possible_conds.find(conditional_id) != possible_conds.end());

    if (conditional_id == "C1") {
        return C1__IN_BOUNDS_OF_DELINEATION(dmdt,di,desc);
    } else if (conditional_id == "C2") {
        return C2__ARITHMETICSEQ_IN_MOD_RANGE(dmdt,di,desc);
    } else if (conditional_id == "C3") {
        return C3__POINTSEQ_INTERSECTS_W_PREVIOUS_SPAN(dmdt,di,desc);
    } else if (conditional_id == "C4") {
        return C4__POINTSEQ_MULTICLASS_MEASURE(dmdt,di,desc);
    } else if (conditional_id == "C5") {
        return C5__SEQUENTIAL_EUCLIDEAN_DISTANCE_MEASURE(dmdt,di,desc);
    } else {
        return C6__BOOLEAN_DELTA_MEASURE_ALONG_AXIS(dmdt,di,desc);
    }

}
