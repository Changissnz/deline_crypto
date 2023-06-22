
#ifndef RDBOT_HPP
#define RDBOT_HPP

#include "dbot.hpp"

class Reactor {

    std::vector<RInst*> anytimer;
    std::vector<RInst*> forr;

public:

    Reactor(DBot* db) {
        LoadInst(db->rk);
        db->rk.clear();
    }

    void LoadInst(std::vector<RInst*> ri);
    void React(DBot* db,std::pair<std::string,std::string> output);
    void ReactOne(DBot* db, RInst* ri,std::pair<std::string,std::string> output);
    void ReactOnTargets(DBot* db, RInst* ri);
    RInst* ReactantForChar(string s);
};

#endif