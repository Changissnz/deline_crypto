#include "keygen.hpp"

void KeyGen::OutCKeyBasic() {
    ckey_genrd.clear();

    int cl = ck.size();
    for (int i = 0; i < cl; i++) {
        ckey_genrd.push_back(OutOneCKeyBasic());
    }
}

/// generates one character key by the following rules:
/// - every instruction has number of commands in the APPROXIMATE range [4,12].
/// - The number of commands may exceed 12 because for every MO command generated,
///   there is a 1/5ths chance that a HOP command is also generated.
/// - every command of the instruction could be one of the following:
///     - MSK,*
///     - MO,*
///     - BG,*
///     - HOP,*
/// - *arbitrary rule*
///  1/5'ths chance there is a BG element
///  4/5th's chance there is a MSK
vector<string> KeyGen::OutOneCKeyBasic() {
    // get the length of the character key
    int r = rg1->PRIntInRange(make_pair(4,12));

    vector<string> s;

    // get the initial commands
    //      - 1/5ths chance that there is a background command
    if (rg1->PRFloatInRange(make_pair(0.,1.), 5) <= 0.2) {
        s.push_back(OutBG());
        r -= 1;
    }
    
    //      - 4/5ths chance that there is a mask
    if (rg1->PRFloatInRange(make_pair(0.,1.), 5) <= 0.8) {
        s.push_back(OutMSK());
        r -= 1;
    }

    // make MO commands for the rest
    for (int i = 0; i < r; i++) {
        s.push_back(OutMO());

        if (rg1->PRFloatInRange(make_pair(0.,1.),5) >= 0.2) {
            s.push_back(OutHOP());
        }
    }

    return s;
}

/// - description
/// generates a string denoting a "mask" command to <DMDTraveller>.
/// 
/// *format of the output string*
///     MSK,mask type,i1_i2_..._iN
///
///  * there are three mask types available, FOX SNAKE HAWK
///    if FOX, then N = 16
///    if SNAKE, then N = 14
///    if HAWK, then N = 6
/// 
///    the numbers in the i1_..._iN sequence correspond to LCG's
///    and all fall in the range of [1,100].
/// 
string KeyGen::OutMSK() {

    int j = rg1->PRIntInRange(make_pair(0,2));
    string s = "MSK,";
    int l;

    if (j == 0) {
        s += "FOX,";
        l = 16;
    } else if (j == 1) {
        s += "SNAKE,";
        l = 14;
    } else {
        s += "HAWK,";
        l = 6;
    }

    for (int i = 0; i < l; i++) {
        s += to_string(rg1->PRIntInRange(make_pair(1,100))) + "_";
    }

    s = s.substr(0,s.size() - 1);
    return s; 
}

/// - description
/// generates a string denoting a "move one" command to <DMDTraveller>.
/// 
/// *format of the output string*
///     MO,number of iterations,index of DelineMC* of DelineMD*, index of Delineate22* of DelineMC*,
///       dir0.dir1...dirN,float1_float2_...floatN,bool::(assign point to closest data point)
///         * the number of iterations is between 1-25. 
///         * N, the length of the direction sequence and 
///           float sequence (that signifies the pace [degree] of the 
///           direction to take) is in the range 2-20 (inclusive).
///         * every floati is in the range 0.01-2 (inclusive).
/// *example*
/// MO,3,2,3,L.R.T.B.B.R,0.5_1_0.75_0.5_1_0.2,1
string KeyGen::OutMO() {

    pair<int,int> pi = OutColumnD22Pair();
    if (erroneous) {
        return "";
    }
    
    int index = pi.first;
    int index2 = pi.second;

    int iterations = rg1->PRIntInRange(make_pair(1,25));
    int length_of_dirpace = rg1->PRIntInRange(make_pair(2,20));

    string mo = "MO,"; 
    mo += to_string(iterations) + ",";
    mo += to_string(index) + ",";
    mo += to_string(index2) + ",";
    
    for (int i = 0; i < length_of_dirpace; i++) {
        int j = rg1->PRIntInRange(make_pair(0,3));
        mo += IntToNavDir(j) + ".";
    }
    mo = mo.substr(0,mo.size() - 1);
    mo += ",";
    for (int i = 0; i < length_of_dirpace; i++) {
        mo += to_string(rg1->PRFloatInRange(make_pair(0.001,2.),5)) + "_";
    }

    mo = mo.substr(0,mo.size() - 1);
    mo += ",";

    // generate the bool 
    mo += to_string(rg1->PRIntInRange(make_pair(0,1)));
    return mo;
}

/// outputs a background modification command of the form 
///
///     *form 1*
///
/// BG,ALTER,number of iterations,index of DelineMC* of DelineMD*, index of Delineate22* of DelineMC*,
///     float1ABSMIN_float2ABSMAX
///
/// *example*
/// BG,ALTER,5,2,4,-0.75_-5.0
///
///     *form 2*
/// BG,JITTER,number of iterations,index of DelineMC* of DelineMD*, index of Delineate22* of DelineMC*,
///    XMAGofFORCE_YMAGofFORCE
///
/// *example*
/// BG,JITTER,5,2,4,100.245_-29.0234
///
/// *note*
/// instance of <KeyGen> does not have information about the bounds of the
/// Delineation22's of concern, so for the values float1ABSMIN_float2ABSMAX 
/// and XMAGofFORCE_YMAGofFORCE, the floats fall in the absolute range
/// [0,128]. 
string KeyGen::OutBG() {

    pair<int,int> pi = OutColumnD22Pair();
    if (erroneous) {
        return "";
    }

    // choose alter or jitter
    string aoj = (rg1->PRIntInRange(make_pair(0,1))) ? "ALTER" : "JITTER"; 
    int iterations = rg1->PRIntInRange(make_pair(1,25));

    string s = "BG,";
    s += aoj + ",";
    s += to_string(iterations) + ",";
    s += to_string(pi.first) + ",";
    s += to_string(pi.second) + ",";

    // case: alter
    if (aoj == "ALTER") {
        float min1 = (rg1->PRFloatInRange(make_pair(0.,128.),5));
        float max1 = (rg1->PRFloatInRange(make_pair(min1,128.),5));
        
        if (!(rg1->PRIntInRange(make_pair(0,1)))) {
            min1 = -1. * min1;
            max1 = -1. * max1;
        }
        
        s += to_string(min1) + "_";
        s += to_string(max1);
    } else { 
    // case: jitter
        float min1 = (rg1->PRFloatInRange(make_pair(0.,128.),5));
        float max1 = (rg1->PRFloatInRange(make_pair(0.,128.),5));

        if (rg1->PRIntInRange(make_pair(0,1))) {
            min1 = -1. * min1;
        }

        if (rg1->PRIntInRange(make_pair(0,1))) {
            max1 = -1. * max1;
        }

        s += to_string(min1) + "_";
        s += to_string(max1);
    }

    return s;
}

///     *form 1*
///
/// HOP,index of DelineMC* of DelineMD*, index of Delineate22* of DelineMC*,APRNGstring
///
/// -- example
/// HOP,2,4,APRNGstring
string KeyGen::OutHOP() {
    string s = "HOP,";

    pair<int,int> pi = OutColumnD22Pair();
    s += to_string(pi.first) + ",";
    s += to_string(pi.second) + ",";

    s += APRNGStringFromAPRNG(rg1);
    return s;
}

pair<int,int> KeyGen::OutColumnD22Pair() {
    int index = rg1->PRIntInRange(make_pair(0,dmddim.first - 1));
    int index2 = rg1->PRIntInRange(make_pair(0,dmddim.second[index] - 1));
    return make_pair(index,index2);
}

/////////////////////// for <RInst>

void KeyGen::OutRKeyBasic() {

}

void KeyGen::OutOneRKeyBasic() {

}

void KeyGen::OutANYTIME() {

}

/// format: 
void KeyGen::OutFOR() {

}

void KeyGen::WriteToFile() {

}