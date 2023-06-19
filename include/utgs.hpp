#ifndef UTGSWAPPER_HPP
#define UTGSWAPPER_HPP
#include "ffact.hpp"

// RSwap := route swap
// BSwap := best swap
// FSwap := swap that declines the solution
static std::set<std::string> possible_swap_commands = {"RSwap","ObjSwap", "TNSwap"};

/// WARNING: post-order does not have written tests for it.
///          Use the mode w/ caution. 
static std::string type_route_mode = "pre-order";
typedef std::deque<GPath> RouteContainer;

float ComponentToCenterMeanDistance(UTComponentGraph* utcg, GInfo* gi, std::string cid);
float NodeToCenterMeanDistance(UTGraph* utg,GInfo* gi, std::string nid);
std::vector<std::pair<std::string,float>> NodeOrderingByDistanceToCenter(UTGraph* utg, GInfo* gi, bool descending);//,AbstractPRNG* aprng);
std::vector<std::string> GPathToComponentNodes(UTComponentGraph* utcg, GPath gp);
std::vector<std::pair<std::string,std::pair<int,int>>> SortSwapScoresType1(std::vector<std::pair<std::string,std::pair<int,int>>> sv);
std::vector<RouteContainer> MinimalCollidingGPathsEstimate(GInfo* gi, 
    std::vector<std::pair<std::pair<std::string,float>,std::string>> node_order, GPath prohibited_nodes); 
RouteContainer OrderRouteContainerForNodeGivenPrevious(GInfo* gi, std::string src_node, std::string target_node, GPath prohibited_nodes);

RouteContainer OrderRouteContainerForNodeGivenPrevious(RouteContainer rc, GPath prohibited_nodes);
std::vector<std::pair<std::pair<std::string,float>,RouteContainer>> MinimalCollidingGPathsEstimate(UTGraph* utg, UTGraph* utgx,GInfo* gix,GPath prohibited_nodes);
//std::vector<std::pair<std::pair<std::string,float>,RouteContainer>> OrderRoutingSolution(std::vector<std::pair<std::pair<std::string,float>,RouteContainer>> vppr, GPath prohibited_nodes);

std::set<std::string> PairwiseGPathCollision(GPath g1, GPath g2);
std::vector<std::pair<std::pair<std::string,float>,RouteContainer>> ReformatNodeOrdering(std::vector<std::pair<std::string,float>> node_order, std::vector<RouteContainer> vr);


/// container that holds the sequence of swaps used to solve
/// token-swapping problem on UTGraph. 
class SwapHistory {

public:

    int swap_count;
    // contains all swaps conducted + recorded. Each element is 
    // [0] swap;
    // [1] token distances after swap of the tokens sitting on edge before swap.
    std::vector<std::pair<std::pair<std::string,std::string>,std::pair<int,int>>> vp;

    // indices of `vp` of interest, usually swaps that are categorized as distraction swaps.
    std::vector<int> ioi;
    bool record;

    SwapHistory(bool record_swaps) {
        swap_count = 0;
        record = record_swaps;
        vp = {};
        ioi = {};
    }

    void AddSwap(std::string n1, std::string n2,std::pair<int,int> pi,bool of_interest) {
        if (record) {
            ///cout << "\t\t\tpushing vp" << endl;
            vp.push_back(make_pair(make_pair(n1,n2),pi));
            if (of_interest) {
                ///cout << "\t\t\tIOI" << endl;
                ioi.push_back(vp.size() - 1);
            }
        }
        Increment();
        ///cout << "after increment" << endl;
    }

    void Increment() {
        swap_count += 1;
    }

    void Clear() {
        swap_count = 0;
        vp.clear();
        ioi.clear();
    }

    void Summarize(bool full = false) {
        std::cout << "number of swaps: " << vp.size() << std::endl;
        std::cout << "number of interest: " << ioi.size() << std::endl;
        if (full) {
            std::cout << "\t-- swaps" << std::endl;
            for (int i = 0; i < swap_count; i++) {
                auto vp_ = vp[i];
                bool stat = (std::find(ioi.begin(),ioi.end(),i) != ioi.end());
                std::cout << "[" << stat << "] " << vp_.first.first << " " << vp_.first.second << 
                    " | " << vp_.second.first << ":" << vp_.second.second << std::endl;
            }
        }
    }
};


/// class is used to improve solution in route-swapping approach.
/// Class variables reference a `SwapHistory` instance. 
class RouteSwapContainer {

public: 

    std::string cid;
    int ri;
    int ei;
    std::vector<int> gpath_indices;

    RouteSwapContainer(std::string cid, int reference_index) {
        this->cid = cid;
        this->ri = reference_index;
        this->ei = -1;
    }

    void AddGPathIndex(int gpi) {
        gpath_indices.push_back(gpi);
    }

    /// used to improve solution. 
    void Reformat(SwapHistory* sh);
};

/// class used by <UTGSwapper> to conduct swapping;
class UTGSwapInstruction {
public:

    UTGraph* utg;
    // component graph corresponding to `utg`. 
    UTComponentGraph* utcg;
    // path info + others for `utg`.
    GInfo* gi;
    // path info + others for `utcg`.
    GInfo* gicg;
    // used for deciding amongst ties in ranked choices.
    AbstractPRNG* aprng;
    // order of components to be solved
    std::vector<std::pair<std::string,float>> component_order;

    // * below variable is used for approach::GPath-swapping.
    // key is component
    // value is order of nodes to be solved, each w/ a list of GPaths to take.
    std::map<std::string,std::vector<std::pair<std::pair<std::string,float>,RouteContainer>>> kvo;
    GPath previous_nodes;
    SwapHistory* sh;
    RouteSwapContainer* rsc;
    bool record_to_rsc;

    /// used to avoid repeat-swaps 
    std::string pnrs; 


    /// utg := `UTGraph` of interest
    /// utcg := `UTComponentGraph` corresponding to utg
    /// gi := `GInfo` corresponding to utg
    /// aprg := `AbstractPRNG` used to make decisions in the event of ties
    /// record := bool that determines whether instance records swap data to `SwapHistory`  
    /// record_to_rsc := bool that determines whether to record route-swapping information for
    ///                  improving solution. 
    UTGSwapInstruction(UTGraph* utg, UTComponentGraph* utcg,GInfo* gi,AbstractPRNG* aprng,bool record,bool record_to_rsc) {
        this->utg = utg;
        this->utcg = utcg;
        this->gi = gi;
        this->aprng = aprng; 
        this->sh = new SwapHistory(record); 
        this->record_to_rsc = record_to_rsc;
        pnrs = "NULLAXI";
    }

    void CompOrder();
    void AnalyzeNodeRoutesOfComponent(std::string srcid);

    void SwapSolveComponent(std::string srcid);
    std::vector<std::pair<std::string,float>> RankedNodesByTokenDistance(std::set<std::string> component_ids);
    void RouteSwapToComponent(std::string srcid);
    bool RandomSwapOnComponent(std::string srcid);
    std::pair<int,int> ScoreSwap(std::string n1,std::string n2);
    void SwapPath(GPath gp);
    void SwapT(std::string n1,std::string n2);  
    void SwapT(std::string n1,std::string n2,bool b); 
    std::pair<std::pair<std::string,std::string>,std::pair<int,int>> RandomSwapOnNode(std::string nd, bool obj_best,bool no_repeat = false);

    void DisplayComponentSetTokenDistance(std::set<std::string> csi);
    void DisplayAllTokenDistances();
    
    float BijectiveTokenDistance(std::string t);
};

///////////////////////////////////////////

/// contains instances of <UGModule>, <UTComponentGraph>, and <UTGSwapInstruction>
/// that aids in swapping tokens on an undirected graph to solve it.
///
/// Ways of usage:
/// - use <UTGSwapInstruction> instance `usi` in method<InitialRunTypeRoute>.
/// - use a string command in method<CommandSwap>.
class UTGSwapper {

public:

    UTGraph* utg;
    AbstractPRNG* aprng;
    bool record;
    UGModule* ugm;
    UTComponentGraph* utcg;
    std::map<std::string,std::pair<std::string,float>> tokenDistances;
    GInfo* gi;
    // GInfo for component graph 
    GInfo* gicg;    
    UTGSwapInstruction* usi;

    /// TODO: relocate this to a class "SwapCommand"
    /// variables used by method "CommandSwap"
    // string: component order index, index of node in kvo[component], index of swap in GPath (first element of RouteContainer)
    std::pair<int,std::pair<int,int>> type_route_index;
    // used for SwapCommand type route.
    std::vector<std::string> component_remainder; // sorted nodes in order by token distance
    bool component_remainder_stat;
    bool type_route_stat;

    // used for SwapCommand 
    std::string target_node_cs; // target node for command swap

    UTGSwapper(UTGraph* utg, AbstractPRNG* aprng,bool record) {
        this->utg = utg;
        this->aprng = aprng; 
        this->record = record;
        type_route_index = std::make_pair(-1,std::make_pair(0,0));

        utcg = nullptr;
        ugm = nullptr;
        type_route_stat = false;
        target_node_cs = "NULLAXI";
    }

    void Preprocess();
    void ComponentReductionDefault();
    std::map<std::string,RouteContainer> C2CTokenAnalysis(std::string cid);
    RouteContainer BasicTokenAnalysis(std::string sx);

    void ComponentAnalysis();
    void InitialRunTypeRoute();
    void FindBestSolutionTypeRoute();

    /// methods used for "CommandSwap"
    void PreloadInitialRunTypeRoute();
    void SwapOneTypeRoute();
    void InitSwapOneTypeRoute();
    bool SwapOneTypeRouteRemainder();
    void LoadComponentRemainderInfo(std::string cmpnt);
    void SwapOneTypeObj(bool obj_best, AbstractPRNG* aprng,int iterations);
    void SwapOneTypeTargetNodeRoute(AbstractPRNG* aprng,int iterations);

    void CommandSwap(std::string command);

    // permutation run
    void PRunTypeRoute();
    void RewindNSwaps();
};



#endif