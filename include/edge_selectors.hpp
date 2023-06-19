#ifndef EDGESELECTORS_HPP
#define EDGESELECTORS_HPP

#include "extras.hpp"
#include <utility>
#include <armadillo>
using namespace arma;

bool is_valid_point_data(mat PS);
int index_of_next_point_in_direction(mat PS,rowvec p, char direction,int startIndex,bool orderedMat);
int tiebreak_set(mat PS,char direction);

std::pair<ivec,ivec> EliminateContraryPointsIndices(mat es1,mat es2,int axis);
int NextPointInRange(mat ep,std::pair<rowvec,rowvec> edge,int axis,int startIndex);
std::pair<bool,bool> CorrectRectangleCross(std::pair<rowvec,rowvec> l1,std::pair<rowvec,rowvec> l2,int axis);
std::pair<std::pair<bool,bool>,std::pair<bool,bool>> LineSegmentsCrossInRectangle(std::pair<rowvec,rowvec> l1,std::pair<rowvec,rowvec> l2);
bool PointInDiagArea(rowvec p,std::pair<rowvec,rowvec> d);
int CloserEdge(rowvec e1,rowvec e2,rowvec p,int axis);

std::pair<mat,mat> EliminateDuplicatePoints(mat es1,mat es2,int axis);
int AssignDuplicatePoint(rowvec em1,rowvec em2,int el1,int el2,int axis,rowvec p);

// outputs indices corresponding to jagged edges of point sequence
ivec JaggedEdges(mat PS, char direction); 
bool IsJagged(rowvec e1,rowvec e2, char direction);

mat SortPoints(mat PS,int col);
mat SortMatByIndices(mat PS,uvec indices);
int SearchOrderedMatForRowMaxIndex(mat m, rowvec r, int start_index);
int SearchMatForRow(mat m, rowvec r, int start_index);

#endif
