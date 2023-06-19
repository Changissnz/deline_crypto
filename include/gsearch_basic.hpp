#ifndef GSEARCHBASIC_HPP
#define GSEARCHBASIC_HPP

#include "gmech.hpp" 
#include <deque>
//#include <map>
// basic DFS and BFS search on graph.

static int DEFAULT_PATH_MAX_PARENT_SIZE = 3;
static int DEFAULT_MAX_PATH_SEARCH_SIZE = 1000;

// data structure can be used for machine-learning purposes.
class MultiDirectionalPath {

    std::string reference;
    std::vector<std::vector<std::string>> mdp;

    MultiDirectionalPath() {

    }

    MultiDirectionalPath(std::vector<std::vector<std::string>> mdp) {
        this->mdp = mdp;
    }

    void AddNext(std::vector<std::string> vs);
    void SetIndex(int i); 
    void DirectSelectionNext(int j);
    /// TODO: work on this!
    void PRNGSelectionNext(int j);
};

class BasicSearchStruct {
public:

    std::string nodeIdn;
    std::string parentIdn;
    float distance;

    BasicSearchStruct(std::string ni, std::string pi, float di) {
        nodeIdn = ni;
        parentIdn = pi;
        distance = di;
    }

    void UpdatePathVars(std::string pi, float di);
    std::string ToString();
};

typedef std::deque<std::string> GPath;

class BestPaths {
public:

    std::map<std::string,std::deque<BasicSearchStruct*>> bp;
    std::set<std::string> unrn; // unreachable nodes
    std::string head;

    BestPaths(std::string head,std::map<std::string,std::deque<BasicSearchStruct*>> bp) {
        this->head = head;
        this->bp = bp;
    }

    /// TODO: 
    int KeySize();

    std::string ToString();
    float Eccentricity();

    std::map<std::string,BasicSearchStruct*> ShortestPaths();

    BestPaths* BestPathsByNodesetComplement(std::set<std::string> restricted);
    static void DeleteUnreachableNodes(BestPaths* bp);
    bool IsNodeConnected(std::string ns);
    std::deque<GPath> BackTrace(std::string ns,int max_parent_size=DEFAULT_PATH_MAX_PARENT_SIZE); 
    std::vector<std::string> OrderedParentSet(std::string ns,int parent_size);
    std::set<std::string> ParentSet(std::string ns);
    std::set<std::string> NodeSet();
};

// graph synopsis data structure
class GSynopsis {

public:
    float r;
    float d;
    std::set<std::string> c;

    GSynopsis(float r, float d, std::set<std::string> c) {
        this->r = r;
        this->d = d;
        this->c = c;
    }

    std::string ToString();
};

class GInfo {

public:

    // map from source node to its best paths
    std::map<std::string,BestPaths*> nbp;

    GInfo(std::map<std::string,BestPaths*> nbp) {
        this->nbp = nbp;
    }

    std::map<std::string,float> Eccentricities();
    float Radius();
    float Diameter();
    std::set<std::string> Center();

    vec EccentricityVec();

    GSynopsis* BasicPathInfo();
    GInfo* SubGInfo(std::set<std::string> nodeset);
    std::vector<std::set<std::string>> Components();
    float AverageDistanceToNodeset(std::set<std::string> ns,std::string rn);

    float N2NDistance(std::string n1,std::string n2);
    
};

/// TODO: after Eccentricities, do "subgraph_by_nodeset"
///       that updates the best paths to available best paths.


class GSearch {
public:

    UTGraph* utg;
    BestPaths* btg;

    GSearch(UTGraph* utg) {
        this->utg = utg;
    }

    /// TODO: work on this.
    GInfo* RunGInfo();

    BestPaths* BFSOnNodeBestMPathsPerNode(std::string n,int p);
    std::map<std::string,BasicSearchStruct*> BFSOnNode(std::string n);
};

#endif