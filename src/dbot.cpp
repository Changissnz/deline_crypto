
#include "dbot.hpp"

void DBot::LoadFiles() {
    LoadDMDT();
    LoadIKey();
    LoadRKey();
    LoadUTGS(); 
}

/// loads a <DMDTraveller> instance from the dataset `drd`
/// first two lines of string vector are for <DelineInstr>:
///     - vector<deline_method,bool::clockwise> 
///     - vector<int>;
/// the third line is for the <DelineMD> instance 
///     - int; axis of delineation
/// the fourth line is for the <AbstractPRNG> instance.
void DBot::LoadDMDT() {

    string s;
    vector<string> vs;
    while (!dri->finished) {
        s = dri->ReadNextLine();
        if (s == "") {
            break;
        }

        vs.push_back(s); 
    }

    assert(vs.size() == 4);

    // declare the DelineInstr
    vector<pair<string,bool>> instr;
    vector<int> mhc;

    vector<string> vs1 = SplitStringToVector(vs[0]," ");

    for (auto vs1_: vs1) {

        vector<string> vs12 = SplitStringToVector(vs1_,","); 
        assert (vs12.size() == 2);

        string s = vs12[0];
        assert(possible_dschemes.find(vs12[0]) != possible_dschemes.end()); 
        bool b = !!stoi(vs12[1]);

        instr.push_back(make_pair(s,b)); 
    }

    vector<string> vs2 = SplitStringToVector(vs[1]," ");
    for (auto vs2_: vs2) {
        mhc.push_back(stoi(vs2_)); 
    }

    DelineInstr* di = new DelineInstr(instr,mhc); 

    drd->BatchNConversion();
    DelineMD* dmd = new DelineMD(drd->ndata,di, stoi(vs[2]), false);
    
    cout << "performing delineation" << endl;

    dmd->DelineateFull();

    // declare the AbstractPRNG
    AbstractPRNG* aprng = APRNGFromString(vs[3]);
    dmdt = new DMDTraveller(dmd, aprng);
}

void DBot::LoadIKey() {
    int i = 0;
    while (!dri->finished && i < cs.size()) {
        mcd[cs[i]] = LoadOneCommand(true);
        i += 1;
    }
}

void DBot::LoadRKey() {
    if (drr == nullptr) {
        return;
    }

    while (!drr->finished) {
        rk.push_back(LoadOneCommand(false));
    }
}

void DBot::LoadUTGS() {
    AbstractPRNG* aprng = APRNGFromString(utgfp.second.first);
    UTGraph* utg;
    if (utgfp.first) {
        UTGraph utg1 = UTGraph::FromFile(utgfp.second.second);
        utg = &utg1;
    } else {
        vector<string> vs = SplitStringToVector(utgfp.second.second,"_");
        assert(vs.size() == 3);

        vector<string> vs2 = SplitStringToVector(vs[0]," ");
        assert(vs2.size() == 2);
        pair<int,int> dr = make_pair(stoi(vs2[0]),stoi(vs2[1]));

        AbstractPRNG* aprng2 = APRNGFromString(utgfp.second.second);

        vector<string> vs3 = SplitStringToVector(vs[2], " ");
        assert(vs3.size() == 5);
        PermLCG* plcg = new PermLCG(stoi(vs3[0]),stoi(vs3[1]),stoi(vs3[2]),
            stoi(vs3[3]),stof(vs3[4]));

        UTGraphGen ugg = UTGraphGen(cs,dr,aprng2,plcg);
        ugg.LoadUTGraph();
        utg = ugg.utg; 
    }
        assert(CheckGraph(utg));

    utgs = new UTGSwapper(utg,aprng,false);
}

bool DBot::CheckGraph(UTGraph* utg) {
    // check uniqueness of charset
    set<string> scs(cs.begin(),cs.end());

    if (!(scs.size() == cs.size())) {
        return false; 
    }

    // iterate through the nodes of utg
    set<string> ns;
    set<string> ts;
    for (auto n_: utg->nodes) {
        ns.insert((n_.second)->c);
        ts.insert((n_.second)->iidt);
    }

    if (scs != ns) {
        return false;
    }

    if (scs != ts) {
        return false;
    }

    return true;
};

DInstSeq* DBot::LoadOneCommand(bool is_ik) {
    DataReader* drx = is_ik ? dri : drr; 
    
    // collect the information. 
    string s;
    vector<string> vs;
    while (!drx->finished) {
        s = drx->ReadNextLine();
        if (s == "") {
            break;
        }

        vs.push_back(s); 
    }

    DInstSeq* dis = new DInstSeq(vs);
    dis->ParseIntoDInst();
    return dis; 
}

/// - return:
/// (encoding of character `c`, integer mapping of `c`)
pair<mat,int> DBot::OneChar(string c) {
    mat m = mat(0,0,fill::zeros);
    return make_pair(m,0);
}
