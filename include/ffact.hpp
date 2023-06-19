#ifndef FFACT_HPP
#define FFACT_HPP

#include "gsearch_basic.hpp"
#include "extras.hpp"
#include <vector>

static float DEFAULT_COMPONENT_DEG_RATIO = 1.;

/// TODO: test this. 
// method specially designed for vectors of `pair<int,T>` 
template<typename T> 
int TieBreakerMinORMax(std::vector<std::pair<int,T>> vpt, AbstractPRNG* aprng, bool minobj) {

    if (minobj) {
        sort(vpt.begin(),vpt.end(), [](std::pair<int,T> l, std::pair<int,T> r)
        {    
            return l.second < r.second;       
        });
    } else {
        sort(vpt.begin(),vpt.end(), [](std::pair<int,T> l, std::pair<int,T> r)
        {    
            return l.second > r.second;       
        });
    }

    // collect all the ties
    std::vector<std::pair<int,T>> vpt2;
    vpt2.push_back(vpt[0]);
    
    for (int i = 1; i < vpt.size(); i++) {
        if (vpt[i].second == vpt2[0].second) {
            vpt2.push_back(vpt[i]);
        } else {
            break;
        }
    }

    // do another sort
    sort(vpt2.begin(),vpt2.end(), [](std::pair<int,T> l, std::pair<int,T> r)
    {
            return l.first < r.first;
    });

    // select by AbstractPRNG
    int j = aprng->PRIntInRange(std::make_pair(0,vpt2.size() - 1));
    return vpt2[j].first;
}

class FFSol {

    mat sol;
    std::vector<std::vector<std::string>> candidates;
    ivec dindices;
    int isol; // index of solution

    FFSol(mat s,std::vector<std::vector<std::string>> c,int isol) {
        sol = s;
        candidates = c;
        this->isol = isol;
    }

    FFSol* DefaultNext();
    FFSol* SelectedNext(ivec dincies);
};


std::vector<std::set<std::string>> AssignCenterToComponentSeq(GInfo* rgi, std::vector<std::set<std::string>> ctsq,std::set<std::string> cn);

/// CAUTION: data structure calculates modules
/// based on input UTGraph given, and does
/// not consider token-swapping.
///
// undirected graph module that can contain 
// a subgraph that is not a module. 
class UGModule {

public:
    UTGraph* utg;
    AbstractPRNG* aprng;
    int reqm;

    GInfo* gi;
    // represents the components
    std::vector<std::set<std::string>> m;
    // final solution
    std::vector<std::set<std::string>> fm;

    /// NOTE: UNUSED
    std::vector<bool> module_class;

    bool finstat;

    UGModule(UTGraph* utg,AbstractPRNG* aprng, int reqm = 2) {
        this->utg = utg;
        this->aprng = aprng;
        assert (reqm > 1); 
        this->reqm = reqm;
        finstat = false;
    }

    std::string ToString();
    void Preprocess();
    int TotalComponentSize();
    void Split();
    void DefaultSplitComponent();
    int ChooseGreatestComponent();
    std::vector<std::set<std::string>> SplitComponent(int mindex);
    void ClassifyModules();


};

// specialized case of the standard UTGraph;
// not designed to be a recursive class. 
//
// given a module `ugm`, processes it into a smaller
// UTGraph* utg such that the original nodes of a
// UTGraph* utg0 form subsets, each of which is a node
// utg. 
//
// Each node n of the new graph utg has an entry in the class
// variable `key_index`, with value of a set of nodes in the
// graph utg0 corresponding to that node n of utg.
// 
// Typically, none of the nodes `nx` of the graph utg represent a set
// of nodes of utg0 that intersects w/ any other set that represents
// `ns` of utg, where `ns != nx`.  
class UTComponentGraph {

public:

    UTGraph* utg;
    std::map<std::string, std::set<std::string>> key_index;

    UTComponentGraph() {
    }

    void ProcessUGModule(UGModule* ugm);
    std::pair<bool,std::pair<int,int>> PairwiseComponentConnectivity(UGModule* ugm, int ci1, int ci2);

    ///float MeanTokenSolveDistance(std::string s);
    std::set<std::string> ComponentSet();
    std::string UTCGRefNodeOfOldNode(std::string old_node);
    
    std::string ToString();


};


class FFactor {

    // connected graph
    UTGraph* utg;
    
    FFactor(UTGraph* utg) {
        this->utg = utg;
    }

    std::pair<FFSol,ivec> FindFactorBF(std::string mode,bool save_decision_proc);
};

#endif