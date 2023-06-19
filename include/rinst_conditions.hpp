#ifndef RINSTCONDS_HPP
#define RINSTCONDS_HPP

#include "dmd_traveller.hpp" 
#include "dinst.hpp"

bool C1__IN_BOUNDS_OF_DELINEATION(DMDTraveller* dmdt,DInstSeq* di,std::string desc);
ivec C1__IN_BOUNDS_OF_DELINEATION_(DMDTraveller* dmdt,DInstSeq* di);

bool C2__ARITHMETICSEQ_IN_MOD_RANGE(DMDTraveller* dmdt,DInstSeq* di,std::string desc);
vec C2__ARITHMETICSEQ_IN_MOD_RANGE_(DMDTraveller* dmdt,DInstSeq* di, std::string desc);

bool C3__POINTSEQ_INTERSECTS_W_PREVIOUS_SPAN(DMDTraveller* dmdt,DInstSeq* di,std::string desc);
ivec C3__POINTSEQ_INTERSECTS_W_PREVIOUS_SPAN_(DMDTraveller* dmdt,DInstSeq* di);

bool C4__POINTSEQ_MULTICLASS_MEASURE(DMDTraveller* dmdt,DInstSeq* di, std::string desc);
ivec C4__POINTSEQ_MULTICLASS_MEASURE_(DMDTraveller* dmdt, DInstSeq* di,bool mode_h);

bool C5__SEQUENTIAL_EUCLIDEAN_DISTANCE_MEASURE(DMDTraveller* dmdt,DInstSeq* di,std::string desc);
float C5__SEQUENTIAL_EUCLIDEAN_DISTANCE_MEASURE_(DMDTraveller* dmdt,DInstSeq* di);

bool C6__BOOLEAN_DELTA_MEASURE_ALONG_AXIS(DMDTraveller* dmdt,DInstSeq* di,std::string desc);
ivec C6__BOOLEAN_DELTA_MEASURE_ALONG_AXIS(DMDTraveller* dmdt,DInstSeq* di, char direction);

mat PreviousSequence(DMDTraveller* dmdt,int l);

#endif
