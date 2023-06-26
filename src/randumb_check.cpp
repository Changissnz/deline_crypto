#include "randumb_check.hpp"
using namespace std; 

int MAX_GAUGE_SIZE = 10000; 
float POINT_RADIUS_RATIO = 0.005;

float CoverageOfSequence(vec vf, std::pair<float,float> fr,float pr) {
    assert (fr.second > fr.first);
    vector<pair<float,float>> vpf = NonIntersectingActivationRangesOfPointSequence(vf,fr,pr);
    float fx = 0;

    for (auto vpf_: vpf) {
        fx += (vpf_.second - vpf_.first);
    }

    return RoundDecimalNPlaces(fx / (fr.second - fr.first),5);
}


/// - description: 
///         *procedure*
/// Iterate through each value, and assign its radial range to
/// vfx. If the range intersects w/ that of the previous range in
/// vfx, modify it so the two ranges do not intersect.
///
/// CAUTION: assume the vector `vf` is sorted in ascending order.
vector<pair<float,float>> NonIntersectingActivationRangesOfPointSequence(vec vf, pair<float,float> fr,float pr) {
    assert(pr > 0);

    vector<pair<float,float>> vfx;
    pair<float,float> bs;
    vf = unique(vf);

    for (auto vf_: vf) {
        vec f1 = {vf_ - pr,fr.first};
        vec f2 = {vf_ + pr,fr.second};
        bs = make_pair(f1.max(),f2.min());
        
        // check for intersection w/ the previous
        if (vfx.size() > 0) {
            bs = CorrectIntersectingRanges(vfx[vfx.size() - 1],bs);
        }

        vfx.push_back(bs);
    }

    // trim the boundary ranges
        // min boundary 
    int j = 0;
    float fx = 0;
    for (int i = 0; i < vfx.size(); i++) {
        if (vfx[i].first == fr.first) {
            j = i;
            fx = vfx[i].second;
            continue;
        }
        break;
    }
        // case: duplicate ranges containing min of `fr`
    if (j > 0) {
        vfx.erase(vfx.begin(),vfx.begin() + j + 1);
        vfx.insert(vfx.begin(), make_pair(fr.first,fx));
    }

        // max boundary
    j = -1;
    fx = 0;
    for (int i = 0; i < vfx.size(); i++) {
        if (vfx[i].second == fr.second) {
            j = i;
            fx = vfx[i].first;
            break;
        }
    }

        // case: duplicate ranges containing max of `fr`
    if (j != vfx.size() - 1) {
        vfx.erase(vfx.begin() + j,vfx.end());
        vfx.insert(vfx.end(),make_pair(fx,fr.second));
    }

    return vfx;
}

/// corrects two ranges if they intersect so that the resultant range `f2'` and
/// `f1` have the same cumulative span as before the correction.
///
/// CAUTION: method designed for use w/ `NonIntersectingActivationRangesOfPointSequence`.
pair<float,float> CorrectIntersectingRanges(pair<float,float> f1,pair<float,float> f2) {
    if (f2.first >= f1.first && f2.first <= f1.second) {
        f2.first = f1.second;
    } else if (f2.first < f1.first) {
        f2.first = f1.second; 
    }

    return f2;
}

/// used to gauge the ordering of elements in a vector.
float UnidirectionalWeighedPairwiseDistance(vec v) {
    int l = v.size();
    float s = 0.;

    for (int i = 0; i < l - 1; i++) {
        for (int j = i + 1; j < l; j++) {
            s += (abs(v(i) - v(j)) / (j - i));
        }
    }

    return s;
}

/// normalized unidirectional weighed pairwise distance
///
/// CAUTION: does not check for validity of range `fr`. 
float NormalizedUWPD(vec v, pair<float,float> fr) {
    assert(fr.second >= fr.first);
    float fx1 = UnidirectionalWeighedPairwiseDistance(v);
    float fx2 = MaxUWPD(fr,v.size());

    if (fx2 == 0.) {
        return 0.;
    }
    return RoundDecimalNPlaces(fx1 / fx2,5);
}

/// min max min max
float MaxUWPD(pair<float,float> fr, int vs) {
    int j = 0;
    vector<float> vf;
    for (int i = 0; i < vs; i++) {
        if (j) {
            vf.push_back(fr.second);
        } else {
            vf.push_back(fr.first);
        }
        j = (j + 1) % 2;
    }
    
    return UnidirectionalWeighedPairwiseDistance(conv_to<vec>::from(vf));
}

//////////////////////////////////////////////////////////////////////////////////////////

/// gauges the output of the AbstractPRNG* by two measures
/// - coverage: uses the method `
/// - ordering
void APRNGGauge::MeasureCycle(bool floatgen) {
    vec vf = conv_to<vec>::from(CycleOne(floatgen));
    vf = unique(vf);
    cout << "vec is: " << vf << endl;
    if (point_radius == 0.) {
        DefaultSetRadius(vf);
    }

    float f1 = CoverageOfSequence(vf,fr,point_radius);
    float f2 = NormalizedUWPD(vf,fr);
    rowvec rx = {f1,f2}; 
    cycle_scores.insert_rows(cycle_scores.n_rows,rx);
}

void APRNGGauge::DefaultSetRadius(vec v) {
    float f1 = v.min();
    float f2 = v.max();
    point_radius = (f2 - f1) * POINT_RADIUS_RATIO;
}
    
vector<float> APRNGGauge::CycleOne(bool floatgen) {
    return aprng->CycleOne(floatgen,MAX_GAUGE_SIZE); 
}
