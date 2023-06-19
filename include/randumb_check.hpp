#ifndef RANDUMBCHECK_HPP
#define RANDUMBCHECK_HPP

#include "randumb.hpp"

std::pair<float,float> ScoreAPRNG();

/// functions used to gauge coverage
float CoverageOfSequence(vec vf, std::pair<float,float> fr,float pr);
std::vector<std::pair<float,float>> NonIntersectingActivationRangesOfPointSequence(vec vf, std::pair<float,float> fr,float pr);
std::pair<float,float> CorrectIntersectingRanges(std::pair<float,float> f1,std::pair<float,float> f2);
//std::vector<std::pair<float,float>> NonIntersectingActivationRangesOfPointSequence(std::vector<float> vf, std::pair<float,float> fr,float pr);
//std::pair<float,float> CorrectIntersectingRanges(std::pair<float,float> f1,std::pair<float,float> f2);

/// functions used to gauge ordering
float UnidirectionalWeighedPairwiseDistance(vec v);
float NormalizedUWPD(vec v, std::pair<float,float> fr);
float MaxUWPD(std::pair<float,float> fr, int vs);

/// class is used to gauge the coverage and ordering of the pseudo-random
/// generator output. 
class APRNGGauge {

public:
    AbstractPRNG* aprng;
    std::pair<float,float> fr;

    std::set<float> cover_set;
    
    float point_radius;

    float reference;
    bool init;

    mat cycle_scores;

    APRNGGauge(AbstractPRNG* aprng, std::pair<float,float> fr, float point_radius = 0.) {
        assert(fr.second > fr.first);
        this->aprng = aprng; 
        this->fr = fr;
        this->point_radius = point_radius;
        cycle_scores = mat(0,0,fill::zeros);
    }

    void MeasureCycle(bool floatgen);
    void DefaultSetRadius(vec v);
    std::vector<float> CycleOne(bool floatgen);
    void GaugePROutput();
    void GauagePROutputReferential(vec r);
  
};

#endif