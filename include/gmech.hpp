#ifndef GMECH_HPP
#define GMECH_HPP

#include "extras.hpp"
#include "randumb.hpp"
#include "data_reader.hpp"

/// TODO: consider adding an extra attribute for colored case. 

// undirected token node 
class UTNode {

public:
    std::string c;
    std::string iidt; // instruction identifier token
    std::vector<UTNode*> neighbors;

    UTNode(std::string c, std::string iidt) {
        this->c = c;
        this->iidt = iidt;
    }

    UTNode* PartialCopy();


    void AddNeighbor(UTNode* utn) {
        if (IsNeighbor(utn->c)) {
            return;
        }

        neighbors.push_back(utn);
    };

    void DeleteNeighbor(std::string nid);

    bool IsNeighbor(std::string n);

    std::string ToString();
    std::vector<std::pair<std::string,std::string>> NeighborsStringVec();


};

/*
class FFactorAlg {
};
*/

// wanted token-to-vertex pairs
class WantedTVPairs {

};


/// - string of the format
///  node1.node2,node3.node4,...
///
class SwapSequence {
public:

    std::string dct;

    SwapSequence(std::string data_collect_type) {
        dct = data_collect_type;
    }

    void AddEdge(std::pair<std::string,std::string> ps);
};

class UTGraph {
public:

    std::map<std::string,UTNode*> nodes;

    // 
    UTGraph() {
    }

    void Display();

    std::string ToString();
    void NodeFromString(std::string s);
    //UTNode AddNodeByString(std::string s);

    static UTGraph FromFile(std::string fp);
    static UTGraph FromUTNodeVec(std::vector<UTNode*> utnv);

    void AddEdge(std::pair<std::string,std::string> n1, std::pair<std::string,std::string> n2);
    bool AddNode(std::pair<std::string,std::string> n);
    void AddNode(UTNode* utn);

    UTGraph* SubgraphByNodeset(std::set<std::string> ns);

    /// TODO: 
    bool DelNode();
    void WriteToFile(std::string fp);


    UTNode* Node(std::string c);
    std::vector<std::string> NodesOfToken(std::string t);
    std::string NodeOfToken(std::string t);

    int MaxNodeDegree();

    //////// TODO: delete these methods; no longer needed.    
    //void InfoProc();

    //void Routine1(AbstractPRNG* ag);
    //void Routine2(AbstractPRNG* ag);
    //void Routine3(AbstractPRNG* ag);

    //std::string LabelSwap(pair<string,string>);
    //void ConsiderBestSwaps();

    //std::pair<float,float> SwapScore(std::pair<std::string,std::string> p);
    //void SwapEdge(std::pair<std::string,std::string> p);


    float CumulativeDistanceMeasure();

    //int MinumumCumulativeDistance();
};

#endif 