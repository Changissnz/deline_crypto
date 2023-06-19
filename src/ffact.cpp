#include "ffact.hpp"
using namespace std;

/// assigns each node in `centers` to a component in `ctsq` 
/// based on the distance values of `rgi`. Tie-breaking is
/// done by `aprng`.
vector<set<string>> AssignCenterToComponentSeq(GInfo* rgi, AbstractPRNG* aprng,vector<set<string>> ctsq,set<string> cn) {
    assert (ctsq.size() > 0);
    
    // collect the distances from each node to the set of components
    // <index of component, distance>
    vector<pair<int,float>> vp;

    for (auto ct: cn) {
        vp.clear();
        for (int i = 0; i < ctsq.size(); i++) {
            float f = rgi->AverageDistanceToNodeset(ctsq[i],ct);
            vp.push_back(make_pair(i,f));
        }

        int i2 = TieBreakerMinORMax(vp,aprng,true);
        ctsq[i2].insert(ct);
    }

    return ctsq;
}

string UGModule::ToString() {

    string s = "components are:\n";

    for (auto fm_:fm) {
        s += "* ";
        for (auto fm2_: fm_) {
            s += fm2_ + " ";
        }
        s += "\n";
    }

    /// TODO: delete this
    for (auto fm_:m) {
        s += "* ";
        for (auto fm2_: fm_) {
            s += fm2_ + " ";
        }
        s += "\n";
    }

    return s;


}


void UGModule::Preprocess() {
    GSearch gs = GSearch(utg);
    gi = gs.RunGInfo();

    set<string> ss;
    for (auto cf:utg->nodes) {
        ss.insert(cf.first);
    }

    m.push_back(ss);
}

int UGModule::TotalComponentSize() {
    return fm.size() + m.size();
}

void UGModule::Split() { //AbstractPRNG* aprng ) {
    bool stat = (TotalComponentSize() < reqm) && !finstat;
    int sz = 0; 
    while (stat) {
        DefaultSplitComponent();
        stat = (TotalComponentSize() < reqm) && !finstat;

        // case: constant size, terminate or infinite loop
        if (TotalComponentSize() == sz) {
            stat = !stat;
        }
        sz = TotalComponentSize();        
    }


    copy(m.begin(),m.end(),back_inserter(fm));
    m.clear();
}


/// # TODO:
// splits the module w/ the greatest vertex size.
void UGModule::DefaultSplitComponent(){ //AbstractPRNG* aprng ) {
    if (m.size() == 0) {
        finstat = true;
        return;
    }

    int igc = ChooseGreatestComponent();
    vector<set<string>> scv = SplitComponent(igc);
    
    /*
    cout << "POST-SPLIT SIZE " << scv.size() << endl;
    for (auto scv_: scv) {
        cout << "* "; 
        for (auto sc1: scv_) {
            cout << " " << sc1;
        }
        cout << endl;
    }
    */

    set<string> cv = scv[scv.size() - 1];
    scv.pop_back();

    // case: only 1 component (all center nodes)
    //       or 2 components (center nodes and other)
    if (scv.size() <= 1) {
        fm.push_back(m[igc]);
        m.erase(m.begin() + igc);

        if (scv.size() == 1) {
            m.push_back(scv[0]);
        }


        return;
    }

    GInfo* gi2 = gi->SubGInfo(m[igc]);
    scv = AssignCenterToComponentSeq(gi2,aprng,scv,cv);
    m.erase(m.begin() + igc);
    copy(scv.begin(),scv.end(),back_inserter(m));
}

// chooses the component w/ the greatest degree
int UGModule::ChooseGreatestComponent() {
    assert (m.size() > 0);

    vector<pair<int,int>> vp;
    for (int i = 0; i < m.size();i++) {
        vp.push_back(make_pair(i,m[i].size()));
    }

    return TieBreakerMinORMax(vp,aprng,false);
} 

//// TODO: test this
//
// splits the component at index `mindex` using its center
// nodeset.
// 
// - return:
// {components w/o center...center}
vector<set<string>> UGModule::SplitComponent(int mindex) {
    assert (mindex >= 0 && mindex < m.size());
    
    vector<set<string>> vss;
    set<string> sm = m[mindex];

    // make another GInfo and get the new center
    GInfo* gi2 = gi->SubGInfo(sm);
    set<string> gi2s = gi2->Center();
    
    // get the connected components for the new subgraph
    set<string> sm2 = SetComplement(sm,gi2s);
    GInfo* gi3 = gi->SubGInfo(sm2); 
    vector<set<string>> cps = (gi3)->Components();
    cps.push_back(gi2s);
    return cps;
}

void UTComponentGraph::ProcessUGModule(UGModule* ugm) {

    // declare each node in the component graph
    utg = new UTGraph();
    key_index.clear();

    // add a node for each component
    ///cout << "adding node for each component" << endl;
    int sz = ugm->fm.size();
    for (int i = 0; i < sz; i++) {
        utg->AddNode(make_pair(to_string(i),"t"));
        key_index[to_string(i)] = ugm->fm[i];
    }

    // add edges b/t connected components
    for (int i = 0; i < sz - 1; i++) {

        for (int j = i + 1; j < sz; j++) {
            if (PairwiseComponentConnectivity(ugm,i,j).first) {
                utg->AddEdge(make_pair(to_string(i),"t"),
                    make_pair(to_string(j),"t"));
            }
        }
    }
}

pair<bool,pair<int,int>> UTComponentGraph::PairwiseComponentConnectivity(UGModule* ugm, int ci1, int ci2) {

    set<string> q1 = (ugm->fm)[ci1];
    set<string> q2 = (ugm->fm)[ci2];
    pair<int,int> score = make_pair(0,0);
    bool stat = false;

    for (auto a1: q1) {
        for (auto a2: q2) {
            ///cout << "nodes " << a1 << " " << a2 << endl;
            if ((ugm->utg)->nodes[a1]->IsNeighbor(a2)) {
                score.first = score.first + 1;
                stat = true;
            }
            score.second = score.second + 1;
        }
    }

    return make_pair(stat,score);
}

set<string> UTComponentGraph::ComponentSet() {
    set<string> sx;
    for (auto p: key_index) {
        sx.insert(p.first);
    }
    return sx;
}

string UTComponentGraph::UTCGRefNodeOfOldNode(string old_node) {
    for (auto x: key_index) {
        if (x.second.find(old_node) != x.second.end()) {
            return x.first;
        }
    }

    return "";
}

string UTComponentGraph::ToString() {

    string s = "";
    for (auto x: key_index) {
        s += "* " + x.first;
        s += " ";
        for (auto x2: x.second) {
            s += x2 + " ";
        }
        s += "\n";
    }
    return s;
}
