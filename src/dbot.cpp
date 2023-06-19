
#include "dbot.hpp"

void DBot::LoadFiles() {
    LoadDMDT();
    LoadIKey();
    LoadRKey(); 
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

DInstSeq* DBot::LoadOneCommand(bool is_ik) {
    DataReader* drx = is_ik ? dri : drr; 
    
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

mat DBot::DoOneChar(char c) {
    mat m = mat(0,0,fill::zeros);
    return m;
}



// declares the DMDTraveller
/*
void DBot::Preprocess() {  
}
*/
