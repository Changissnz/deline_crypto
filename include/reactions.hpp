#ifndef REACTIONSINYOURFACE_HPP
#define REACTIONSINYOURFACE_HPP

#include "dmd_traveller.hpp"
#include "utgs.hpp"
#include "dinst.hpp"
#include "randumb.hpp"

/// modifies the instruction
DInstSeq* RTMODINSTR(DMDTraveller* dmdt, DInstSeq* di, std::string vs);

void PERMUTEDINST(DInstSeq* di, AbstractPRNG* aprng);

void MODIFYDINST(DMDTraveller* dt,DInstSeq* di, AbstractPRNG* aprng);
void MODIFYDINST_(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng);
void MODIFY_TYPEMO(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng);
void MODIFY_TYPEMSK(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng);
void MODIFY_TYPEBG(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng);
void MODIFY_TYPEHOP(DMDTraveller* dt,DInst* dx, AbstractPRNG* aprng);

void DUPDINST(DInstSeq* di, AbstractPRNG* aprng);
void INSDINST(DMDTraveller* dt, DInstSeq* di, AbstractPRNG* aprng);
void DELDINST(DInstSeq* di, AbstractPRNG* aprng);

void DINST_APRNGINSERT(DInstSeq* di, DInst* dx, AbstractPRNG* aprng);
DInst* SELECTDINST(DInstSeq* di, AbstractPRNG* aprng);

/// modifies the permutation graph (UTGraph)
void RTMODUTG(UTGSwapper* utgs,std::string command);

#endif