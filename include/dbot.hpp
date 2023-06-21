
#ifndef DBOT_HPP
#define DBOT_HPP

#include "rinst.hpp"
#include "gen_struct.hpp"
//#include "dinst.hpp"
//#include "data_reader.hpp"
//#include "instruction_key.hpp"
//#include <string>


// possible deineation schemes
static std::set<std::string> possible_dschemes = {"nodup","nocross","nojag"};

///AbstractPRNG* APRNGFromString(std::string s);

/// delineation bot
class DBot {

public:

    /// filepath for instruction keys
    std::string ifp;
    /// filepath for reaction keys
    std::string rfp;
    /// filepath for dataset used by DMDTraveller
    std::string dfp;
    /// is_filepath,
    /// [generate graph] deg_range UNDERSCORE aprng string used by UTGSwapper UNDERSCORE string args for PermLCG
    /// [load from file] (filepath for UTGraph|aprng string to generate UTGraph)
    std::pair<bool,std::pair<std::string,std::string>> utgfp;
    std::vector<std::string> cs;
    std::map<std::string,DInstSeq*> mcd; 
    std::vector<DInstSeq*> rk;

    // reader for the instruction keys
    DataReader* dri;
    // reader for the reaction keys
    DataReader* drr;
    // reader for the dataset
    DataReader* drd;

    DMDTraveller* dmdt;
    UTGSwapper* utgs;

    DBot(std::string ifp, std::string rfp,std::string dfp,std::pair<bool,
        std::pair<std::string,std::string>> utgfp, std::vector<std::string> character_sequence) {
        this->ifp = ifp;
        this->rfp = rfp;
        this->dfp = dfp;
        this->utgfp = utgfp;
        dri = new DataReader(ifp,50);
        if (this->rfp != "") {
            drr = new DataReader(rfp,50);
        }

        drd = new DataReader(dfp,50);
        cs = character_sequence;
    }

    void LoadFiles();
    void LoadDMDT();
    void LoadIKey();
    void LoadRKey();
    void LoadUTGS();
    bool CheckGraph(UTGraph* utg);
    DInstSeq* LoadOneCommand(bool is_ik);
    std::pair<mat,int> OneChar(std::string c);
};

#endif
