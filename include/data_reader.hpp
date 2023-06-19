#ifndef DATAREADER_HPP
#define DATAREADER_HPP
/*
#include <string>
#include <fstream>
#include <iostream>
#include "assert.h"
#include <unistd.h>
#include <vector>
*/

#include <sys/stat.h>
#include <string>
#include <fstream>
#include <vector>
#include "assert.h"
#include <unistd.h>

#include <string.h>
#include "extras.hpp"
//#include <armadillo>
//using namespace arma;

// std::ifstream/// read proceeds by byte size

// read_n_lines
// read_next_line

void TravelToBaseDir();
std::string MakeBackTraversalString(int numDirBack);

class DataReader {

private:

public:
    std::string fp;
    int bs;
    std::ifstream inp;
    bool finished;
    std::vector<std::string> data;
    mat ndata;

    DataReader(std::string fp_,int batchSize) {
        //assert (fp_.find("/") == -1);
        fp = fp_;
        bs = batchSize;
        TravelToBaseDir();
        
        inp = std::ifstream(fp, std::ifstream::in);
        finished = false;
    }

    char* ReadNextChar(int charSize);
    std::string ReadNextLine();
    void ReadEntire();
    void ReadBatch();
    void BatchNConversion();
    void ReachedEnd();
};

mat basic_matrix_read(std::string fp); 
std::string read_next_node(DataReader* dr);
#endif