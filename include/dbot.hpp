
#ifndef DBOT_HPP
#define DBOT_HPP

#include "dinst.hpp"
#include "data_reader.hpp"
//#include "instruction_key.hpp"
//#include <string>


// possible deineation schemes
static std::set<std::string> possible_dschemes = {"nodup","nocross","nojag"};

///AbstractPRNG* APRNGFromString(std::string s);

/// delineation bot
class DBot {

public:

    std::string ifp;
    std::string rfp;
    std::string dfp;
    std::vector<char> cs;
    std::map<char,DInstSeq*> mcd; 
    std::vector<DInstSeq*> rk;

    // reader for the instruction keys
    DataReader* dri;
    // reader for the reaction keys
    DataReader* drr;
    // reader for the dataset
    DataReader* drd;

    DMDTraveller* dmdt;

    DBot(std::string ifp, std::string rfp,std::string dfp, std::vector<char> character_sequence) {
        this->ifp = ifp;
        this->rfp = rfp;
        this->dfp = dfp;
        dri = new DataReader(ifp,50);
        if (this->rfp != "") {
            drr = new DataReader(rfp,50);
        }

        drd = new DataReader(dfp,50);
    }

    void LoadFiles();

    void LoadDMDT();
    bool IsDMDTInst();

    void LoadIKey();
    void LoadRKey();
    DInstSeq* LoadOneCommand(bool is_ik);
    mat DoOneChar(char c);
};

#endif
