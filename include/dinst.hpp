#ifndef DINST_HPP
#define DINST_HPP

#include "dmd_traveller.hpp"
#include "data_reader.hpp"
#include "accessory_functions.hpp"

static set<string> oneapp = {"MO","BG","HOP"};
static set<string> allapp = {"MSK"};

class DInst {

public:

    std::string ins;
    std::vector<std::string> inst_args;

    DInst(std::string inst,std::vector<std::string> inst_args) {
        this->ins = inst;
        this->inst_args = inst_args;
        PrecheckArgs();
    }

    void PrecheckArgs(); 
    mat MapMSK(DMDTraveller* dt,mat m1,mat m2,ivec m2i);
    void MapBG(DMDTraveller* dt);
    void MapMO(DMDTraveller* dt);
    void MapHOP(DMDTraveller* dt);
    DInst* Copy();
};

class DInstSeq {

public:

    std::vector<std::string> s;

    /// TODO: variables used for future development 
    std::vector<DInst*> one_apply;
    std::vector<DInst*> all_apply;
    mat output;

    DInstSeq(std::vector<std::string> s) {
        this->s = s;
    }

    void ParseIntoDInst(); 
    void Map(DMDTraveller* dt);
    int MOPointSize(DMDTraveller* dt);

    std::pair<mat,std::pair<mat,ivec>> MOsequence(DMDTraveller* dt);
    std::vector<DInst*> DInstCategory(std::string category);
};

int MSKToInt(std::string msk);
std::string IntToMSK(int i);

int BGToInt(std::string bg);
std::string IntToBG(int i);

#endif