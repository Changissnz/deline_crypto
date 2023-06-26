#ifndef DCBOT_HPP
#define DCBOT_HPP

#include "rdbot.hpp"
#include "randumb_check.hpp"
static int DCMSG_BATCHSIZE = 50;

class DCBot {

public:

    DBot* db;
    Reactor* reactor;

    std::string inputf;
    std::pair<std::string,std::string> outputf;
    bool load_stat;
    bool read_stat;
    
    // input reader
    DataReader* inputdr;
    // output writer
    ofstream fx1;
    ofstream fx2;

    DCBot(std::string ifp, std::string rfp,std::string dfp,std::string difp,
        std::pair<bool,std::pair<std::string,std::string>> utgfp,
        std::vector<std::string> character_sequence,
        std::string inputf,std::pair<std::string,std::string> outputf) {

        this->db = new DBot(ifp,rfp,dfp,difp,utgfp,character_sequence);
        this->inputf = inputf;
        this->outputf = outputf;
        load_stat = false;
        read_stat = false;
        LoadFiles();
    }

    void LoadFiles();
    void LoadNewInput(std::string newinputf); 
    void Code();
    void CodeOneBatch();
    void CodeOneLine(int i); 
    void WriteToOutF(std::pair<mat,int> pms);
    void Terminate();
};

#endif