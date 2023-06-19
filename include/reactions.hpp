#ifndef REACTIONSINYOURFACE_HPP
#define REACTIONSINYOURFACE_HPP

#include "dmd_traveller.hpp"
#include "utgs.hpp"
#include "dinst.hpp"
#include "randumb.hpp"

///mat RTMODSEQ(DMDTraveller* dmdt, UTGSwapper* utgs,std::vector<std::string> vs);
DInstSeq* RTMODINSTR(DMDTraveller* dmdt, UTGSwapper* utgs, DInst* di, std::vector<std::string> vs);

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









void MODDINST(DMDTraveller* dmdt, DInst* di, AbstractPRNG* aprng,bool mutabl);

DInstSeq* RTMODMAP(DMDTraveller* dmdt, UTGSwapper* utgs, std::vector<std::string> vs);

/// methods used to modify point sequence
mat REFERENTIAL_PAIRWISE_OP_ON_SEQUENCE();
mat REFERENTIAL_PERM_BY_APRNG_ON_SEQUENCE();
///mat CLONE_DERIVE(DMDTraveller* dmdt,AbstractPRNG* aprng, std::vector<std::string> index_pattern);

#endif