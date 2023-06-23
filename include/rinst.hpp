#ifndef RINST_HPP
#define RINST_HPP

#include "reactions.hpp"
#include "rinst_conditions.hpp"

class RInst {
public:
    /// initial parameter that needs to be parsed
    std::vector<std::string> inf;

    /// ANYTIME|FOR
    std::string c1;
    /// CONDITION|INSTRUCTION
    std::string c2;
    /// CONDITION|INSTRUCTION
    std::string c3;
    /// CONDITION|INSTRUCTION 
    std::string c4;

    std::string cond_args;
    std::vector<std::string> reaction_args;
    std::vector<std::string> reaction_targets;

    RInst(std::vector<std::string> inf) {
        this->inf = inf;
        Parse();
    }

    void Parse();
    void Parse_();
    bool ReactDecision(DMDTraveller* dmdt,DInstSeq* dis,std::string chrcter);
};

#endif