#ifndef GENSTRUCT_HPP
#define GENSTRUCT_HPP
#include "gmech.hpp" 

static std::vector<std::string> STD_NODE_LIST = {
    "A","a","B","b","C","c","D","d","E","e",
    "F","f","G","g","H","h","I","i","J",
    "j","K","k","L","l","M","m","N","n",
    "O","o","P","p","Q","q","R","r","S",//
    "s","T","t","U","u","V","v","W","w",
    "X","x","Y","y","Z","z","0","1","2",
    "3","4","5","6","7","8","9","!","@",
    "#","$","%","^","&","*","(",")","-",
    "_","+","=","{","}","[","]","|","\\",
    ":",";","\"","'","<",",",">",".","?",
    "/","`","~"," "};

// undirected simple graph generator
class UGraphGen {

public:

    int num_nodes;
    std::pair<int,int> deg_range;
    AbstractPRNG* aprng;
    mat mx;
    std::set<int> excluded;

    UGraphGen(int num_nodes, std::pair<int,int> deg_range, AbstractPRNG* aprng) {
        assert(deg_range.first <= deg_range.second);
        assert(deg_range.second < num_nodes);
        assert(num_nodes > 0);
        this->num_nodes = num_nodes;
        this->deg_range = deg_range;
        this->aprng = aprng;
        mx = mat(num_nodes,num_nodes,fill::zeros);
    }

    void Generate();
    void ConnectTargetNode();

};

class UTGraphGen {

public:

    std::vector<std::string> char_list;
    UGraphGen* ugg; 
    PermLCG* plcg;
    UTGraph* utg;

    UTGraphGen(std::vector<std::string> char_list,
        std::pair<int,int> deg_range, AbstractPRNG* aprng, PermLCG* plcg) {
        
        this->char_list = char_list;
        ugg = new UGraphGen(char_list.size(),deg_range,aprng);
        this->plcg = plcg;
    }

    void LoadUTGraph();

};




#endif