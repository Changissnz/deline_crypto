#ifndef KEYGEN_HPP
#define KEYGEN_HPP

#include "rinst.hpp"
#include "dinst.hpp" 
#include "randumb.hpp"
#include "gen_struct.hpp"

static std::vector<std::string> STD_ARITHMETIC_OPS = {"+","-","*","/"};

/// CAUTION: using this key with APRNG instances that "score poorly" on
///          variance tests might result in erroneous outcomes.
class KeyGen {

public:

    AbstractPRNG* rg1;
    AbstractPRNG* rg2;

    std::string f1;
    std::string f2;
    //std::pair<std::pair<int,int>,std::vector<int>> dmddim;
    std::pair<int,std::map<int,int>> dmddim;
    std::vector<std::string> ck;
    // used for unique selection
    std::vector<std::string> ck2;
    int num_reactions;

    std::vector<std::vector<std::string>> ckey_genrd;
    std::vector<std::vector<std::string>> rkey_genrd;
    bool erroneous;

    /// rg1 := used for initial generation of (I|R)Key 
    /// rg2 := used for modification of (I|R)Key based on their similarity/difference
    ///        to other keys (that have already been generated). 
    /// f1 := file to write out the generated CKeys `ikey_genrd`.
    /// f2 := file to write out the generated RKeys `rkey_genrd`.
    /// dmddim := [0] number of DelineMD's
    ///           [1] number of Deline22's for each DelineMD
    KeyGen(AbstractPRNG* rg1, AbstractPRNG* rg2,
        std::string f1, std::string f2, std::pair<int,std::map<int,int>> dmddim,
        std::vector<std::string> char_keys=STD_NODE_LIST,int num_reactions = STD_NODE_LIST.size()) {
        //int cl) {
        this->rg1 = rg1;
        this->rg2 = rg2;
        this->f1 = f1;
        this->f2 = f2;
        this->dmddim = dmddim;
        this->ck = char_keys;
        this->ck2 = char_keys;
        this->num_reactions = num_reactions;
        erroneous = false;
    }

    void Out();
    void OutCKeyBasic();
    std::vector<std::string> OutOneCKeyBasic();
    std::string OutMSK();
    std::string OutMO();
    std::string OutBG();
    std::string OutHOP();
    pair<int,int> OutColumnD22Pair();
    
    void OutRKeyBasic();
    void OutOneRKeyBasic();
    void OutANYTIME();
    void OutFOR();

    pair<std::string,std::string> OutCONDITION();
    std::string OutCONDITIONALDESC(std::string desc);
    std::string OutCONDITIONALARITHMETIC();
    std::string OutRANGE();

    std::vector<std::string> OutTAKEREACTIONAS();
    std::string OutUTGSCommandSwap();


    void WriteToFile(std::pair<std::string,std::string> ps);
    void WriteK(std::string fp, bool is_ck);
    void WriteCommand(ofstream* fx,int i,bool is_ck);

};

#endif