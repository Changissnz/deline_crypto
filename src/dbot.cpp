
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
    pair<DelineMD*,AbstractPRNG*> dmd = LoadDMD();
    cout << "performing delineation" << endl;
    dmd.first->DelineateFull();

    // declare the AbstractPRNG
    dmdt = new DMDTraveller(dmd.first, dmd.second);
}

pair<DelineMD*,AbstractPRNG*> DBot::LoadDMD() {
    
    string s;
    vector<string> vs;
    while (!drdi->finished) {
        s = drdi->ReadNextLine();
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
    drd->ReadEntire();
    drd->BatchNConversion();

    AbstractPRNG* aprng = APRNGFromString(vs[3]);
    DelineMD* dmd = new DelineMD(drd->ndata,di, stoi(vs[2]), false);
    return make_pair(dmd,aprng);
}

void DBot::LoadIKey() {
    int i = 0;
    while (!dri->finished && i < cs.size()) {
        mcd[cs[i]] = LoadOneCommand(true).first;
        i += 1;
    }
}

void DBot::LoadRKey() {
    if (drr == nullptr) {
        return;
    }

    while (!drr->finished) {
        rk.push_back(LoadOneCommand(false).second);
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

        AbstractPRNG* aprng2 = APRNGFromString(utgfp.second.first);

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

pair<DInstSeq*,RInst*> DBot::LoadOneCommand(bool is_ik) {
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

    if (is_ik) {
        DInstSeq* dis = new DInstSeq(vs);
        dis->ParseIntoDInst();
        return make_pair(dis,nullptr); 
    }

    RInst* ris = new RInst(vs);
    return make_pair(nullptr,ris);
}

/// - return:
/// (encoding of character `c`, integer mapping of `c`)
pair<pair<mat,int>,pair<string,string>> DBot::OneChar(string c) {
    /// fetch the appropriate DInst
    pair<DInstSeq*,string> dism = CharToInstr(c);
    
    // calculate the output
    (dism.first)->Map(dmdt);
    
    mat m = ((dism.first)->MOsequence(dmdt)).first;
    int j = CharToInt(dism.second);
    pair<mat,int> sol = make_pair(m,j);

    pair<string,string> sol2 = make_pair(c,dism.second);
    // done
    return make_pair(sol,sol2);
}

/// c := node identifier
pair<DInstSeq*,string> DBot::CharToInstr(string c) {
    // get the token identifier
    string iidt = ((utgs->utg)->nodes[c])->iidt;
    return make_pair(mcd[iidt],iidt);
}

/// c := token identifier
///
/// return:
/// - 
int DBot::CharToInt(string c) {
    for (int i = 0; i < cs.size(); i++) {
        if (cs[i] == c) {
            return i;
        }
    }
    return -1;
}