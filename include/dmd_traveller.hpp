#ifndef DMDTRAVELLER_HPP
#define DMDTRAVELLER_HPP

#include "deline_md.hpp"
#include "randumb.hpp"
#include "data_reader.hpp"

/// TODO: estimation for runtime/memory funtions.
///////

std::pair<char,int> IncrementNextIndexInEdges(std::vector<std::pair<char,int>>* mx, std::vector<int> szs, char direction);

rowvec DeltaOnCoord(rowvec p, std::pair<float,float> dx, float sgn, int axis, AbstractPRNG* aprng);
rowvec DeltaOnPoint(std::pair<float,float> rr, rowvec p, float r0, float r1,AbstractPRNG* aprng);

void JPMomentum(Deline22* d,AbstractPRNG* aprng, vec iv);
vec ModifyMomentum(Deline22* d,AbstractPRNG* aprng, vec iv,int index);
///void JitterPatternRadius(Deline22* d, ivec iv);

class DelineModInstructions {
public:

    int iterations;
    std::string inst_description;
    // arguments for the instruction function are in string format
    std::vector<std::string> parameters;
};

class DMDTraveller {

public:

    DelineMD* dmd; 
    AbstractPRNG* aprng;
    float excess_res; // used to determine length of computation before derivative.
    mat travel_points;
    mat tp_considered;
    vector<int> tpc_indices;

    // add a random generator
    DMDTraveller(DelineMD* dmd, AbstractPRNG* aprng) {
        this->dmd = dmd;
        this->aprng = aprng;

        travel_points = mat(0,0,fill::zeros);
        tp_considered = mat(0,0,fill::zeros);
    }

    float EstimateAction(); // (restrained action case), used to determine the resources to calculate the action.
    void Act(DelineModInstructions dmi);
    
    void AlterDeline22(Deline22* d22, std::pair<float,float> rr,bool expand);
    void AlterDeline22Edge(Deline22* d22, std::pair<float,float> rr, char direction,bool expand); 
    void AlterDeline22VertexPoints(Deline22* d22, std::pair<float,float> rr, bool expand);
    void AlterDeline22VertexPointsOfEdge(Deline22* d22, char direction, char postEdgeDir, std::pair<float,float> rr, bool expand);
    rowvec AlterNonVertexPoint(rowvec p,rowvec prev, rowvec post, char direction, bool expand, std::pair<float,float> rr);
    
    void JitterDeline22(Deline22* d22, vec v, std::function<void (Deline22*,AbstractPRNG*,vec)> jitterPatternFunc);
    rowvec MoveOne(Deline22* d22, int index, vector<char> directions,vec pointPace,bool assignToClosest); // vector<directions>
    rowvec MoveOnePoint_(Deline22* d22, int index, vector<char> directions,vec pointPace);

    mat SelectPointsByReference(Deline22* d22, int index, vector<char> directions);

    void ClearTravelData();

    Deline22* SelectD22(int dmci, int d22i);
    Deline22MC* SelectDMC(int dmci); 

    std::pair<int, std::map<int,int>> Info();
    
    std::pair<int,int> ModuloOnD22Index(int dmci,int d22i);
    mat LastNthTPCSequence(int i);
    std::pair<int,int> LastNthSequenceIndices(int i);

    /// TODO: swap orientation
};

DMDTraveller* DMDTravellerCase1();

#endif