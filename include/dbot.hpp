#ifndef DBOT_HPP
#define DBOT_HPP

#include "rinst.hpp"
#include "gen_struct.hpp"

// possible deineation schemes
static std::set<std::string> possible_dschemes = {"nodup","nocross","nojag"};

/// delineation bot
class DBot {

public:

    /// filepath for instruction keys
    std::string ifp;
    /// filepath for reaction keys
    std::string rfp;
    /// filepath for dataset used by DMDTraveller
    std::string dfp;
    /// filepath for delineation instructions
    std::string difp;

    /// [0] is_filepath,
    /// [1,0] aprng string for UTGSwapper
    /// [1,1] 
    ///         if is_filepath => filepath 
    ///         otherwise => deg_range UNDERSCORE aprng string used by UTGSwapper UNDERSCORE string args for PermLCG
    std::pair<bool,std::pair<std::string,std::string>> utgfp;
    std::vector<std::string> cs;
    std::map<std::string,DInstSeq*> mcd; 
    
    //Reactor* reactor;
    std::vector<RInst*> rk;

    // reader for the instruction keys
    DataReader* dri;
    // reader for the reaction keys
    DataReader* drr;
    // reader for the dataset
    DataReader* drd;
    // reader for the dataset instructions
    DataReader* drdi;

    DMDTraveller* dmdt;
    UTGSwapper* utgs;

    DBot(std::string ifp, std::string rfp,std::string dfp,std::string difp, std::pair<bool,
        std::pair<std::string,std::string>> utgfp, std::vector<std::string> character_sequence) {
        this->ifp = ifp;
        this->rfp = rfp;
        this->dfp = dfp;
        this->difp = difp;
        this->utgfp = utgfp;
        dri = new DataReader(ifp,50);
        if (this->rfp != "") {
            drr = new DataReader(rfp,50);
        }

        drd = new DataReader(dfp,50);
        drdi = new DataReader(difp,50);
        cs = character_sequence;
    }

    void LoadFiles();
    void LoadDMDT();
    std::pair<DelineMD*,AbstractPRNG*> LoadDMD();
    void LoadIKey();
    void LoadRKey();
    void LoadUTGS();
    bool CheckGraph(UTGraph* utg);
    std::pair<DInstSeq*,RInst*> LoadOneCommand(bool is_ik);
    std::pair<std::pair<mat,int>,std::pair<std::string,std::string>> OneChar(std::string c);
    std::pair<DInstSeq*,std::string> CharToInstr(std::string c);
    int CharToInt(std::string c);
};

#endif
