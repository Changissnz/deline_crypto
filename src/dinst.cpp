#include "dinst.hpp"
using namespace std;

void DInst::PrecheckArgs() {

    if (ins == "MSK") {
        assert (inst_args.size() == 2); 
        return;
    } else if (ins == "MO") {
        assert (inst_args.size() ==  6);
        return; 
    } else if (ins == "BG") {
        assert (inst_args.size() ==  5);
        return; 
    } else if (ins == "HOP") {
        assert (inst_args.size() ==  3);
        return;
    }

    assert (0==1); 
}

/// *format of the output string*
///     MSK,mask type,i1_i2_..._iN
///
///  * there are three mask types available, FOX SNAKE HAWK
///    if FOX, then N = 16
///    if SNAKE, then N = 14
///    if HAWK, then N = 6
mat DInst::MapMSK(DMDTraveller* dt,mat m1,mat m2,ivec m2i) {

    // output the mask by category:
    
    // - fox, snake, hawk
    if (inst_args[0] == "FOX") {
        return FoxMask(m1,m2,m2i,inst_args[1]);
    } else if (inst_args[0] == "SNAKE") {
        return SnakeMask(m1,inst_args[1]);
    } else if (inst_args[0] == "HAWK") {
        return HawkMask(m1,inst_args[1]);
    } else {
        assert(false);
    }
    
    mat m(0,0,fill::zeros);
    return m;
}

///     *form 1*
///
/// BG,ALTER,number of iterations,index of DelineMC* of DelineMD*, index of Delineate22* of DelineMC*,
///     float1ABSMIN_float2ABSMAX
///
/// *example*
/// BG,ALTER,5,2,4,-0.75_-5.0
///
///     *form 2*
/// BG,JITTER,number of iterations,index of DelineMC* of DelineMD*, index of Delineate22* of DelineMC*,
///    XMAGofFORCE_YMAGofFORCE
///
/// *example*
/// BG,JITTER,5,2,4,100.245_-29.0234
void DInst::MapBG(DMDTraveller* dt) {
    string bgtype = inst_args[0];
    int nm = stoi(inst_args[1]); 
    int dmci = stoi(inst_args[2]);
    int d22i = stoi(inst_args[3]);
    Deline22* d22 = dt->SelectD22(dmci,d22i);

    vector<string> vs = SplitStringToVector(inst_args[4],"_");
    assert(vs.size() == 2);
    float r1 = stof(vs[0]);
    float r2 = stof(vs[1]);

    if (bgtype == "ALTER") {
        assert(abs(r1) <= abs(r2));
        assert((r1 >= 0. && r2 >= 0.) || (r1 <= 0. && r2 <= 0.));
        bool expand = (r1 >= 0.) ? true: false;
        r1 = abs(r1);
        r2 = abs(r2);

        for (int i = 0; i < nm;i++) {
            dt->AlterDeline22(d22,make_pair(r1,r2),expand);
        }
        return;
    }

    if (bgtype == "JITTER") {
        for (int i = 0; i < nm;i++) {
            dt->JitterDeline22(d22, {r1,r2},JPMomentum);
        }
        return;
    }

    assert(false);
}

///     *format of MO command
/// ins := MO
/// inst_args := number_of_iterations::(int),index_of_Deline22MC::(int),
///         index_of_Deline22::(int),dir0.dir1...dirN,float1_float2_...floatN,(assign point to closest data point)::bool
///     
///     * dirx is a value in {'l','r','t','b'}.
///     * floatx is its corresponding scale.
///
/// -- example
/// MO,3,2,3,L.R.T.B.B.R,0.5_1_0.75_0.5_1,1
void DInst::MapMO(DMDTraveller* dt) {
    /*
    // clear all travel data first
    ///dt->ClearTravelData();
    */

    int nm = stoi(inst_args[0]); 
    int dmci = stoi(inst_args[1]);
    int d22i = stoi(inst_args[2]);

    vector<string> vs_ = SplitStringToVector(inst_args[3], ".");
    vector<char> vs;
    for (auto vs1: vs_) {
        auto cs = vs1.c_str();
        vs.push_back(*cs);
    }

    vec av = NumStringToArmaVec(inst_args[4],"_");
    bool b = stoi(inst_args[5]);

    Deline22* d22 = dt->SelectD22(dmci,d22i); 

    for (int i = 0; i < nm; i++) {
        dt->MoveOne(d22, d22->tindex, vs,av,b);
    }
}

/// ins := HOP 
/// inst_args := index_of_Deline22MC::(int),index_of_Deline22::(int),APRNGstring
/// 
/// -- example
/// HOP,2,4,APRNGstring
void DInst::MapHOP(DMDTraveller* dt) {
    int dmci = stoi(inst_args[0]);
    int d22i = stoi(inst_args[1]);
    AbstractPRNG* aprng = APRNGFromString(inst_args[2]);

    Deline22* d22 = dt->SelectD22(dmci,d22i);
    int jx = (d22->del)->data.n_rows;
    int qx = d22->tindex + aprng->PRIntInRange(make_pair(0,jx));
    d22->tindex = (d22->tindex) % jx;
}

DInst* DInst::Copy() {
    string i1 = ins;
    vector<string> i2;
    for (auto vs: inst_args) {
        i2.push_back(vs);
    }

    return new DInst(i1,i2);
}

////////////////////////////////////////////////

void DInstSeq::ParseIntoDInst() {

    vector<string> vs;
    string sg;
    DInst* dix;
    bool stat = false;
    for (auto s_: s) {
        vs = SplitStringToVector(s_,",");
        sg = vs[0];
        vs.erase(vs.begin() + 0);
        dix = new DInst(sg,vs);

        if (sg == "MSK") {
            all_apply.push_back(dix);
        } else {
            one_apply.push_back(dix);
        }

        if (sg == "MO") {
            stat = true;
        }
    }

    //free(dix);
    assert (stat);
    s.clear();
}

/// Uses the commands provided to conduct operations on `dt`.
/// Saves the sequence generated. 
void DInstSeq::Map(DMDTraveller* dt) {
    output.clear();// = (0,0,fill::zeros);

    int i = 0;
    // run all the one_apply
    for (auto oa: one_apply) {
        if (oa->ins == "MO") {
            oa->MapMO(dt);
        } else if (oa->ins == "BG") {
            oa->MapBG(dt);
        } else if (oa->ins == "HOP") {
            oa->MapHOP(dt);
        } else {
            assert(false);
        }
        i += 1;
    }
    
    /// case: no mask, encoding completed for instruction
    // retrieve the last sequence created by the MO's. 
    if (DInstCategory("MSK").size() == 0) {
        pair<mat,pair<mat,ivec>> pmp = MOsequence(dt);
        output.insert_rows(output.n_rows,pmp.first); 
    }

    // run the only all apply category: MSK
    for (auto oa: all_apply) {
        if (oa->ins == "MSK") {
            pair<mat,pair<mat,ivec>> pmp = MOsequence(dt);
            mat mx = oa->MapMSK(dt,pmp.first,pmp.second.first,pmp.second.second);
            output.insert_rows(output.n_rows,mx); 
        } else {
            assert(false);
        }
    }
}

int DInstSeq::MOPointSize(DMDTraveller* dt) {
    vector<DInst*> vdi = DInstCategory("MO");
    int ix = 0;
    for (auto dx: vdi) {
        ix += stoi(dx->inst_args[0]);
    }
    return ix;
}

/// gets the matrix of points corresponding to all
/// the MO commands represented by this class instance. 
/// CAUTION: assumes DInstSeq* already called `Map`.
///
/// - return:
/// [0] := travel points 
/// [1] := travel points considered, indices of travel points considered.
pair<mat,pair<mat,ivec>> DInstSeq::MOsequence(DMDTraveller* dt) {
    
    // get the number of points 
    int ix = MOPointSize(dt);
    assert(dt->travel_points.n_rows >= ix);

    // get the travel points
    int l = dt->travel_points.n_rows;
    int s = dt->travel_points.n_rows - ix;
    vector<int> vi;

    for (int j = s; j < l;j++) {
        vi.push_back(j);
    }

    mat mx = IndicesToSubArmaMat(dt->travel_points, conv_to<ivec>::from(vi));

    mat mx2(0,0,fill::zeros); 
    // case: FOX 
        // get those tpc points
    for (int j = ix; j > 0; j--) {
        mx2.insert_rows(mx2.n_rows,dt->LastNthTPCSequence(j));
    }

    // get the tpc indices. 
    int minu = dt->tpc_indices.size() - ix -1;
    int maxu = dt->tpc_indices.size();
    vi.clear();
    for (int j = minu;j < maxu; j++) {
        vi.push_back(j);
    }

    // calibrate the tpc indices to be at 0.
    ivec vi2 = conv_to<ivec>::from(vi);
    vi2 = vi2 - vi2(0);
    vi.clear();
    for (int i = 1; i < vi2.size();i++) {
        vi.push_back(vi2[i]);
    }
    
    mx.shed_col(2);
    return make_pair(mx,make_pair(mx2,conv_to<ivec>::from(vi)));
}

vector<DInst*> DInstSeq::DInstCategory(string category) {

    vector<DInst*> vdi; 
    
    if (oneapp.find(category) != oneapp.end()) {

        for (auto x: one_apply) {
            if (x->ins == category) {
                vdi.push_back(x); 
            }
        }

        return vdi;
    }

    if (allapp.find(category) != allapp.end()) {

        for (auto x: all_apply) {
            if (x->ins == category) {
                vdi.push_back(x); 
            }
        }

        return vdi;
    }
    
    assert(false);
    return vdi;
}


int MSKToInt(string msk) {

    if (msk == "FOX") {
        return 0;
    } else if (msk == "SNAKE") {
        return 1;
    } else if (msk == "HAWK") {
        return 2;
    }

    assert(false);
    return 3;
}

string IntToMSK(int i) {

    if (i == 0) {
        return "FOX";
    } else if (i == 1) {
        return "SNAKE";
    } else if (i == 2) {
        return "HAWK";
    }
    
    assert(false);
    return "MASKARA";
}


int BGToInt(string bg) {

    if (bg == "ALTER") {
        return 0;
    } else if (bg == "JITTER") {
        return 1;
    }

    assert(false);
    return 2;
}

string IntToBG(int i) {

    if (i == 0) {
        return "ALTER";
    } else if (i == 1) {
        return "JITTER";
    }

    assert(false);
    return "BIGNUFFIN";
}