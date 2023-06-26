#include "rdbot.hpp"
using namespace std;

void Reactor::LoadInst(vector<RInst*> ri) {

    while (ri.size() > 0) {

        if (ri[0]->c1 == "ANYTIME") {
            anytimer.push_back(ri[0]);
        } else if (ri[0]->c1 == "FOR") {
            forr.push_back(ri[0]);
        } else {
            assert(1 == 0); 
        }

        ri.erase(ri.begin() + 0);
    }
}

/// output := (input char, mapped char)
void Reactor::React(DBot* db, pair<string,string> output) {

    // react FOR character
    RInst* ri = ReactantForChar(output.first);
    if (ri != nullptr) {
        ReactOne(db,ri,output);
    }

    // react ANYTIME 
    for (auto ayt: anytimer) {
        ReactOne(db,ayt,output);
    }
}

void Reactor::ReactOne(DBot* db, RInst* ri,pair<string,string> output) {

    /// get reaction decision
    bool stat = ri->ReactDecision(db->dmdt,db->mcd[output.second],output.first); 

    /// case: reaction by MODINSTR
    if (stat && (ri->reaction_args[0] == "MODINSTR")) {
        //cout << "react on targets" << endl;
        ReactOnTargets(db,ri);
        return;
    } 

    if (stat && (ri->reaction_args[0] == "MODUTG")) {
        (db->utgs)->CommandSwap(ri->reaction_args[1]);
        return;
    }
}

void Reactor::ReactOnTargets(DBot* db, RInst* ri) {
    //cout << "reacting on: " << endl;
    //cout << "reaction targets len: " << ri->reaction_targets.size() << endl; 
    for (auto rt: ri->reaction_targets) {
        //cout << "RT: " << rt << endl;
        DInstSeq* dis = RTMODINSTR(db->dmdt, (db->mcd)[rt], ri->reaction_args[1]);
        db->mcd[rt] = dis;
    }
}

RInst* Reactor::ReactantForChar(string s) {

    for (auto f: forr) {
        if (f->c2 == s) {
            return f;
        }
    }

    return nullptr;
}

////////////////////////////////////
