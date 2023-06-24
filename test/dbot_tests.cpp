#include "dbot.hpp"
#include <gtest/gtest.h>
using namespace std;

TEST(DBot__LoadFiles__Case1_Test, DBot__LoadFiles__Case1_Test_Correct) {
    /// case: loading a generated graph
    string ifp = "keyc1.txt";
    string rfp = "keyr1.txt";
    string difp = "dmdi1.txt";
    string dfp = "t6.csv";

    string utgfp1 = "lcg 10 3 4 21";
    string utgfp2 = "3 6_stdrg 62_6 2 3 50 1.24";
    pair<string,string> utgfp_ = make_pair(utgfp1,utgfp2);
    pair<bool,pair<string,string>> utgfp = make_pair(false,utgfp_);

    DBot* db = new DBot(ifp,rfp,dfp,difp,utgfp,STD_NODE_LIST);
    db->LoadFiles();
}