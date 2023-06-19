#ifndef DELINEATOR_HPP
#define DELINEATOR_HPP

#include "dcurve.hpp"
#include "edge_selectors.hpp"

#include <iostream>

class Delineator {

public:

    mat data;
    mat data2;

    std::vector<DCurve> curves;
    std::pair<std::pair<int,int>,std::pair<int,int>> extremum;
    rowvec mn;
    
    bool cw;
    std::string app;
    mat edgeL;
    mat edgeR;
    mat edgeT;
    mat edgeB;

    Delineator(mat d,bool clockwise,std::string approach) {
        data = d;
        cw = clockwise;
        assert (approach == "nojag" || approach == "nodup" || approach == "nocross");
        app = approach;
        SimpleAnalysis();
    }

    void SimpleAnalysis();
    void CROps();
    void BuildDefaultCurveSequence();
    std::vector<char> ClockwiseOrder();
    void RebuildDefaultCurveSequence();

    void AddEdgeToCurveSequence(char direction);
    mat Edge(char direction);
    void AssignEdge(mat mx, char direction);

    rowvec InitializeCarve(char direction);
    void CarveEdge(char direction);
    void RefineEdge(int axis);
    void OrderPointsByAxis(int axis);
    std::pair<int,int> IndexRangeForCurveDirection(char direction);
    std::vector<int> ComplementForCurve(int curve_index);
    bool FlowOfDirection(char direction);
    int IndexOfDCurveInPointRange(rowvec p, char d);
}; 

#endif