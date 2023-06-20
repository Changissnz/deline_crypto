#include "utgs.hpp"
using namespace std;

float ComponentToCenterMeanDistance(UTComponentGraph* utcg, GInfo* gi, string cid) {

    // get all nodes of component
    set<string> cn = utcg->key_index[cid];
    set<string> ctr = gi->Center();
    int c = 0;
    float f = 0.;

    for (auto cr: ctr) {
        for (auto cn_: cn) {
            f += gi->N2NDistance(cr,cn_);
            c += 1;
        }
    }

    if (c == 0) {
        return numeric_limits<float>::max();
    } 
    
    return f / float(c);
}

/// - description
/// averages out the distance of node `nid` to the set of center nodes,
/// calculated by `gi`. 
float NodeToCenterMeanDistance(UTGraph* utg,GInfo* gi, string nid) {
    set<string> sc = gi->Center();
    float s = 0;

    for (auto sc_: sc) {
        s += gi->N2NDistance(nid,sc_);
    }

    if (sc.size() == 0) {
        return numeric_limits<float>::max();
    }

    return s / float(sc.size());

}

/// - description
/// orders nodes by distance to center based on ?`descending`? order. 
///
/// - return:
/// vector, each element is (node,distance to center)
vector<pair<string,float>> NodeOrderingByDistanceToCenter(UTGraph* utg, GInfo* gi, bool descending) {//,AbstractPRNG* aprng) {
    vector<pair<string,float>> vp;

    for (auto n: utg->nodes) {
        float f = NodeToCenterMeanDistance(utg,gi,n.first);
        vp.push_back(make_pair(n.first,f));
    }

    if (descending) {
        sort(vp.begin(),vp.end(), [](pair<string,float> l, pair<string,float> r)
        {   
            /*
            if (l.second == r.second) {
                if (aprng->PRIntInRange(make_pair(0,1))) {
                    return false;
                }
                return true; 
            } 
            */
            return l.second > r.second;       
        });
    } else {
        sort(vp.begin(),vp.end(), [](pair<string,float> l, pair<string,float> r)
        {   
            /*
            if (l.second == r.second) {
                if (aprng->PRIntInRange(make_pair(0,1))) {
                    return false;
                }
                return true; 
            }
            */
            return l.second < r.second;
        });
    }

    return vp;
}

/// - description
/// maps the nodes of the path `gp` to its corresponding components in the component
/// graph `utcg`.
/// 
/// NOTE: unused
vector<string> GPathToComponentNodes(UTComponentGraph* utcg, GPath gp) {
    vector<string> vs;

    for (auto x: gp) {
        vs.push_back(utcg->UTCGRefNodeOfOldNode(x));
    }

    return vs;
}

/// - description
/// function is used to determine the best swap involving a node `x`. The score
/// of the token originally on node `x` is the first value of each element's second
/// value. Sorts elements of `sv` in descending order of priority.
///
/// sort rule:
/// if a = (-1,-1) : a is higher priority
/// if b = (-1,-1) : b is higher priority
/// if a = (-1,1) && b = (1,-1): a is higher priority 
/// else: b is higher priority
vector<pair<string,pair<int,int>>> SortSwapScoresType1(vector<pair<string,pair<int,int>>> sv) {
    sort(sv.begin(),sv.end(), [](pair<string,pair<int,int>> l, pair<string,pair<int,int>> r)
    {
        // best case
            //// LINE:
        /*
        if (l.first == r.first) {
            return false;
        }

        if ((l.second.first == r.second.first) && 
            (l.second.second == r.second.second)) {
            return false;
        }
        */

        /*
        if ((r.second.first < 0) && (r.second.second < 0)) {
            return false;
        }

        if ((l.second.first < 0) && (l.second.second < 0)) {
            return true;
        } 

        if ((l.second.first + l.second.second) < 
            (r.second.first + r.second.second)) {
            return true;
        }

        return false;
        */
        
        if ((l.second.first < 0) && (l.second.second < 0)) {
            return true;
        } 

        if ((r.second.first < 0) && (r.second.second < 0)) {
            return false;
        }

        if (l.second.first < 0) {
            return true;
        }
        
        return false;
    });

    return sv;
}

/// - description:
/// function is to gauge the best route for each token to its solve-node
/// (of the same component) based on minimizing collision with nodes previously
/// routed in the iteration. Assume ordering of nodes in `node_order` is in ascending
/// order by distance to the center of the pertaining component.
/// 
/// arguments:
/// - node_order := each element is <<target node, score>, source node>
/// - rc := map of token to solve a node of component x to its RouteContainer
///
/// return:
/// - a sequence of RouteContainers, each ordered by the estimated best path at index 0.
vector<RouteContainer> MinimalCollidingGPathsEstimate(GInfo* gi, vector<pair<pair<string,float>,string>> node_order, GPath prohibited_nodes) {

    // have the nodes and their possible component routes
    // so for each node, determine the
    vector<RouteContainer> vrc;
    RouteContainer rct; 
    int c = 0;
    for (auto x: node_order) {
        rct = OrderRouteContainerForNodeGivenPrevious(gi,x.second,x.first.first,prohibited_nodes);
        prohibited_nodes.push_back(x.first.first);
        vrc.push_back(rct); 

        c += 1;
    }
    return vrc;
}

/// 
RouteContainer OrderRouteContainerForNodeGivenPrevious(GInfo* gi, string src_node, string target_node, GPath prohibited_nodes) {
    BestPaths* bp = gi->nbp[src_node];
    RouteContainer rc = bp->BackTrace(target_node,DEFAULT_PATH_MAX_PARENT_SIZE);
    return OrderRouteContainerForNodeGivenPrevious(rc,prohibited_nodes);
}

/// TODO:
RouteContainer OrderRouteContainerForNodeGivenPrevious(RouteContainer rc, GPath prohibited_nodes) {

    vector<pair<GPath,int>> vp; 
    for (auto rc_: rc) {
        int s = PairwiseGPathCollision(rc_,prohibited_nodes).size(); 
        vp.push_back(make_pair(rc_,s));
    }

    // sort by least number of collisions
    sort(vp.begin(),vp.end(), [](pair<GPath,int> l, pair<GPath,int> r)
    {
        // for the ties in collisions, sort by shortest path
        if (l.second == r.second) {
            return (l.first.size() < r.first.size());
        }

        return l.second < r.second;       
    });

    deque<GPath> dgp;
    for (auto vp_: vp) {
        dgp.push_back(vp_.first);
    }

    return dgp;
}

/// alternative to the function of the same name (above) but with `node_order` as an unordered set.
///
/// 
/// return: 
///
/// TODO: test 
vector<pair<pair<string,float>,RouteContainer>> MinimalCollidingGPathsEstimate(UTGraph* utg, UTGraph* utgx,GInfo* gix,GPath prohibited_nodes) {

    // start w/ the initial hypothesis for ordering
    vector<pair<string,float>> node_order = NodeOrderingByDistanceToCenter(utgx, gix, false);
    
    // reformat: ((node,node distance),solve-token node)
    // get route container for each node-token pair
    vector<pair<pair<string,float>,string>> no2;
    vector<RouteContainer> vrc;
    RouteContainer rc;
    for (auto n_: node_order) {
        string t = utg->NodeOfToken(n_.first);
        no2.push_back(make_pair(n_,t));
        rc = (gix->nbp[n_.first])->BackTrace(t,DEFAULT_PATH_MAX_PARENT_SIZE);
        vrc.push_back(rc);
    }

    GPath prn;
    vector<int> porder;
    for (int i = 0; i < node_order.size(); i++) {
        porder.push_back(i);
    }

    // iterate through and determine which nodes in component
    // collide with which, permuting the ordering to minimize
    // collisions.
    int i = 0;
    while (i < no2.size()) {
        // get the previous destination nodes in ordering
        prn.clear();
        for (int j = 0; j < i; j++) {
            prn.push_back(no2[porder[j]].first.first);
        }

        // get the collisions w/ the destination nodes for this RouteContainer
        rc = vrc[porder[i]];
        set<string> collisions;
        for (auto rc_: rc) {
            set<string> coll2 = PairwiseGPathCollision(rc_,prn); 
            collisions.insert(coll2.begin(),coll2.end());
        }

        // determine the index of insertion (re-arrangement)
        int k = -1;
        for (int j = 0; j < i; j++) {
            if (collisions.find(no2[porder[j]].first.first) != collisions.end()) {
                k = j;
                break;
            }
        }

        // case: re-order by inserting and deleting the previous
        if (k != -1) {
            int q = porder[i];
            porder.erase(porder.begin() + i);
            porder.insert(porder.begin() + k, q);
        }
        i += 1; 
    }

    // re-order by `porder` 
    vector<pair<string,float>> no3;
    vector<RouteContainer> vrc1;
    int kz = no2.size();
    for (int i = 0; i < kz; i++) {
        no3.push_back(no2[porder[i]].first);
        vrc1.push_back(vrc[porder[i]]);
    }

    // reformat the sol'n
    vector<pair<pair<string,float>,RouteContainer>> sol = ReformatNodeOrdering(no3,vrc1);

    // order each RouteContainer
    for (int i = 0; i < kz; i++) {
        sol[i].second = OrderRouteContainerForNodeGivenPrevious(sol[i].second,prohibited_nodes);
        prohibited_nodes.push_back(sol[i].first.first);
    }

    return sol;
}

/// - description:
/// outputs the set of nodes that both `g1` and `g2` contain. 
///
/// - return:
/// 
set<string> PairwiseGPathCollision(GPath g1, GPath g2) {
    
    // convert the paths into sets
    set<string> s1(g1.begin(), g1.end());
    set<string> s2(g2.begin(), g2.end());
    set<string> s3;

    // do set intersection
    set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(),inserter(s3,s3.end()));
    return s3;
}

/// TODO: test
vector<pair<pair<string,float>,RouteContainer>> ReformatNodeOrdering(vector<pair<string,float>> node_order, vector<RouteContainer> vr) {
    assert (node_order.size() == vr.size());
    vector<pair<pair<string,float>,RouteContainer>> vpr;
    int nos = node_order.size();
    for (int i = 0; i < nos; i++) {
        vpr.push_back(make_pair(node_order[i], vr[i])); 
    }
    return vpr;
}

//////////////////////////////////////////////////////////////

/// - description
/// orders the components based on their distance to mean
void UTGSwapInstruction::CompOrder() {
    
    vector<string> fmk;

    for (auto x: utcg->key_index) {
        fmk.push_back(x.first);
    }

    vector<pair<string,float>> vpsf;

    for (string y: fmk) {
        float z = ComponentToCenterMeanDistance(utcg,gi,y);
        vpsf.push_back(make_pair(y,z));
    }

    // sort in descending order
    sort(vpsf.begin(),vpsf.end(), [](pair<string,float> l, pair<string,float> r)
    {    
        return l.second > r.second;       
    });

    component_order = vpsf;
}

/// - description
/// 
///
/// 
void UTGSwapInstruction::AnalyzeNodeRoutesOfComponent(string srcid) {
    // get the subgraph and the sub-ginfo
    UTGraph* utgx = utg->SubgraphByNodeset(utcg->key_index[srcid]);
    GInfo* gix = gi->SubGInfo(utcg->key_index[srcid]);
    
    // (node,node distance)
    vector<pair<string,float>> node_order = NodeOrderingByDistanceToCenter(utgx, gix, false);
    
    // ((node,node distance),solve-token node)
    vector<pair<pair<string,float>,string>> no2;
    for (auto n_: node_order) {
        string t = utg->NodeOfToken(n_.first);
        no2.push_back(make_pair(n_,t));
    }
    
    // get the minimal colliding paths estimate
    // each element is a RouteContainer with the best path placed at index 0.
    vector<pair<pair<string,float>,RouteContainer>> rcx;
    if (type_route_mode == "pre-order") {
        vector<RouteContainer> vr = MinimalCollidingGPathsEstimate(gi, no2, previous_nodes);
        rcx = ReformatNodeOrdering(node_order,vr);
    } else {
        rcx = MinimalCollidingGPathsEstimate(utg,utgx,gi,previous_nodes);
    }

    kvo[srcid] = rcx;
}

void UTGSwapInstruction::SwapSolveComponent(string srcid) {
    RouteSwapToComponent(srcid);
    set<string> sx = {srcid};
    vector<pair<string,float>> vpsf = RankedNodesByTokenDistance(sx);

    // perform random swaps on nodes in order until all nodes are solved.
    bool stat = true;
    while (stat) {
        // perform random swaps on the first node w/ non-zero token distance
        int i = -1;
        for (int j = 0; j < vpsf.size(); j++) {
            if (BijectiveTokenDistance(vpsf[j].first) > 0) {
                i = j;
                break;
            }
        }

        if (i == -1) {
            stat = !stat;
            continue; 
        }

        while (BijectiveTokenDistance(vpsf[i].first) > 0) {
            string l = utg->NodeOfToken(vpsf[i].first);
            pair<pair<string,string>,pair<int,int>> ps = RandomSwapOnNode(l,true);
            SwapT(ps.first.first,ps.first.second,false);
        }
    }
}

vector<pair<string,float>> UTGSwapInstruction::RankedNodesByTokenDistance(set<string> component_ids) {

    // fetch all nodes from components
    set<string> sn;
    for (auto x: component_ids) {
        sn.insert(utcg->key_index[x].begin(),
            utcg->key_index[x].end());
    }

    // get the node-score pairs
    vector<pair<string,float>> vpsf;
    for (auto x: sn) {
        string l = utg->NodeOfToken(x);
        vpsf.push_back(make_pair(x,BijectiveTokenDistance(x))); 
    }

    sort(vpsf.begin(),vpsf.end(),[](pair<string,float> l, pair<string,float> r)
        {    
            return l.second > r.second;
        });

    return vpsf;
}

/// - description
/// class will always use the first <GPath> in a <RouteContainer>
/// to conduct swapping.
void UTGSwapInstruction::RouteSwapToComponent(string srcid) {
    ///DisplayComponentSetTokenDistance({srcid}); 

    assert(kvo.find(srcid) != kvo.end());
    auto kx = kvo[srcid];

    if (record_to_rsc) {
        rsc = new RouteSwapContainer(srcid,sh->vp.size());
    }

    for (auto kx_: kx) {
        // assume any RouteContainer is non-empty
        SwapPath((kx_.second)[0]);
        previous_nodes.push_back(kx_.first.first);

        if (record_to_rsc) {
            rsc->AddGPathIndex(sh->vp.size());
        }
    }

    /// TODO: delete after testing
    ///DisplayComponentSetTokenDistance({srcid});   
}

/// WARNING: repeatedly calling this method may lead to infinite
///          swap sequences.
/// 
/// method called after `RouteSwapToComponent`; conducts a random swap
/// on an unsolved token so that it is closer to its destination. 
bool UTGSwapInstruction::RandomSwapOnComponent(string srcid) {
    set<string> nx = utcg->key_index[srcid];
    for (auto nd: nx) {        
        // fetch the location of the token `nd`
        string lc = utg->NodeOfToken(nd);

        // case: token does not solve its node. 
        if (lc != nd) {
            pair<pair<string,string>,pair<int,int>> ps = RandomSwapOnNode(lc,true);
            SwapT(ps.first.first,ps.first.second);
            return true;
        }
    }

    return false;
}

/// - description
/// scores every swap
pair<int,int> UTGSwapInstruction::ScoreSwap(string n1,string n2) {
    // get distance of token on node n1 to its target
    float d1 = gi->N2NDistance(n1,(utg->nodes[n1])->iidt);
    float d2 = gi->N2NDistance(n2,(utg->nodes[n2])->iidt);

    float d12 = gi->N2NDistance((utg->nodes[n1])->iidt,n2);
    float d22 = gi->N2NDistance(n1,(utg->nodes[n2])->iidt);

    return make_pair(d12-d1,d22-d2);
}

/// - description
/// swaps every edge in the path `gp`.
void UTGSwapInstruction::SwapPath(GPath gp) {

    for (int i = 0; i < gp.size() - 1; i++) {
        SwapT(gp[i],gp[i+1]);
    }
}

/// - description:
/// swaps tokens on nodes `n1` and `n2`. Marks the swaps as important if
/// `n1` or `n2` is a node that has been involved in a previous swap _destination.   
void UTGSwapInstruction::SwapT(string n1,string n2) {
    // get distances of the tokens on the nodes before
    set<string> pn(previous_nodes.begin(),previous_nodes.end());
    bool important = ((pn.find(n1) != pn.end()) || ((pn.find(n2) != pn.end())));
    
    SwapT(n1,n2,important);
}


void UTGSwapInstruction::SwapT(string n1,string n2,bool b) {
    pair<int,int> pi = ScoreSwap(n1,n2);
    UTNode* utn1 = utg->nodes[n1];
    UTNode* utn2 = utg->nodes[n2];

    string s1 = utn1->iidt;
    string s2 = utn2->iidt;

    utn1->iidt = s2;
    utn2->iidt = s1;

    sh->AddSwap(n1,n2,pi,b);
}

/// calculates a random swap on node based on objective `obj_best`, which considers the best-scoring
/// nodes for swaps if set to true, otherwise the worst-scoring nodes for swaps. 
/// 
/// if no_repeat is set to true, then excludes the node `pnrs` in consideration.
///
/// return:
/// - (node 1, node 2), 
///   (solve-distance of token initial on node 1, solve-distance of token initial on node 1)
pair<pair<string,string>,pair<int,int>> UTGSwapInstruction::RandomSwapOnNode(string nd,bool obj_best,bool no_repeat) {

    // score the possible swaps involving node `nd`
    vector<pair<string,string>> vps = (utg->nodes[nd])->NeighborsStringVec();
    vector<pair<string,pair<int,int>>> vps2;
    
    for (auto vp: vps) {
        if (no_repeat && (vp.second == pnrs)) {
            continue;
        }

        if (vp.first == nd) {
            assert(false);
        }

        pair<int,int> pi = ScoreSwap(nd,vp.first);
        vps2.push_back(make_pair(vp.first,pi));
    }

    // case 
    if (vps2.size() == 0) {
        cout << "ERROR: no possible swap available for lone node" << endl;
        return make_pair(
            make_pair("AWOLIENTHAO;EIWHBNT239O85R98O23HY5R", "8I23U5YRJNWTFEGU;AWENJ"),
            make_pair(10000000,20202020));
    }

    // sort the swaps
    vps2 = SortSwapScoresType1(vps2);

    // case: obj_best is false, reverse the vector
    if (!obj_best) {
        reverse(vps2.begin(),vps2.end());
    }

        // "best" swap
    pair<int,int> bsscore = vps2[0].second;

    // get the index range of the best swap
    int start_index = 0;
    int end_index = 0;

    for (int i = 1; i < vps2.size(); i++) {
        if (vps2[i].second.first == bsscore.first &&\
            vps2[i].second.second == bsscore.second) {
            end_index = i;
        } else {
            break;
        }
    }

    // choose a random index in the range start_index, end_index
    int j = aprng->PRIntInRange(make_pair(start_index,end_index));
    pnrs = nd;
    return make_pair(make_pair(nd,vps2[j].first),vps2[j].second);
}

void UTGSwapInstruction::DisplayComponentSetTokenDistance(set<string> csi) {
    cout << "-- TOKEN DISTANCES" << endl; 
    // collect all nodes of interest
    set<string> noi;
    for (auto x: csi) {
        noi.insert(utcg->key_index[x].begin(),utcg->key_index[x].end());
    }

    // display the node: token distance
    for (auto ni: noi) {
        string l = utg->NodeOfToken(ni); 
        cout << "node " << ni << " location " << l <<  "  token distance " << gi->N2NDistance(l,ni) << endl;
    }
}

void UTGSwapInstruction::DisplayAllTokenDistances() {
    for (auto x: utcg->key_index) {
        cout << "*-* COMPONENT " << x.first << endl;
        DisplayComponentSetTokenDistance({x.first});
    }
}

float UTGSwapInstruction::BijectiveTokenDistance(string t) {
    string n1 = utg->NodeOfToken(t);
    return gi->N2NDistance(t,n1);
}

//////////////////////////////////////////////////////////////

/// the preprocess for <UTGSwapper> instance. Runs a `GSearch`
/// to find the best paths for the original graph, then performs
/// a component reduction.
void UTGSwapper::Preprocess() {
    GSearch gs = GSearch(utg);
    gi = gs.RunGInfo();
    ComponentReductionDefault();
}

/// splits the graph into components based on node distance
/// to "centers".
void UTGSwapper::ComponentReductionDefault() {
    int x = utg->MaxNodeDegree() * DEFAULT_COMPONENT_DEG_RATIO;
    ugm = new UGModule(utg,aprng,x);
    ugm->Preprocess();
    ugm->Split();

    utcg = new UTComponentGraph();
    utcg->ProcessUGModule(ugm);

    GSearch gs = GSearch(utcg->utg);
    gicg = gs.RunGInfo();
}

/// - description:
/// calculates the RouteContainer of components for each solve-token 
/// corresponding to a node in component `cid`. 
map<string,RouteContainer> UTGSwapper::C2CTokenAnalysis(string cid) {

    set<string> ss = utcg->key_index[cid];
    map<string,RouteContainer> vgp;

    for (auto n: ss) {
        vgp[n] = BasicTokenAnalysis(n);
    }

    return vgp;
}

/// analysis of node includes:
/// - the shortest path on the component graph `utcg` that the token
///   on node `sx` takes to be solved. 
/// 
/// CAUTION: does not consider the colored case.
///
/// colored case requires there to be a matching
/// b/t tokens and their satisfying node.
RouteContainer UTGSwapper::BasicTokenAnalysis(string sx) {
    string ds = utg->NodeOfToken(sx);

    string start_comp = utcg->UTCGRefNodeOfOldNode(ds);
    string end_comp = utcg->UTCGRefNodeOfOldNode(sx);

    // get all paths b/t start and end component
    BestPaths* bp = gicg->nbp[start_comp];
    return bp->BackTrace(end_comp);
}

/// BRIEF:
/// method used to token-swap by GPaths of minimal
/// collision to solve each component in a sequence,
/// ordered by distances to the graph center. 
void UTGSwapper::InitialRunTypeRoute() {
    PreloadInitialRunTypeRoute();
    auto cov = usi->component_order;
    for (auto co: cov) {
        cout << "\t\t solving component " << co.first << endl; 
        usi->AnalyzeNodeRoutesOfComponent(co.first);
        usi->SwapSolveComponent(co.first); 
    }
}

/// TODO: test
void UTGSwapper::PreloadInitialRunTypeRoute() {
    /// reset the swap-history
    usi = new UTGSwapInstruction(utg,utcg,gi,aprng,record,false);
    usi->CompOrder();
}

void UTGSwapper::ModeiaReset() {
    if (modeia_reset) {
        PreloadInitialRunTypeRoute();
        type_route_index = make_pair(-1,make_pair(0,0));
        modeia_reset = false;
        type_route_stat = false;
    }
}

/// TODO: test
void UTGSwapper::SwapOneTypeRoute() {
    

    // case: complete
    if (type_route_stat) {
        return;
    }

    // case: no designated routes yet
    if (type_route_index.first == -1) {
        InitSwapOneTypeRoute();
    }

    pair<pair<string,float>,RouteContainer> vpr;
    string cmpnt;

    // case: there is a remainder to the previous component
    if (component_remainder_stat) {
        component_remainder_stat = SwapOneTypeRouteRemainder();
        
        // case: component remainder done, increment 
        if (!component_remainder_stat) {
            cout << "double" << endl;
            type_route_index.first = type_route_index.first + 1;
            type_route_stat = (type_route_index.first == usi->component_order.size());
            if (type_route_stat) {
                return;
            }
            type_route_index.second = make_pair(0,0);
            cmpnt = (usi->component_order[type_route_index.first]).first;
            LoadComponentRemainderInfo(cmpnt);
            usi->AnalyzeNodeRoutesOfComponent(cmpnt);
            return SwapOneTypeRoute();
        }

        return;
    }

    // check if path has been completed
        // get component info
    cmpnt = (usi->component_order[type_route_index.first]).first;
    vpr = usi->kvo[cmpnt][type_route_index.second.first];
    int sz = vpr.second[0].size();
    bool stat = (type_route_index.second.second < (sz - 1));

    // case: path has been completed; increment.
    if (!stat) {

        // add destination node to usi->previous_nodes
        usi->previous_nodes.push_back(vpr.second[0][sz- 1]);

        // check if component routing is finished
        // load another node in component routes
        type_route_index.second.first = type_route_index.second.first + 1;
        type_route_index.second.second = 0;

        if (usi->kvo[cmpnt].size() == type_route_index.second.first) {
            component_remainder_stat = true;
            return SwapOneTypeRoute();
        }

    }

    // fetch the edge and conduct the swap
    vpr = usi->kvo[cmpnt][type_route_index.second.first];
    string n1 = vpr.second[0][type_route_index.second.second];
    string n2 = vpr.second[0][type_route_index.second.second + 1];
    type_route_index.second.second = type_route_index.second.second + 1;
    usi->SwapT(n1,n2);
}

void UTGSwapper::InitSwapOneTypeRoute() {
    type_route_index.first = 0;
    type_route_index.second = make_pair(0,0);
    string cmpnt = (usi->component_order[type_route_index.first]).first;
    usi->AnalyzeNodeRoutesOfComponent(cmpnt);
    LoadComponentRemainderInfo(cmpnt);

}

bool UTGSwapper::SwapOneTypeRouteRemainder() {
    cout << "REMAINDER" << endl;
    // get the first available node that is unsolved.
    if (target_node_cs == "NULLAXI") {
        int i = -1;
        for (int j = 0; j < component_remainder.size(); j++) {
            if (usi->BijectiveTokenDistance(component_remainder[j]) > 0) {
                i = j;
                break;
            }
        }

        if (i == -1) {
            return false;
        }
        target_node_cs = component_remainder[i];
    }

    // fetch the complementary node
    string cn = utg->NodeOfToken(target_node_cs);
    if (cn == target_node_cs) {
        target_node_cs = "NULLAXI";
        return SwapOneTypeRouteRemainder();
    }

    // fetch a swap by `usi` and conduct it
    pair<pair<string,string>,pair<int,int>> ps = usi->RandomSwapOnNode(cn,true);
    usi->SwapT(ps.first.first,ps.first.second);
    return true;
}

void UTGSwapper::LoadComponentRemainderInfo(string cmpnt) {
    set<string> cmps = {cmpnt};
    vector<pair<string,float>> vps = usi->RankedNodesByTokenDistance(cmps);
    component_remainder.clear();
    for (auto vps_:vps) {
        component_remainder.push_back(vps_.first);
    }
    component_remainder_stat = false;
}

/// TODO: test
/// NOTE: inefficiency by sorting nodes by token distances for every call. 
/// OPTIONAL: add code to have `usi` use `aprng` instead of its own.
void UTGSwapper::SwapOneTypeObj(bool obj_best, AbstractPRNG* aprng,int iterations) {
    // set `modeia_reset` to True
    modeia_reset = true;

    // rank the nodes 
    set<string> components = (usi->utcg)->ComponentSet();
    vector<pair<string,float>> vps = usi->RankedNodesByTokenDistance(components);

    for (int i = 0; i < iterations; i++) {
        // select by aprng
        int j = aprng->PRIntInRange(make_pair(0,vps.size() - 1)); 

        // get the complementary node (w/ the token)
        string cn = utg->NodeOfToken(vps[j].first);

        // fetch a swap by `usi` and conduct it
        pair<pair<string,string>,pair<int,int>> ps = usi->RandomSwapOnNode(cn,obj_best);     
        usi->SwapT(ps.first.first,ps.first.second);
    }
}

/// CAUTION: recursive, 
void UTGSwapper::SwapOneTypeTargetNodeRoute(AbstractPRNG* aprng, int iterations) {

    modeia_reset = true;

    if (iterations == 0) {
        return;
    }

    // target node route
    if (target_node_cs == "NULLAXI") {
        set<string> components = (usi->utcg)->ComponentSet();
        vector<pair<string,float>> vps = usi->RankedNodesByTokenDistance(components);
        int j = aprng->PRIntInRange(make_pair(0,vps.size() - 1));
        target_node_cs = vps[j].first;
    }

    //
    int i = -1;
    int rem_iter;
    for (int i = 0; i < iterations; i++) {
        string cn = utg->NodeOfToken(target_node_cs);

        pair<pair<string,string>,pair<int,int>> ps = usi->RandomSwapOnNode(cn,true);
        usi->SwapT(ps.first.first,ps.first.second);

        // reset `target_node_cs` if node has been solved
        if (usi->BijectiveTokenDistance(target_node_cs) == 0) {
            target_node_cs = "NULLAXI";
            rem_iter = iterations - i - 1;
            break;
        }
    }

    // case: more iterations needed
    if (rem_iter == 0) {
        SwapOneTypeTargetNodeRoute(aprng,rem_iter - 1);
    }
}

/// TODO: test
/// *forms for the two commands*
///
///     * form 1 *
/// RSwap, iterations::(int)
///
///     * form 2 *
/// ObjSwap,iterations::(int), aprng string,best::(bool)
///
///     * form 3*
/// TNSwap, iterations::(int), aprng string
void UTGSwapper::CommandSwap(string command) {
    // split string
    vector<string> vs = SplitStringToVector(command,",");
    assert(possible_swap_commands.find(vs[0]) != possible_swap_commands.end());

    int iterations;
    if (vs[0] == "RSwap") {
        assert(vs.size() == 2);
        iterations = stoi(vs[1]);
        while (iterations > 0) {
            SwapOneTypeRoute();
            iterations -= 1; 
        }
        return;
    } 
    
    assert(vs.size() == 3);
    iterations = stoi(vs[1]);
    AbstractPRNG* aprng = APRNGFromString(vs[2]);

    if (vs[0] == "ObjSwap") {
        bool bx = stoi(vs[3]); 
        SwapOneTypeObj(bx,aprng,iterations);
    } else {
        // declare the
        SwapOneTypeTargetNodeRoute(aprng,iterations);
    }
}

