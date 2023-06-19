#include "data_reader.hpp"
using namespace std;
//inp.seekg (0, inp.beg);

string DATA_DIRECTORY = "src/data/";

void TravelToBaseDir() {
    // get cwd as vector of tokens
    char cwd[256];
    char* buff = getcwd(cwd, sizeof(cwd));
    string b(buff);
    vector<string> x = SplitStringToVector(b, "/");

    // iterate through and find base directory
    int q = x.size() -1;
    int r = -1,y = 0,i = q;
    for (i = q; i > -1; i--, y++) {
        if (x[i] == "deline_crypto") {
            r = 0;
            break;
        }
    };

    assert (r != -1);

    // traverse back y directories to be at base directory
    string bts = MakeBackTraversalString(y);
    chdir(bts.c_str());
    
    // traverse to data_directory
    chdir(DATA_DIRECTORY.c_str());
}

string MakeBackTraversalString(int numDirBack) {

  string x = "../";
  string y = "";
  for (int i = 0; i < numDirBack; i++) {
    y = y + x;
  }
  return y;
}

////////////////////////////////////////////// 

char* DataReader::ReadNextChar(int charsize) {
    char * buffer = new char [charsize];
    // read data as a block:
    inp.read (buffer,charsize);
    ReachedEnd();
    return buffer;
}

string DataReader::ReadNextLine() {
    string line;
    getline(inp,line);
    ReachedEnd();
    return line;
}

void DataReader::ReadEntire() {
    vector<string> data2;

    while (!finished) {
        ReadBatch();
        data2.insert(data2.end(), data.begin(), data.end());
    }

    data = data2;
}

void DataReader::ReadBatch() {

    int bs2 = 0;
    string s;
    data.clear();
    while (bs2 < bs && !finished) {
        s = ReadNextLine();
        data.push_back(s);
        bs2 += 1;
    }
}

void DataReader::BatchNConversion() {
    int l = data.size();

    if (l == 0) {
        return;
    }

    rowvec r = conv_to<rowvec>::from(NumStringToArmaVec(data[0],",")); 

    mat m(1,r.n_cols,fill::zeros);
    m.row(0) = r;

    for (int i = 1; i < l; i++) {
        r = conv_to<rowvec>::from(NumStringToArmaVec(data[i],",")); 
        m.insert_rows(i,r);
    }

    ndata = m;
}

void DataReader::ReachedEnd() {
    if (finished) {
        return ;
    }

    if (inp.peek() != EOF) {
        return;
    }

    finished = true; 
    inp.close();
}


mat basic_matrix_read(string fp) {
    DataReader dr = DataReader(fp,2);
    dr.ReadEntire();
    dr.BatchNConversion();
    return dr.ndata;
}

string read_next_node(DataReader* dr) {
    string s;
    string s2;

    while (!dr->finished) {
        s = dr->ReadNextLine();

        if (s == "\n") {
            continue;
        }

        if (s != "$#$") {
            s2 += s + "\n";
        } else {
            dr->ReadNextLine();
            break;
        }
    }

    return s2;
}