#include "dcbot.hpp"
#include <gtest/gtest.h>
using namespace std;


TEST(DCBot__Code__Case1_DemoTest, DCBot__Code__Case1_DemoTestCorrect) {

    string ifp = "keyc1.txt";
    string rfp = "keyr1.txt";
    string difp = "dmdi1.txt";
    string dfp = "t6.csv";

    string utgfp1 = "lcg 10 3 4 21";
    string utgfp2 = "3 6_stdrg 62_6 2 3 50 1.24";

    pair<string,string> utgfp_ = make_pair(utgfp1,utgfp2);
    pair<bool,pair<string,string>> utgfp = make_pair(false,utgfp_);

    string inputf = "message2.txt";
    string outputf = "encmessage1.txt";
    string outputf2 = "encmessage11.txt";

    DCBot* dcb = new DCBot(ifp,rfp,dfp,difp,utgfp,STD_NODE_LIST,inputf,make_pair(outputf,outputf2));
    dcb->Code();
    dcb->Terminate();
}

TEST(DCBot__Code__Case2_DemoTest, DCBot__Code__Case2_DemoTestCorrect) {
    string ifp = "keyc2.txt";
    string rfp = "keyr2.txt";
    string difp = "dmdi1.txt";
    string dfp = "t6.csv";

    string utgfp1 = "lcg 10 3 4 21";
    string utgfp2 = "3 6_stdrg 62_6 2 3 50 1.24";
    pair<string,string> utgfp_ = make_pair(utgfp1,utgfp2);
    pair<bool,pair<string,string>> utgfp = make_pair(false,utgfp_);

    string inputf = "message3.txt";
    string outputf = "encmessage2.txt";
    string outputf2 = "encmessage21.txt";

    DCBot* dcb = new DCBot(ifp,rfp,dfp,difp,utgfp,STD_NODE_LIST,inputf,make_pair(outputf,outputf2));
    dcb->Code();
    dcb->Terminate();
}