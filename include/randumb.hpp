// random float generator
#ifndef RANDUMB_HPP
#define RANDUMB_HPP

#include "extras.hpp"

static int MIN_APRNG_VAR = 0;
static int MAX_APRNG_VAR = 1023;
static std::set<std::string> APRNG_CLASSES = {"stdrg","lcg","lcg2h","permlcg"};

template<typename C> 
bool BoundsInBounds(std::pair<C,C> r1,std::pair<C,C> r2) {
    assert(r1.second >= r1.first);
    assert(r2.second >= r2.first);

    return (r1.first >= r2.first && r1.first <= r2.second) &&
    (r1.second >= r2.first && r1.second <= r2.second);
}

int ScaleIntRangeToRange(int x, std::pair<int,int> r1,std::pair<int,int> r2);
float ScaleFloatRangeToRange(float x, std::pair<float,float> r1,std::pair<float,float> r2,int decimalLength);

class AbstractPRNG {

public: 

    AbstractPRNG() {

    };

    virtual int PRIntInRange(std::pair<int,int> r)=0;
    virtual float PRFloatInRange(std::pair<float,float> r,int decimalLength)=0;
    virtual std::vector<float> CycleOne(bool floatgen, int max_size = 10000)=0;

};

class StdRandGenerator: public AbstractPRNG {

public:

    float sn; 

    // range used for `CycleOne`.
    std::pair<float,float> pfr;

    StdRandGenerator(int seed_number = RAND_MAX,std::pair<float,float> pfr = 
        std::make_pair(MIN_APRNG_VAR,MAX_APRNG_VAR)) : AbstractPRNG() {//int magic_number = RAND_MAX) {
        sn = seed_number;

        if (sn != RAND_MAX) {
            srand(sn); 
        }
        this->pfr = pfr;
    };

    int PRIntInRange(std::pair<int,int> r);
    float PRFloatInRange(std::pair<float,float> r,int decimalLength);

    std::vector<float> CycleOne(bool floatgen,int max_size);
};

class LCG: public AbstractPRNG  {

public:
    // start number
    float six;
    int si;
    int mu;
    int a;
    int m;

    LCG(int start_int, int multiplier, int additive, int modulus) : AbstractPRNG() {
        si = start_int % modulus;
        six = si;
        mu = multiplier;
        a = additive;
        m = modulus; 
    }

    int PRIntInRange(std::pair<int,int> r= std::make_pair(0,0));
    float PRFloatInRange(std::pair<float,float> r,int decimalLength);

    int DefaultPRIntInRange();
    float DefaultPRFloatInRange(int decimalLength);

    std::vector<float> CycleOne(bool floatgen,int max_size);
};

/// two-headed LCG
class LCG2H: public AbstractPRNG  {

public:

    LCG* l1;
    LCG* l2;
    int l;
    
    // first output of integer
    int fi;
    // first output of float
    float ff;

    LCG2H(ivec r1,ivec r2,int l): AbstractPRNG() {
        assert (r1.size() == 4 && r2.size() == 4);
        l1 = new LCG(r1(0),r1(1),r1(2),r1(3));
        l2 = new LCG(r2(0),r2(1),r2(2),r2(3));
        this->l = l; 
        fi = std::numeric_limits<int>::infinity();
        ff = std::numeric_limits<float>::max();
    }
    void SetFirst(std::pair<int,float> vp, bool floatgen);

    int PRIntInRange(std::pair<int,int> r= std::make_pair(0,0));
    float PRFloatInRange(std::pair<float,float> r,int decimalLength);

    int DefaultPRIntInRange();
    float DefaultPRFloatInRange(int decimalLength);

    std::vector<float> CycleOne(bool floatgen,int max_size);

};

/// permutational lcg, specially designed for integers
/// but can be used to generate floats, although performance
/// has not been measured. 
class PermLCG: public AbstractPRNG {

public:

    LCG* lcg;
    int start_index;
    bool finished_stat;
    std::pair<int,int> rx1;
    std::pair<float,float> rx2;
    std::set<int> s1;
    std::set<float> s2;
    float f_spacing; 

    PermLCG(int start_int, int multiplier, int additive, int modulus,float f_spacing=1.0) : AbstractPRNG() {
        assert(f_spacing > 0.);
        this->lcg = new LCG(start_int,multiplier,additive,modulus);
        start_index = 0;
        finished_stat = false;
        this->f_spacing = f_spacing;

        rx1 = std::make_pair(-1,-1);
        rx2 = std::make_pair(-1.,-1.);
    }

    void SetRangeData(std::pair<int,int> rx);
    void SetRangeData(std::pair<float,float> rx);

    int PRIntInRange(std::pair<int,int> r= std::make_pair(0,0));
    float PRFloatInRange(std::pair<float,float> r,int decimalLength);

    int ClosestInt(int i);
    float ClosestFloat(float i);

    std::vector<float> CycleOne(bool floatgen,int max_size);
};

//////// TODO: test this. 
//////// TODO: add this to "APRNGFromString"
class ValueIndexStretchGenerator: public AbstractPRNG {

public:
    
    // seed vector
    ivec iv;
    // arguments are (value,index)
    std::function<bool (std::pair<int,int>,std::string)> f1;
    // argument is (selected values from `iv`)
    std::function<int (ivec,std::string)> f2;
    std::string cmdf1;
    std::string cmdf2;
    int auto_empty;

    ValueIndexStretchGenerator(ivec iv, std::function<bool (std::pair<int,int>,std::string)> f,
        std::function<int (ivec,std::string)> f2, std::string cmdf1,
        std::string cmdf2,int auto_empty = 0) {
        assert(auto_empty >= 0);
        this->iv = iv;
        this->f1 = f1;
        this->f2 = f2;
        this->cmdf1 = cmdf1;
        this->cmdf2 = cmdf2;
        this->auto_empty = auto_empty;
    }

    static ValueIndexStretchGenerator FromString(std::string s);

    int PRIntInRange(std::pair<int,int> r)=0;
    float PRFloatInRange(std::pair<float,float> r,int decimalLength)=0;
    int DefaultPRIntInRange();
    float DefaultPRFloatInRange(int decimalLength);
    std::vector<float> CycleOne(bool floatgen, int max_size = 10000)=0;

    void Empty();

};

bool RelevantValueFunction1(std::pair<int,int> p,std::string command);
int OutputValueFunction1(ivec iv,std::string command);

/////////////////////////////

AbstractPRNG* APRNGFromString(std::string s);
std::string APRNGStringArgsDeltaScheme1(std::string astr,AbstractPRNG* aprng);
std::string APRNGStringFromAPRNG(AbstractPRNG* aprng);
std::vector<float> OutputSequenceByRanges(AbstractPRNG* aprng, 
    std::vector<std::pair<float,float>> vp,bool floatgen);
#endif