#include "extras.hpp"
using namespace std;

float RoundDecimalNPlaces(float x,int r) {
    float d = pow(10.,float(r));
    return round(x * d) / d;
}

bool FloatEquals(float f,float f2,int d) {
    float f3 = abs(f - f2);
    return f3 <= pow(10.,-d);
}


vector<string> SplitStringToVector(string s, const char* delimiter) {
    // convert string to char array
    const char * x2 = s.c_str();
    char x[s.length()];
    strcpy(x, x2);

    vector<string> v;
    char * pch;
    pch = strtok(x, delimiter);
    while (pch != NULL)
    {
        string str(pch);
        v.push_back(str);
        pch = strtok (NULL, delimiter);
    }

    return v;
}


vec NumStringToArmaVec(string s,string delimiter,int d) {

    vector<float> x;
    size_t found; 
    float f; 
    while (s.length() > 0) {
        size_t found = s.find(delimiter);

        // last one
        if (found == -1) {
            f = RoundDecimalNPlaces(stof(s),d);
            x.push_back(f);
            s = ""; 
        } else {
        // get the next one
            f = RoundDecimalNPlaces(stof(s.substr(0,found)),d);
            x.push_back(f);
            s = s.substr(found + 1);
        }

    }
    return conv_to<vec>::from(x); 
}

// row indices
mat IndicesToSubArmaMat(mat m, ivec i) {
    int sz = i.size();
    mat m2(sz,m.n_cols,fill::zeros);
    for (int j = 0; j < sz; j++) {
        m2.row(j) = m.row(i(j)); 
    }
    return m2;
}

// ivec := sorted (in ascending order)
mat IndicesToComplementarySubArmaMat(mat m, ivec i) {
    int sz = m.n_rows - i.size();
    assert (sz >= 0);
    mat m2(sz,m.n_cols,fill::zeros);
    i = sort(i);

    int z = 0;
    int k = 0;
    for (int j = 0; j < m.n_rows; j++) {
        //cout << "IZ " << i(z) << " " << j << " " << k << endl;
        if (z < i.size()) {
            if (i(z) == j) {
                z += 1;
                continue;
            }
        }

        m2.row(k) = m.row(j);
        k += 1; 
    }
    return m2;
}

ivec IndicesToComplementaryIndices(ivec i,int l) {

    vector<int> i2;
    set<int> ix = IntIterToSet(i);
    for (int j = 0; j < l; j++) {
        if (ix.find(j) == ix.end()) {
            i2.push_back(j);
        }
    }

    return conv_to<ivec>::from(i2);
}

/// i := sorted (in ascending order)
/// - return
/// sorted ivec `i` with elements not in `s`
ivec IndicesMinusSet(ivec i, set<int> s) {
    vector<int> i2;

    for (auto c: i) {
        if (s.find(c) == s.end()) {
            i2.push_back(c);
        }
    }

    return conv_to<ivec>::from(i2);
}

map<int, vector<int>> IndexMapOfIntLabelledData(mat data,int labelCol) {
    map<int,vector<int>> x;
    int r = data.n_rows;

    for (int i = 0; i < r; i++) {
        if (x.find(int(data(i,labelCol))) == x.end()) {
            x[int(data(i,labelCol))] = {i}; 
        } else {
            x[int(data(i,labelCol))].push_back(i); 
        }
    }
    return x; 
}

ivec GatherMapValuesTypeVectorInt(map<int, vector<int>> m) {
    vector<int> v = {};

    for (auto c: m) {
        copy(c.second.begin(), c.second.end(), back_inserter(v));
    }

    return conv_to<ivec>::from(v);
}

char ComplementaryDirection(char d) {
    if (d == 'l') {
        return 'r';
    } else if (d == 'r') {
        return 'l';
    } else if (d == 't') {
        return 'b';
    } else if (d == 'b') {
        return 't';
    } else {
        assert(false); 
    }
}

int DirectionToAxis(char d) {
    if (d == 'l' || d == 'r') {
        return 1;
    }
    return 0; 
}

// integer to navigational direction
string IntToNavDir(int i) {
    assert(i >= 0 && i <= 3);
    return STD_NAVDIR[i];
}

int NavDirToInt(char cx) {
    if (cx == 'l') {
        return 0;
    }

    if (cx == 'r') {
        return 1;
    }

    if (cx == 't') {
        return 2;
    }

    if (cx == 'd') {
        return 3;
    }

    assert(false);
    return 4;
}

/*
string test_call_function(string s, string s2, int n) {
    string s3 = "";
    for (int i = 0; i < n; i++) {
        s3 += (s + s2);
    }
    return s3;
}
*/

////

float EuclideanDistance(rowvec p, rowvec p2) {

    float s = 0.;
    int sz = p.size();

    for (int i =0; i < sz; i++) {
        s += powf((abs(p(i) - p2(i))),2.);
    }



    return sqrt(s);
}

int MinPointByEuclideanDistance(mat m, rowvec p) {

  int j = -1;
  float ds = numeric_limits<float>::max();
  float ds2;
  for (int i = 0; i < m.n_rows; i++) {
    ds2 = EuclideanDistance(m.row(i), p);
    if (ds2 < ds) {
      j = i;
      ds = ds2;
    }
  }

  return j;
}

float MeanSequentialEuclideanDistance(mat m) {
    float x = 0;
    for (int i = 0; i < m.n_rows - 1; i++) {
        x += EuclideanDistance(m.row(i),m.row(i + 1));
    }

    if (m.n_rows - 1 == 0) {
        return 0.;
    }

    return RoundDecimalNPlaces(x / (m.n_rows - 1), 5); 
}


/// CAUTION: not yet tested; only real number solutions. 
std::pair<float,float> QuadraticFormula(float a, float b, float c) {
    
    // case: a = 0.
    if (a == 0.) {
        // subcase: b = 0.
        if (b == 0.) {
            assert(c == 0.);
        }
        return make_pair(c / b, c / b); 
    }

    float s2 = sqrt(powf(b,2.) - (4. * a * c));
    cout << "S22: " << s2 << endl; 
    cout << "B^2: " << powf(b,2.) << " | 4AC: " << (4 * a * c) << endl;
    cout << "PRE-OP: " << powf(b,2.) - (4. * a * c) << endl; 
    cout << "OP: " << sqrt(powf(b,2.) - (4. * a * c)) << endl;

    float p1 = (-1 * b + s2) / (2 * a);
    float p2 = (-1 * b - s2) / (2 * a);
    return make_pair(p1,p2);
}

mat IndiceRangeToSubArmaMat(mat m, pair<int,int> irnge) {
    assert (irnge.first >= 0 && irnge.second >= 0);
    assert (irnge.first < m.n_rows && irnge.second < m.n_rows);

    vector<int> iv;

    if (irnge.second > irnge.first) {
        for (int i = irnge.first; i <= irnge.second; i++) {
            iv.push_back(i);
        }
    } else {
        for (int i = irnge.first; i >= irnge.second; i--) {
            iv.push_back(i);
        }
    }

    return IndicesToSubArmaMat(m, conv_to<ivec>::from(iv));
}

mat IndiceRangeToPalindromicSubArmaMat(mat m, pair<int,int> irnge) {

    assert (irnge.first >= 0 && irnge.second >= 0);
    assert (irnge.first < m.n_rows && irnge.second < m.n_rows);

    ivec iv = IndiceRangeToPalindromicIndices(irnge); 
    return IndicesToSubArmaMat(m, iv);
}

ivec IndiceRangeToPalindromicIndices(pair<int,int> irnge) {

    vector<int> iv;

    if (irnge.second > irnge.first) {
        for (int i = irnge.first; i <= irnge.second; i++) {
            iv.push_back(i);
        }

        for (int i = irnge.second - 1; i >= irnge.first; i--) {
            iv.push_back(i); 
        }

    } else {
        for (int i = irnge.first; i >= irnge.second; i--) {
            iv.push_back(i);
        }

        for (int i = irnge.second + 1; i <= irnge.first; i++) {
            iv.push_back(i); 
        }
    }

    return conv_to<ivec>::from(iv);
}