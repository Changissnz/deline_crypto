#ifndef EXTRAS_HPP
#define EXTRAS_HPP

#include "assert.h"
#include <map>
#include <set>
#include <cmath>
#include <string>
#include <vector>
#include <armadillo>
using namespace arma;

static std::vector<std::string> STD_ARITHMETIC_OPS = {"+","-","*","/"};
static std::vector<std::string> STD_NAVDIR = {"l","r","t","d"};

float RoundDecimalNPlaces(float x,int d);
bool FloatEquals(float f,float f2,int d); 
std::vector<std::string> SplitStringToVector(std::string s, const char* delimiter);

template<typename T>
std::string IterableToString(const T& t1,const char* delimiter) {
  std::string sx = "";

  for (auto tx: t1) {
    sx += std::to_string(tx) + delimiter;
  } 
  return sx;
}


template<typename C>  
std::string NumIterToStringType(const C& container, std::string delimiter, int roundingDepth) {
    std::string output = ""; 
    for(auto v: container)
      output += std::to_string(RoundDecimalNPlaces(v,roundingDepth)) + delimiter; 
    
    int l = output.size();
    output = output.substr(0,l - 1);
    return output; 
}

template<typename T>
std::set<T> SetComplement(std::set<T> container, std::set<T> anti) {
    std::set<std::string> nscom;
    set_difference(container.begin(),container.end(),
      anti.begin(),anti.end(),std::inserter(nscom, nscom.end()));
    return nscom;
}

template<typename C>  
std::set<int> IntIterToSet(const C& container) {
    std::set<int> s; 
    for(auto v: container)
      s.insert(int(v));

    return s; 
}

template<typename T>
T Str8MultOnVecs(const T& t1, const T& t2) {
  assert(t1.size() == t2.size());
  std::vector<float> vi;

  for (int i = 0; i < t1.size(); i++) {
    vi.push_back(t1(i) * t2(i));
  }
  return conv_to<T>::from(vi);
}

template<typename T>
T Str8DivOnVecs(const T& t1, const T& t2) {
  assert(t1.size() == t2.size());
  std::vector<float> vi;

  for (int i = 0; i < t1.size(); i++) {
    float f = (t2(i) == 0.) ? 0.: (t1(i) / t2(i));
    vi.push_back(f);
  }
  return conv_to<T>::from(vi);
}

template <typename Map>
bool MapCompare(Map const &m1, Map const &m2) {
    // No predicate needed because there is operator== for pairs already.
    return m1.size() == m2.size()
        && std::equal(m1.begin(), m2.end(),
                      m2.begin());
}

vec NumStringToArmaVec(std::string s,std::string delimiter,int d=5);
mat IndicesToSubArmaMat(mat m, ivec i);
mat IndicesToComplementarySubArmaMat(mat m, ivec i); 
std::map<int, std::vector<int>> IndexMapOfIntLabelledData(mat data,int labelCol);
ivec GatherMapValuesTypeVectorInt(std::map<int, std::vector<int>> m);
char ComplementaryDirection(char d);
int DirectionToAxis(char d);
std::string IntToNavDir(int i);
int NavDirToInt(char cx);
ivec IndicesToComplementaryIndices(ivec i,int l);
ivec IndicesMinusSet(ivec i, std::set<int> s);

template<typename R, typename... Args>
auto call(R(*function)(Args...), Args... args) -> typename std::enable_if<!std::is_same<R, void>::value, R>::type {
    return function(args...);
}

/// std::string test_call_function(std::string s, std::string s2, int n);

float EuclideanDistance(rowvec p, rowvec p2);
int MinPointByEuclideanDistance(mat m, rowvec p);
float MeanSequentialEuclideanDistance(mat m);

/// NOTE: unused, does not work for some cases. 
std::pair<float,float> QuadraticFormula(float a, float b, float c);
mat IndiceRangeToSubArmaMat(mat m, std::pair<int,int> irnge);
mat IndiceRangeToPalindromicSubArmaMat(mat m, std::pair<int,int> irnge);

ivec IndiceRangeToPalindromicIndices(std::pair<int,int> irnge);

/// T is an ordered iterable containing numeric types
template<typename T>
float ArithmeticOp(const T& t1,const T& t2,std::string sx) {
  std::set<std::string> aop(STD_ARITHMETIC_OPS.begin(),STD_ARITHMETIC_OPS.end());
  assert(aop.find(sx) != aop.end());

  if (sx == "+") {
    return t1 + t2;
  }

  if (sx == "-") {
    return t1 - t2; 
  }

  if (sx == "*") {
    return t1 * t2;
  }

  return (t2 == 0) ? 0.: t1 / t2;
}

#endif