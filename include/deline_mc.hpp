#ifndef DELINEMC_HPP
#define DELINEMC_HPP

#include "deline.hpp"
#include <deque>

class Deline22MC {

public:

    /// Delineator needs to output a map
    /// of indices it delineates in .delineate()

    /// which can be used for sub-delineations. 

    Deline22* d;
    std::vector<Deline22*> dx22s;
    bool finished_stat;
    mat data;
    int idn; // id number for next delineation
    int attempts_per_point; // attempts per point
    int max_depth;
    std::map<std::string,int> fpc; // faulty point counter

    Deline22MC(mat data,int app= 4,int max_depth = 4) {
        this->data = data;
        attempts_per_point = app; 
        this->max_depth = max_depth;
        idn = 0;
        finished_stat = false;
        d = nullptr;
        fpc = std::map<std::string,int>();
    }

    int DelineateFull(std::string dname, bool cw,bool verbose = true);
    void DelineateNextLabel(std::string dname,bool cw,bool verbose = true);
    void PrepareDelineation();

    std::vector<Deline22*> SubDelineateOneFull(Deline22* dx, std::string dmethod,bool cw);
    std::vector<Deline22*> SubDelineateOne(Deline22* dx, std::string dmethod,bool cw);
    std::pair<mat,std::vector<IntKeyIntVectorValue>> ReindexDelineation(Deline22* dx);

    void PerformSubDelineation(Deline22* dx, std::string dmethod,bool cw);
    Deline22* PrepareSubDelineation(mat dat, ivec ti ,int tl,int ddepth);
    void ReviewDelineationTarget(Deline22* dx);
    bool AddFaultyPoint(rowvec p);

    mat SubDataOfD22(Deline22* dx,bool complement);

    mat FilterFaultyFromData(mat dat);
    void AssignChildParentRelations(std::vector<Deline22*> d22s);
    
    //// TODO: 
    std::map<std::string,int> HierarchicalClassification(rowvec p); 
    std::map<std::string,int> BaseClassification(rowvec p);

    void ClearAllData();  
    void Display();

};

#endif


