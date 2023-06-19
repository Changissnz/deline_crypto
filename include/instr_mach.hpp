#ifndef INSTRMACH_HPP
#define INSTRMACH_HPP

#include "instruction_key_table.hpp"

/// INSTRUCTION NOTATION
/*

FUNC.ARG1.ARG2...ARGN. 



*/
class InstructionMachine {

    std::string fp; 
    std::string dfp;
    DBot* db;

    InstructionMachine(std::string fp_, std::string dfp_) {
        fp = fp_;
        dfp = dfp_;

    }

    ApplyNext();
    ReadNext();
}



#endif