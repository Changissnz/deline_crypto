#include "dcbot.hpp"
using namespace std;

void DCBot::LoadFiles() {

    // load the DBot
    db->LoadFiles();

    // load the reactor
    reactor = new Reactor(db);

    // load the input file
    inputdr = new DataReader(inputf,DCMSG_BATCHSIZE);
    if (load_stat) {
        return;
    }

    fx1.open(outputf.first,ofstream::trunc);
    fx2.open(outputf.second,ofstream::trunc);
    load_stat = true;
}

void DCBot::Code() {
    while (!inputdr->finished) {
        CodeOneBatch();
    }
}


void DCBot::CodeOneBatch() {
    read_stat =inputdr->finished;
    
    if (read_stat) {
        return;
    }
    
    // read the batch
    inputdr->ReadBatch();

    // iterate through the batch and perform encoding
    int sz = inputdr->data.size();

    for (int i = 0; i < sz; i++) {
        CodeOneLine(i);
    }

}

void DCBot::CodeOneLine(int i) {
    string sx = inputdr->data[i];
    string sx2;

    for (int j = 0; j < sx.size(); j++) {
        sx2 = sx.substr(j,1);
        auto outp = db->OneChar(sx2);
        WriteToOutF(outp.first);

        // perform reactions
        reactor->React(db,outp.second);
    }
    fx2 << endl;
}

void DCBot::WriteToOutF(pair<mat,int> pms) {
    vec msg = conv_to<vec>::from(pms.first.as_row());
    string sx = IterableToString(msg,",") + ",";
    
    fx1 << sx << endl;
    fx2 << to_string(pms.second) + ",";
}

void DCBot::Terminate() {
    fx1.close();
    fx2.close();
}