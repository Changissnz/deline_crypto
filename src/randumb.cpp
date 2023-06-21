#include "randumb.hpp"
using namespace std;

int ScaleIntRangeToRange(int x, pair<int,int> r1,pair<int,int> r2) {
    return int(round(ScaleFloatRangeToRange(float(x),
        make_pair(float(r1.first),float(r1.second)),
        make_pair(float(r2.first),float(r2.second)),5))); 
}

float ScaleFloatRangeToRange(float x, pair<float,float> r1,pair<float,float> r2,int decimalLength) {
    assert (x >= r1.first && x <= r1.second);
    assert (r2.first <= r2.second); 

    if ((r1.second - r2.second) == 0) {
        return 0;
    }

    float r = (x - r1.first) / (r1.second - r1.first);
    r = round(r2.first + r * (r2.second - r2.first));
    return RoundDecimalNPlaces(r,decimalLength); 
}

int StdRandGenerator::PRIntInRange(pair<int,int> r) {
    assert (r.second >= r.first);
    if (r.second == r.first) {
        return r.first;
    }
    int rx = rand();
    return (rx % (r.second - r.first)) + r.first;
}

float StdRandGenerator::PRFloatInRange(pair<float,float> r,int decimalLength) {
    assert (r.second >= r.first);
    if (r.second == r.first) {
        return r.first;
    }

    float x = fmod(float(rand()),(r.second - r.first)) + r.first;
    float x2 = fmod(float(rand()),pow(10.,float(decimalLength)));
    x2 = RoundDecimalNPlaces(x2 / pow(10.,float(decimalLength)),decimalLength); 
    return x + x2;
}

vector<float> StdRandGenerator::CycleOne(bool floatgen,int max_size) {
    vector<float> vi;

    bool stat = true;
    float i;

    if (!floatgen) {
        i = float(PRIntInRange(make_pair(pfr.first,pfr.second)));
    } else {
        i = PRFloatInRange(pfr,5);
    }
    sn = i; 
    vi.push_back(i);

    int j = 1; 
    while (stat && j < max_size) {

        if (!floatgen) {
            i = float(PRIntInRange(make_pair(pfr.first,pfr.second)));
        } else {
            i = PRFloatInRange(pfr,5);
        }

        stat = (i != sn);
        if (stat) {
            vi.push_back(i);
        }
        j += 1;
    }
    return vi;
}

///////////////////////////////

int LCG::PRIntInRange(pair<int,int> r) {
    int r2 = DefaultPRIntInRange();
    return ScaleIntRangeToRange(r2, make_pair(0,m),r);
}

float LCG::PRFloatInRange(pair<float,float> r,int decimalLength) {
    float f2 = DefaultPRFloatInRange(decimalLength);
    return ScaleFloatRangeToRange(f2,make_pair(0.,float(m)),r,decimalLength);
}

int LCG::DefaultPRIntInRange() {
    int si2 = si;
    si = (mu * si + a) % m;
    return si2;
}

float LCG::DefaultPRFloatInRange(int decimalLength) {
    // get the integer portion
    int l;
    int di = DefaultPRIntInRange();
    string di2;
    string s = "";

    while (s.size() < decimalLength) {
        di2 = to_string(DefaultPRIntInRange());
        l = s.size() + di2.size();
        if (l > decimalLength) {
            di2 = di2.substr(0,l - decimalLength);
        }

        s = s + di2;
    }

    return RoundDecimalNPlaces(stof(to_string(di) + "." + s),decimalLength);
}

vector<float> LCG::CycleOne(bool floatgen,int max_size) {
    vector<float> vi;
    bool stat = true;
    float i;

    // get the first element
    if (!floatgen) {
        i = float(DefaultPRIntInRange());
    } else {
        i = DefaultPRFloatInRange(5);
    }
    vi.push_back(i);

    six = i;
    int j = 1;
    while (stat && j < max_size) {
        if (!floatgen) {
            i = float(DefaultPRIntInRange());
        } else {
            i = DefaultPRFloatInRange(5);
        }
        
        stat = (i != six);
        if (stat) {
            vi.push_back(i);
        }

        j += 1; 
    }

    return vi;
}

/////////////////////////////////////////////////////////////////////////////

void LCG2H::SetFirst(pair<int,float> vp, bool floatgen) {
    
    if (floatgen) {
        if (ff != numeric_limits<float>::max()) {
            return;
        } 

        ff = vp.second;
    } else {
        if (fi != numeric_limits<int>::infinity()) {
            return;
        } 

        fi = vp.first;
    }
}


int LCG2H::PRIntInRange(pair<int,int> r) {
    int q = DefaultPRIntInRange();
    return ScaleIntRangeToRange(q,make_pair(0,l1->m + l2->m),r);
}

float LCG2H::PRFloatInRange(std::pair<float,float> r,int decimalLength) {
    float q = DefaultPRFloatInRange(decimalLength);
    return ScaleFloatRangeToRange(q,make_pair(float(0),float(l1->m + l2->m)),r,decimalLength);
}

int LCG2H::DefaultPRIntInRange() {

    int q = 0;
    for (int i = 0; i < l; i++) {
        q = q + (l1->DefaultPRIntInRange() * l2->DefaultPRIntInRange());
    }

    int qx = q % (l1->m + l2->m);
    SetFirst(make_pair(qx,-1.0),false);
    return qx;
}

float LCG2H::DefaultPRFloatInRange(int decimalLength) {
    float q = 0;
    for (int i = 0; i < l; i++) {
        q = q + (l1->DefaultPRFloatInRange(decimalLength) * l2->DefaultPRFloatInRange(decimalLength));
    }

    float qx = fmod(q,float(l1->m + l2->m));
    SetFirst(make_pair(-1.0,qx),true);
    return qx;

}

vector<float> LCG2H::CycleOne(bool floatgen,int max_size) {
    vector<float> vi;
    bool stat = true;
    float i;

    // get the first value so that first value `fi` or `ff` is
    // set
    if (!floatgen) {
        i = float(DefaultPRIntInRange());
    } else {
        i = DefaultPRFloatInRange(5);
    }
    vi.push_back(i);

    // continually loop through 
    int j = 1;
    float s = (floatgen) ? ff : float(fi);
    while (stat && j < max_size) {
        if (!floatgen) {
            i = float(DefaultPRIntInRange());
        } else {
            i = DefaultPRFloatInRange(5);
        }

        stat = (i != s);
        if (stat) {
            vi.push_back(i);
        }

        j += 1;
    }
    return vi;
}

/////////////////////////////////////////////////////////////////////////////
void PermLCG::SetRangeData(pair<int,int> rx) {
    assert (rx.second >= rx.first);
    assert (rx.first >= 0);

    rx1 = rx;

    // declare the available elements
    for (int i = rx.first; i <= rx.second; i++) {
        s1.insert(i);
    }

}

void PermLCG::SetRangeData(pair<float,float> rx) {
    assert (rx.second >= rx.first);
    assert (rx.first >= 0.);

    rx2 = rx;

    // declare the available elements
    float i = rx.first;
    while (i <= rx.second) {
        s2.insert(RoundDecimalNPlaces(i,5));
        i += f_spacing;
    }

}


/// usage:
/// determine a range R that starts w/ 0 and ends w/ a positive
/// integer. Continually call this method until `finished_stat`.
int PermLCG::PRIntInRange(pair<int,int> r) {
    if (finished_stat) {
        return -1;
    }

    if (rx1.first != -1) {
        assert ((rx1.first == r.first) && (rx1.second == r.second)); 
    } else {
        SetRangeData(r);
    }

    int j = lcg->PRIntInRange(rx1);
    j = ClosestInt(j);

    if (j == -1) {
        finished_stat = true;
        return j;
    }

    s1.erase(j); 
    return j;

}


/// usage: 
/// *similar to `PRIntInRange` except for a float range R instead*
float PermLCG::PRFloatInRange(pair<float,float> r,int decimalLength) {

    if (finished_stat) {
        return -1.;
    }

    if (rx2.first != -1) {
        assert ((rx2.first == r.first) && (rx2.second == r.second)); 
    } else {
        SetRangeData(r);
    }

    float j = lcg->PRFloatInRange(rx2,5);
    j = ClosestFloat(j);
    if (j == -1.) {
        finished_stat = true;
        return j;
    }

    s2.erase(j); 
    return j;


}

/// calculates the closest integer in set `s1` to `i`. If there are ties,
/// take the lesser integer. 
int PermLCG::ClosestInt(int i) {
    int j = -1;
    int d = INT_MAX;

    for (set<int>::iterator it=s1.begin(); it!=s1.end(); it++) {
        if (abs(*it - i) < d) {
            j = *it;
            d = abs(*it - i);
        } else if (abs(*it - i) == d) {
            if (*it < j) {
                j = *it;
            }
        }
    }

    return j;
}

/// virtually identical to `ClosestInt`.
/// TODO: consider refactoring. 
float PermLCG::ClosestFloat(float i) {

    float j = -1.;
    float d = numeric_limits<float>::max();

    for (set<float>::iterator it=s2.begin(); it!=s2.end(); it++) {
        if (abs(*it - i) < d) {
            j = *it;
            d = abs(*it - i);
        } else if (abs(*it - i) == d) {
            if (*it < j) {
                j = *it;
            }
        }
    }

    return j;
}

/// sub-class of AbstractPRNG has a CycleOne that ignores
/// argument `max_size`. 
///
/// CAUTION: use `floatgen=true` with caution.
vector<float> PermLCG::CycleOne(bool floatgen,int max_size) {

    vector<float> vi;
    float i;
    
    float s = i;    
    float j = 0;
    
    float xs = (floatgen) ? (rx2.second - rx2.first) : (rx1.second - rx1.first);

    while (j <= xs) {    

        if (!floatgen) {
            i = float(PRIntInRange(rx1));
        } else {
            i = PRFloatInRange(rx2,5);
        }

        vi.push_back(i);
        j += 1;
    }

    return vi; 
}

/////////////////////////////////////////////////////////////////////

int ValueIndexStretchGenerator::PRIntInRange(pair<int,int> r) {
    assert(r.first <= r.second);
    int j = DefaultPRIntInRange();
    return (j % (r.second - r.first)) + r.first;
}

float ValueIndexStretchGenerator::PRFloatInRange(pair<float,float> r,int decimalLength) {
    assert(r.first <= r.second);
    float j = DefaultPRFloatInRange(decimalLength);
    return fmod(j,r.second - r.first) + r.first;
}

int ValueIndexStretchGenerator::DefaultPRIntInRange() {
    vector<int> vi;
    vector<int> vi2;

    // iterate through and collect the
    for (int i = 0; i < iv.size();i++) {
        if (f1(iv(i),i)) {
            vi.push_back(iv(i));
        }
        vi2.push_back(iv(i));
    }

    int v = f2(conv_to<ivec>::from(vi));
    vi2.push_back(v);
    iv = conv_to<ivec>::from(vi2); 
    return v;
}

/// DUPLICATECODE:
// virtually identical to the function of <LCG> 
float ValueIndexStretchGenerator::DefaultPRFloatInRange(int decimalLength) {

    int l;
    int di = DefaultPRIntInRange();
    string di2;
    string s = "";
    while (s.size() < decimalLength) {
        di2 = to_string(DefaultPRIntInRange());
        l = s.size() + di2.size();
        if (l > decimalLength) {
            di2 = di2.substr(0,l - decimalLength);
        }

        s = s + di2;
    }

    return RoundDecimalNPlaces(stof(to_string(di) + "." + s),decimalLength);
}


/// string is a space-separated sequence. Some examples are
/// form (1):
/// `stdrg` seed::int
/// 
/// form (2): 
/// `lcg` startInt::int multiplier::int additive::int modulo::int
/// ...
/// ...
/// form (n):
/// `some aprng subclass` `their` `arguments`
AbstractPRNG* APRNGFromString(string s) {

    // split the string
    vector<string> vs = SplitStringToVector(s, " ");
    assert(vs.size() > 0);

    if (vs[0] == "stdrg") {
        assert (vs.size() == 2);
        return new StdRandGenerator(stoi(vs[1]));
    } else if (vs[0] == "lcg") {
        assert(vs.size() == 5);
        return new LCG(stoi(vs[1]),stoi(vs[2]),stoi(vs[3]),stoi(vs[4])); 
    } else if (vs[0] == "lcg2h") {
        assert(vs.size() == 10);
        ivec iv1 = {stoi(vs[1]),stoi(vs[2]),stoi(vs[3]),stoi(vs[4])};
        ivec iv2 = {stoi(vs[5]),stoi(vs[6]),stoi(vs[7]),stoi(vs[8])};
        int l = stoi(vs[9]); 
        return new LCG2H(iv1,iv2,l);
    } else if (vs[0] == "permlcg") {
        assert(vs.size() == 6);
        return new PermLCG(stoi(vs[1]),stoi(vs[2]),stoi(vs[3]),stoi(vs[4]),stof(vs[5]));
    } 
    else {
        assert(false); 
    }

    return nullptr;
}

/// *description of delta scheme on APRNG string args*
/// for each of the values of index 1 through (x - 1), x the length of the string,
/// modify each value v by the procedure:
///     q := output of APRNG in range [0,3] (corresponds to operators +,-,*,/)
///     q2 := output of APRNG in range [0,1000]; the operand 
///     v := v q q2
///     modulate v so it is in the range [MIN_APRNG_VAR,MAX_APRNG_VAR] 
string APRNGStringArgsDeltaScheme1(string astr,AbstractPRNG* aprng) {
    vector<string> vs = SplitStringToVector(astr, " ");
    assert(APRNG_CLASSES.find(vs[0]) != APRNG_CLASSES.end());

    int sz = vs.size();
    int operat;
    int operand;
    float o1;
    for (int i = 1; i < sz; i++) {
        operat = aprng->PRIntInRange(make_pair(0,3));
        operand = aprng->PRIntInRange(make_pair(MIN_APRNG_VAR,MAX_APRNG_VAR));
        o1 = stof(vs[i]);
        if (operat == 0) {
            o1 = o1 + operand;
        } else if (operat == 1) {
            o1 = o1 - operand;
        } else if (operat == 2) {
            o1 = o1 * operand;
        } else {
            o1 = o1 / operand;
        }

        o1 = fmod(o1,MAX_APRNG_VAR - MIN_APRNG_VAR) + MIN_APRNG_VAR;
        vs[i] = to_string(o1);
    }

    string sx = "";
    for (auto vs_: vs) {
        sx += vs_ + " ";
    }

    sx = sx.substr(0,sx.size() - 1);
    return sx;
}

/// CAUTION: 
/// NOTE: type `permlcg` is excluded from this function
string APRNGStringFromAPRNG(AbstractPRNG* aprng) {
    // make the aprng string
    string sx = "";
    
    // get the aprng type:
    // 0 for stdrg,1 for lcg, 2 for lcg2h
    int i1 = aprng->PRIntInRange(make_pair(0,2));
    int l;
    if (i1 == 0) {
        l = 1;
        sx = "stdrg ";
    } else if (i1 == 1) {
        l = 4;
        sx = "lcg ";
    } else if (i1 == 2) {
        l = 9;
        sx = "lcg2h ";
    } else {
        assert(false);
    }

    for (int i = 0; i < l; i++) {
        sx += to_string(aprng->PRIntInRange(make_pair(MIN_APRNG_VAR,MAX_APRNG_VAR))) + " ";
    }
    sx = sx.substr(0,sx.size() - 1);
    return sx;
}

/// CAUTION: some subclasses of <AbstractPRNG> will not work or output
///          values of quality using this method.
vector<float> OutputSequenceByRanges(AbstractPRNG* aprng, vector<pair<float,float>> vp,bool floatgen) {

    vector<float> vf;
    float i;
    for (auto vp_: vp) {

        if (!floatgen) {
            i = float(aprng->PRIntInRange(make_pair(vp_.first,vp_.second)));
        } else {
            i = aprng->PRFloatInRange(vp_,5);
        }

        vf.push_back(i);
    }

    return vf;
}