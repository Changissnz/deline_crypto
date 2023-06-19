#ifndef DELINEMD_HPP
#define DELINEMD_HPP

#include "deline_mc.hpp" 

class MDClassification {

public: 

    int axis;
    std::map<int,std::map<string,int>> mp;
    MDClassification(int a) {
        axis = a;
    }

    void AddClassification(int c, std::map<std::string,int> m);
    ///std::pair<int,int> CSummary();
};

class DelineInstr {

public:

    std::vector<std::pair<std::string,bool>> instr; 
    std::vector<int> modulo_hop_cycle;
    int index;
    int index2;
    
    DelineInstr(std::vector<pair<std::string,bool>> instr, std::vector<int> mhc) {
        this->instr = instr;
        this->modulo_hop_cycle = mhc;
        index = 0;
        index2 = 0;
    }
 
    pair<std::string,bool> Next();
};

class DelineMD {

public:

    std::map<int,Deline22MC*> dxs;
    DelineInstr* di;
    mat data;
    int axis; 
    bool clear_data;

    DelineMD(mat data, DelineInstr* di, int axis,bool cd = false) {
        this->data = data;
        this->di = di;
        this->axis = axis;
        dxs = std::map<int,Deline22MC*>();
        clear_data = cd;
    }

    void DelineateFull(); 
    void DelineateAxis(int col);
    void DelineateByModularInstr(Deline22MC* dmc);
    MDClassification* MDClassify(rowvec p);
    mat SubmatOnCol(int c);
    mat Submat(vector<int> indices);
    void Display();
}; 

#endif