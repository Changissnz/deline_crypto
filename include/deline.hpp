#ifndef DELINE_HPP
#define DELINE_HPP

#include "delineator.hpp"
#include <algorithm>

typedef std::pair<int, std::vector<int>> IntKeyIntVectorValue;

struct MapValueComparator
{
    bool operator()(const IntKeyIntVectorValue& left, const IntKeyIntVectorValue& right) const
    {
        return left.second.size() < right.second.size();
    }
};

class Deline22Score {

public:

    pair<int,int> ltr; // delineated target-label correct / total of target-labels 
    pair<int,int> ntr; // delineated non-target-label correct / total of non-target-labels
    std::map<int, std::vector<int>> m;

    Deline22Score(std::map<int, std::vector<int>> ikivm, std::pair<int,int> ltr_,std::pair<int,int> ntr_) {
        m = ikivm;
        ltr = ltr_;
        ntr = ntr_;
    }

    std::set<int> IndicesToSet();
    void Display();
}; 

class DelineHierarchy {

public:
    std::set<std::string> parent_names;
    std::set<std::string> child_names;

    DelineHierarchy() {
        parent_names = std::set<std::string>();
        child_names = std::set<std::string>();
    }

    void Display() {
        std::cout << "* parents" << std::endl;
        for (auto c: parent_names) {
            std::cout << c << ",";
        }
        std::cout << "" << std::endl;

        std::cout << "* children" << std::endl;
        for (auto c: child_names) {
            cout << c << ",";
        }
        cout << "" << endl;
    }
};

class Deline22 {

public:

    std::string dname; 
    DelineHierarchy dh;
    mat data;
    int target_label;
    ivec target_indices;
    Delineator* del;
    //vec delineated_indices; 
    Deline22Score* ds;

    // used for traversal during scoring
    int taxis0;
    int taxis1; 

    // curve direction analysis
    std::pair<std::pair<int,int>,std::pair<float,float>> cda;

    bool finished_stat;
    int ddepth;

    int tindex; // traversal index 

    Deline22(mat d,std::string dname = "Deline22",int ddepth = 1) {
        data = d;
        this->dname = dname;
        dh = DelineHierarchy();
        finished_stat = false;
        this->ddepth = ddepth;
        tindex = 0; 
    }

    void LoadPreprocess(std::map<int, std::vector<int>> m);
    void Preprocess();
    void Delineate(std::string dmethod,bool cw); 
    std::pair<std::pair<int,int>,std::pair<int,int>> SMOp(int axis);
    void PreSMOpOnDirection(char d);
    std::pair<int,int> ScoreModulateCurve(int curve_index);
    std::pair<int,int> ClassifyPointsByInfo(int curve_index, std::pair<int,int> pr);
    bool ClassifyPointByCurvePair(std::pair<int,int> cp,rowvec p);
    int ClassifyPoint(rowvec p,int axis=1);
    void DelineatedPoints();

    void DelineatePoints();
    std::pair<std::pair<int,int>,std::pair<float,float>> CurveDirectionAnalysis(char direction);
    std::pair<int,int> RangeOfPointsInExtremum(char curve_dir,int data_mini, int data_maxi,float xmin,float xmax);
    void OrderPointsByAxis(int axis);

    ///// TODO: test these. 
    bool IsSuperSetOf(Deline22* dx);
    void AddHRelation(Deline22* dx, bool is_parent);

    bool IsParent(std::string name);
    bool IsChild(std::string name);

    void ClearData();
    int IndexOfClosestPoint(rowvec p);
    int IndexOfClosestPointOnEdge(rowvec p,char direction);

    void Display();
};

#endif