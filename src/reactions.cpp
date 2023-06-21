#include "reactions.hpp"
#include "keygen.hpp"

/// TODO: test

/// **
/// possible number of modifications `x1` in [3,20]
///
/// *different types of modification*
///     PERM,MODIFY,DUP,INS,DEL
///
DInstSeq* RTMODINSTR(DMDTraveller* dt, DInstSeq* di, std::string vs) {

    // declare the APRNG
    AbstractPRNG* aprng = APRNGFromString(vs);
    
    // get number of modifications
    int num_mod = aprng->PRIntInRange(make_pair(3,20));
    for (int i = 0; i < num_mod; i++) {

        /// get modification type
        /// cases: `delete` is|(is not) an option.
        int sz = di->DInstCategory("MO").size();
        pair<int,int> pf = (sz <= 1) ? make_pair(0,3): make_pair(0,4);
        int mt = aprng->PRIntInRange(pf);

        // case: PERM
        if (mt == 0) {
            PERMUTEDINST(di,aprng);
        // case: MODIFY
        } else if (mt == 1) {
            MODIFYDINST(dt,di,aprng);
        // case: DUP
        } else if (mt == 2) {
            DUPDINST(di,aprng);
        // case: INS
        } else if (mt == 3) {
            INSDINST(dt,di,aprng);
        // case: DEL
        } else {
            DELDINST(di,aprng);
        }
    }
    
    return di;
}

/// permutes the ordering of the DInst* in DInstSeq* 
void PERMUTEDINST(DInstSeq* di, AbstractPRNG* aprng) {
    // declare a PermLCG
    vector<float> vi = aprng->CycleOne(false,4);

    // permute the `one_apply` 
    PermLCG* plcg = new PermLCG(vi[0],vi[1],vi[2],vi[3]);
    int sz1 = di->one_apply.size(); 
    plcg->SetRangeData(make_pair(int(0),sz1 - 1));
    vector<float> vi1 = plcg->CycleOne(false,sz1);
    vector<DInst*> vd1;
    for (auto vi1_: vi1) {
        vd1.push_back(di->one_apply[int(vi1_)]);
    }
    di->one_apply = vd1;

    // permute the `all_apply`
    PermLCG* plcg2 = new PermLCG(vi[0],vi[1],vi[2],vi[3]);
    int sz2 = di->all_apply.size(); 
    plcg2->SetRangeData(make_pair(0,sz2 - 1));
    vector<float> vi2 = plcg2->CycleOne(false,sz2);
    vector<DInst*> vd2;
    for (auto vi2_: vi2) {
        vd2.push_back(di->all_apply[int(vi2_)]);
    }
    di->all_apply = vd2;
}

/// applies the MODIFY scheme to the contents of one DInst*
/// 
/// **selecting the DInst* from DInstSeq* `di`** 
/// - `n` in [0,1]; 0 is one_apply and 1 is all_apply
/// - `n2` in [0,x -1]; x is the length of (one_apply|all_apply)
///
/// *output limits of MODIFY for the different moves (MO,BG,MSK,HOP)*
/// 
/// <MO>: 
///         [1]: `n` in [1,25]; number of moves.
///         [2]: +`index1`%dsize1
///         [3]: +`index2`%dsize2
///         [4]: `x` number of elements,
///              ((direction::constant + increment)%4 | (increment)) * x
///             ** use the first option if given direction sequence is long enough,
///                otherwise use increment.  
///         [5]: ((scale::constant + increment)%8. | (increment)) * x; see [4] for x. 
///             ** choosing the first or second option is the same as that for [4].
///                The maximum scale value is 2. (the modulo), 
/// <MSK>:
///         [1]: `n` in [0,2]; `n` is added to the mask of FOX,SNAKE,HAWK and
///              modulated.
///         [2]: `x` * (14|16|6), depending on the mask.
///
/// <BG>:
///         [1]: `n` in [0,1]; `n` is added to [1] and then modulated.
///         [2]: `n` in [1,25]
///         [3]: +`index1`%dsize1
///         [4]: +`index2`%dsize2
///         [5]: (+`n2` in [0,128]) % 128
///
/// <HOP>:
///         [1]: +`index1`%dsize1
///         [2]: +`index2`%dsize2
///         [3]: aprng string (modified w/ )
/// NOTE: the values enclosed in `` are the output values from an APRNG
/// NOTE: modifying a DInst* does NOT change its type (var<ins>).
void MODIFYDINST(DMDTraveller* dt,DInstSeq* di, AbstractPRNG* aprng) {

    // get a DInst*
    DInst* dx = SELECTDINST(di,aprng);
    MODIFYDINST_(dt,dx,aprng);
}

/// *see the comment for `MODIFYDINST` for information on the aprng limits
///  for each var<ins>*
void MODIFYDINST_(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng) {
    
    if (dx->ins == "MO") {
        MODIFY_TYPEMO(dt,dx,aprng);
    } else if (dx->ins == "MSK") {
        MODIFY_TYPEMSK(dt,dx,aprng);
    } else if (dx->ins == "BG") {
        MODIFY_TYPEBG(dt,dx,aprng);
    } else if (dx->ins == "HOP") {
        MODIFY_TYPEHOP(dt,dx,aprng);
    }
}

/// *see the comment for `MODIFYDINST` for information on the aprng limits
///  for each var<ins>*
void MODIFY_TYPEMO(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng) {
    // get the info for `dt`, used for modulation.
    pair<int,map<int,int>> pim = dt->Info();

    // modify the number of moves
    int i1 = aprng->PRIntInRange(make_pair(1,25));
    dx->inst_args[0] = to_string((i1 + stoi(dx->inst_args[0])) % 25);
    
    // modify the index
    int i2 = aprng->PRIntInRange(make_pair(1,pim.first));
    i2 = (i2 + stoi(dx->inst_args[1])) % pim.first;
    dx->inst_args[1] = to_string(i2);

    // modify the next index
    int i3 = aprng->PRIntInRange(make_pair(1,pim.second[i2]));
    i3 = (i3 + stoi(dx->inst_args[2])) % pim.second[i2];
    dx->inst_args[2] = to_string(i3);

    // get the `x` number of elements for directional
    int i4 = aprng->PRIntInRange(make_pair(2,20));

    // make the directional vector and corresponding scale vector
    vector<string> dv = SplitStringToVector(dx->inst_args[3],".");
    vector<string> dv2 = SplitStringToVector(dx->inst_args[4],"_");
    string dvs = "";
    string svs = "";
    int i5;
    float i6;
    char c; 
    for (int i = 0; i < i4; i++) {
        i5 = aprng->PRIntInRange(make_pair(0,3));
        i6 = aprng->PRFloatInRange(make_pair(0.01,2.),5);
        // case: dv is long enough
        if (dv.size() > i) {
            i5 += NavDirToInt(*(dv[i].c_str()));
            i6 += stof(dv2[i]);
        }
        i5 = i5 % 4;
        i6 = fmod(i6,2.);
        c = IntToNavDir(i5);
        dvs += c + ".";
        svs += to_string(i6) + "_";
    }
    dvs = dvs.substr(0,dvs.size() - 1);
    svs = svs.substr(0,svs.size() - 1);
    dx->inst_args[3] = dvs;
    dx->inst_args[4] = svs;

    int i7 = aprng->PRIntInRange(make_pair(0,1));
    dx->inst_args[5] = i7;

    /// TODO: record `dx` args into APRNG
}

/// *see the comment for `MODIFYDINST` for information on the aprng limits
///  for each var<ins>*
void MODIFY_TYPEMSK(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng) {
    // modify the MSK
    int i1 = aprng->PRIntInRange(make_pair(0,2));
    int i2 = MSKToInt(dx->inst_args[0]);
    dx->inst_args[0] = IntToMSK((i1 + i2) % 3);

    // modify the arguments of MSK
    int nx;
    if (dx->inst_args[0] == "FOX") {
        nx = 16;
    } else if (dx->inst_args[0] == "SNAKE") {
        nx = 14;
    } else {
        nx = 6;
    }

    string sx = "";
    int i3;
    for (int i = 0; i < nx; i++) {
        i3 = aprng->PRIntInRange(make_pair(MIN_APRNG_VAR,MAX_APRNG_VAR));
        sx += to_string(i3) + "_"; 
    }
    sx = sx.substr(0,sx.size() - 1);
    dx->inst_args[1] = sx;
}

/// *see the comment for `MODIFYDINST` for information on the aprng limits
///  for each var<ins>*
void MODIFY_TYPEBG(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng) {
    // modify BG type
    int i1 = aprng->PRIntInRange(make_pair(0,1));
    dx->inst_args[0] = IntToBG((i1 + BGToInt(dx->inst_args[0])) % 2);

    // modify the number of moves
    int i2 = aprng->PRIntInRange(make_pair(1,25));
    dx->inst_args[1] = to_string((i2 + stoi(dx->inst_args[1])) % 25);

    // modify the indices
    pair<int,map<int,int>> pim = dt->Info();
    
        // modify the index
    int i3 = aprng->PRIntInRange(make_pair(1,pim.first));
    i3 = (i3 + stoi(dx->inst_args[2])) % pim.first;
    dx->inst_args[2] = to_string(i3);

        // modify the next index
    int i4 = aprng->PRIntInRange(make_pair(1,pim.second[i3]));
    i4 = (i4 + stoi(dx->inst_args[3])) % pim.second[i3];
    dx->inst_args[3] = to_string(i4);

    // modify the magnitudes
    vector<string> vs = SplitStringToVector(dx->inst_args[4],"_");
    float i5 = aprng->PRFloatInRange(make_pair(0.,128.),5);
    float i6 = aprng->PRFloatInRange(make_pair(0.,128.),5);
    i5 = RoundDecimalNPlaces(fmod(i5 + stof(vs[0]),128.),5);
    i6 = RoundDecimalNPlaces(fmod(i6 + stof(vs[1]),128.),5);
    string sx = to_string(i5) + "_" + to_string(i6);
    dx->inst_args[4] = sx;
}

/// *see the comment for `MODIFYDINST` for information on the aprng limits
///  for each var<ins>*
void MODIFY_TYPEHOP(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng) {

    // modify the indices
    pair<int,map<int,int>> pim = dt->Info();
    
        // modify the index
    int i1 = aprng->PRIntInRange(make_pair(1,pim.first));
    i1 = (i1 + stoi(dx->inst_args[0])) % pim.first;
    dx->inst_args[0] = to_string(i1);

        // modify the next index
    int i2 = aprng->PRIntInRange(make_pair(1,pim.second[i1]));
    i2 = (i2 + stoi(dx->inst_args[1])) % pim.second[i1];
    dx->inst_args[1] = to_string(i2);
    
    // modify the aprng string
    dx->inst_args[2] = APRNGStringArgsDeltaScheme1(dx->inst_args[2],aprng);
}

// duplicates one DInst* of `di` for n times, n in [1,4].
void DUPDINST(DInstSeq* di, AbstractPRNG* aprng) {
    DInst* dx = SELECTDINST(di,aprng);
    DInst* dxs;
    int num_dups = aprng->PRIntInRange(make_pair(1,4));
    for (int i = 0; i < num_dups;i++) {
        dxs = dx->Copy();
        DINST_APRNGINSERT(di,dxs,aprng);
    }
}

/// uses KeyGen to generate a new DInst and inserts it into
/// `di`. Below are the probability ranges for each move:
///     - MSK: 0-15
///     - MO: 15-55
///     - BG: 55-80
///     - HOP: 80-100
void INSDINST(DMDTraveller* dt, DInstSeq* di, AbstractPRNG* aprng) {
    
    // declare a new KeyGen
    pair<int,map<int,int>> info = dt->Info();
    KeyGen* kg = new KeyGen(aprng,nullptr,"","",info);
    int i1 = aprng->PRFloatInRange(make_pair(0.,100.),5);

    // generate a string command w/ the KeyGen
    string sx;
    if (i1 < 15.) {
        sx = kg->OutMSK();
    } else if (i1 >= 15. && i1 < 55.) {
        sx = kg->OutMO();
    } else if (i1 >= 55. && i1 < 80.) {
        sx = kg->OutBG();
    } else {
        sx = kg->OutHOP();
    }
    
    // declare the DInst
    vector<string> vs = SplitStringToVector(sx,",");
    string s1 = vs[0];
    vs.erase(vs.begin() + 0);
    DInst* dx = new DInst(s1,vs);

    // insert the DInst at an index outputted by aprng
    DINST_APRNGINSERT(di,dx,aprng);   

    /// CAUTION:
    free(kg);
    assert(aprng != nullptr); 
}

/// deletes one selected DInst by aprng. 
void DELDINST(DInstSeq* di, AbstractPRNG* aprng) {
    int n1 = aprng->PRIntInRange(make_pair(0,1));
    if (n1 == 0) {
        int n2 = aprng->PRIntInRange(make_pair(0,di->one_apply.size() - 1));
        di->one_apply.erase(di->one_apply.begin() + n2);
    } else {
        int n2 = aprng->PRIntInRange(make_pair(0,di->all_apply.size() - 1));
        di->all_apply.erase(di->all_apply.begin() + n2);
    }
}

/// inserts `dx` into `di` by `aprng`'s output as the indices. 
void DINST_APRNGINSERT(DInstSeq* di, DInst* dx, AbstractPRNG* aprng) {

    // determine if `dx` is one_apply or all_apply
    if (oneapp.find(dx->ins) != oneapp.end()) {
        int i2 = aprng->PRIntInRange(make_pair(0,di->one_apply.size()));
        di->one_apply.insert(di->one_apply.begin() + i2, dx); 
    } else {
        int i2 = aprng->PRIntInRange(make_pair(0,di->all_apply.size()));
        di->all_apply.insert(di->all_apply.begin() + i2, dx);
    }
}

// fetches one DInst* by aprng output
DInst* SELECTDINST(DInstSeq* di, AbstractPRNG* aprng) {
    int n1 = aprng->PRIntInRange(make_pair(0,1));
    DInst* dx;
    if (n1 == 0) {
        int n2 = aprng->PRIntInRange(make_pair(0,di->one_apply.size() - 1));
        dx = di->one_apply[n2];
    } else {
        int n2 = aprng->PRIntInRange(make_pair(0,di->all_apply.size() - 1));
        dx = di->all_apply[n2];
    }
    return dx;
}

void RTMODUTG(UTGSwapper* utgs,string command) {
    utgs->CommandSwap(command);
}