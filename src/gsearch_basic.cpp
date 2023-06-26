#include "gsearch_basic.hpp"
#include <algorithm>
using namespace std;

void BasicSearchStruct::UpdatePathVars(string pi, float di) {
    parentIdn = pi;
    distance = di;
}

string BasicSearchStruct::ToString() {
    return nodeIdn + " |++| " + parentIdn + " |++| " + to_string(distance);
}

string BestPaths::ToString() {
    string s = "";
    for (auto res2: bp) {
        s = s + "best paths for: " + res2.first + "\n";
        s = s + "size: " + to_string(res2.second.size()) + "\n";
        for (auto q: res2.second) {
            s = s + "node id: " + q->nodeIdn + "  parent id: " + q->parentIdn +  " distance: " + to_string(q->distance) + "\n" + "\n";
        }
        s = s + "%&&%&&%&&" + "\n" + "\n";
    }
    return s;
}

map<string,BasicSearchStruct*> BestPaths::ShortestPaths() {
    map<string,BasicSearchStruct*> sp;
    for (auto c: bp) {
        sp[c.first] = c.second[(c.second).size() - 1];
    }
    return sp;
}

float BestPaths::Eccentricity() {
    map<string,BasicSearchStruct*> sp = ShortestPaths();
    vector<float> vf;    

    for (auto c: sp) {
        vf.push_back((c.second)->distance);
    }
    
    vec vf2 = conv_to<vec>::from(vf);
    return vf2.max();
}

BestPaths* BestPaths::BestPathsByNodesetComplement(set<string> restricted) {
    unrn = restricted;
    // iterate through `bp` and check for `BasicSearchStruct` does not have
    // a non-existing `parentIdn` or `nodeIdn`.
    map<string,deque<BasicSearchStruct*>> bp_ = bp;
    for (auto cbp: bp_) {
        int i = 0;
        bool stat = (i < cbp.second.size());
        while (stat) {
            bool b1 = (restricted.find((cbp.second[i])->parentIdn) != restricted.end());
            bool b2 = (restricted.find((cbp.second[i])->nodeIdn) != restricted.end());

            if (b1 || b2) {
                cbp.second.erase(cbp.second.begin() + i);
            } else {
                i += 1;
            }
            stat = (i < cbp.second.size()); 
        }

        bp_[cbp.first] = cbp.second;
    }

    BestPaths* bpn = new BestPaths(head,bp_);
    DeleteUnreachableNodes(bpn);

    unrn.clear();
    return bpn;
}


void BestPaths::DeleteUnreachableNodes(BestPaths* bps) {
    // get list of UnreachableNodes
    set<string> dlist; 
    for (auto c: bps->bp) {
        if (!bps->IsNodeConnected(c.first)) {
            dlist.insert(c.first);
        }
        
    }
    
    for (auto c: dlist) {
        auto it= (bps->bp).find(c);
        (bps->bp).erase(it); 
    } 
}


/*
checks for the starting node equal to the `head` in each path.
*/
bool BestPaths::IsNodeConnected(string ns) {
    deque<GPath> dgp = BackTrace(ns,1);
    for (auto dq: dgp) {
        if (dq.size() > 0) {
            if (dq[0] == head) {
                return true;
            }
        }
    }
    return false;
}

/// TODO: add feature to backtrace 
///       that calculates 
//// TODO: bug in backtrace; redo.
deque<GPath> BestPaths::BackTrace(string ns,int max_parent_size) {
    /// TODO: delete this; buggy
    deque<GPath> dgp;
    deque<GPath> fdgp;

    GPath x;
    x.push_back(ns);
    dgp.push_back(x);

    while (dgp.size() > 0) {
        auto y = dgp.front();
        dgp.pop_front();

        vector<string> rs2 = OrderedParentSet(y.front(),max_parent_size);
        vector<string> rs;
        for (auto rs2_:rs2) {
            if (unrn.find(rs2_) != unrn.end()) {
                continue;
            }
            rs.push_back(rs2_);
        }

        // iterate through `rs` and determine suitable nexts
        for (auto rs_:rs) {
            GPath gp2 = y;
            gp2.push_front(rs_); 

            if (rs_ == head) {
                fdgp.push_back(gp2);
                continue;
            }

            /// NOTE: size limit here.
            if (dgp.size() > DEFAULT_MAX_PATH_SEARCH_SIZE) {
                break;
            }

            dgp.push_back(gp2);
        }
    }
    
    return fdgp;
}

vector<string> BestPaths::OrderedParentSet(string ns,int parent_size) {
    deque<BasicSearchStruct*> dbs = bp[ns];
    reverse(dbs.begin(),dbs.end());

    vector<string> vs;
    for (auto qx: dbs) {
        if (qx->parentIdn == "") {
            vs.push_back(qx->nodeIdn);
            continue;
        }
        vs.push_back(qx->parentIdn);
    }


    if (dbs.size() <= parent_size) {
        return vs;
    }

    vector<string> vs2(vs.begin(),vs.begin() + parent_size);
    return vs2;
}

set<string> BestPaths::ParentSet(string ns) {
    set<string> ps;

    deque<BasicSearchStruct*> dbs = bp[ns];
    for (auto qx: dbs) {
        if (qx->parentIdn == "") {
            ps.insert(qx->nodeIdn);
            continue;
        }

        ps.insert(qx->parentIdn);
    }

    return ps;
}

set<string> BestPaths::NodeSet() {
    set<string> ss;

    for (auto cx: bp) {
        ss.insert(cx.first);
    }

    return ss;
}

string GSynopsis::ToString() {

    string s2 = "r:" + to_string(r) + " d:" + to_string(d) + "\n";

    for (auto ss:c) {
        s2 += "*" + ss + "\n";
    }
    return s2;
}


map<string,float> GInfo::Eccentricities() {

    map<string,float> msf;
    for (auto c: nbp) {
        msf[c.first] = (c.second)->Eccentricity();
    }

    return msf;
}


float GInfo::Radius() {

    return EccentricityVec().min();
}

float GInfo::Diameter() {
    return EccentricityVec().max();    
}

// center is nodeset of the following:
// 
//
//
set<string> GInfo::Center() {
    auto eccs = Eccentricities();
    set<string> nodeset;
    float score = numeric_limits<float>::max();

    for (auto c: eccs) {
        if (c.second < score) {
            nodeset.clear();
            nodeset.insert(c.first);
            score = c.second;
        } else if (c.second == score) {
            nodeset.insert(c.first);
        }
    }

    return nodeset;
}


vec GInfo::EccentricityVec() {
    auto ecs = Eccentricities();
    vector<float> vf;    

    for (auto c: ecs) {
        vf.push_back(c.second);
    }
    
    return conv_to<vec>::from(vf);
}

GSynopsis* GInfo::BasicPathInfo() {
    float r = Radius();
    float d = Diameter();
    set<string> sc = Center();
    return new GSynopsis(r,d,sc);
}

/// TODO: test this. 
GInfo* GInfo::SubGInfo(set<string> nodeset) {

    map<string,BestPaths*> nbp2 = nbp;

    /// TODO: refactor this
    // get complement for nodeset
    set<string> nscom;
    set<string> nds = {};
    for (auto c: nbp) {
        nds.insert(c.first);
    }

    set_difference(nds.begin(),nds.end(),
    nodeset.begin(),nodeset.end(),inserter(nscom, nscom.end()));
    
    for (auto ns: nscom) {
        if (nbp2.find(ns) != nbp2.end()) {
            nbp2.erase(nbp2.find(ns));
        }
    }

    for (auto cm: nbp2) {
        auto cms = (cm.second)->BestPathsByNodesetComplement(nscom);
        nbp2[cm.first] = cms;
    }

    return new GInfo(nbp2);
}

vector<set<string>> GInfo::Components() {
    set<string> ons;
    vector<set<string>> vss;

    for (auto c: nbp) {
        
        int ix = -1;
        set<string> ps = (c.second)->NodeSet();

        for (int i = 0; i < vss.size();i++) {
            
            for (auto c2: ps) {
                if (vss[i].find(c2) != vss[i].end()) {
                    ix = i;
                    break;
                }
            }

            if (ix != -1) {
                break;
            }
        }

        ps.insert(c.first);
        if (ix == -1) {
            vss.push_back(ps);
        } else {
            for (auto c2: ps) {
                vss[ix].insert(c2);
            }
        }
    
    }

    return vss;
}

float GInfo::AverageDistanceToNodeset(set<string> ns,string rn) {
    vector<float> vf;
    assert (nbp.find(rn) != nbp.end());

    BestPaths* rnbp = nbp[rn];
    map<string,BasicSearchStruct*> ssmb = rnbp->ShortestPaths();

    for (auto ns_: ns) {
        if (ssmb.find(ns_) == ssmb.end()) {
            vf.push_back(numeric_limits<float>::max());
        } else {
            vf.push_back(ssmb[ns_][0].distance);
        }
    }

    vec vfv = conv_to<vec>::from(vf);
    
    if (vfv.size() == 0) {
        return numeric_limits<float>::max();
    }

    return float(accu(vfv)) / vfv.size();
}

/// TODO: check this. 
float GInfo::N2NDistance(string n1,string n2) {
    if (nbp.find(n1) == nbp.end() || nbp.find(n2) == nbp.end()) {
        return numeric_limits<float>::max();
    }

    /// CORRECTION: 
    /*
    auto sp = nbp[n1]->ShortestPaths();
    if (sp.find(n2) == sp.end()) {
        return numeric_limits<float>::max();
    }
    return sp[n2]->distance;
    */
    //////

    auto sp = nbp[n1]->ShortestPaths();
    auto sp2 = nbp[n2]->ShortestPaths();
    vec iv = {0,0};
    if (sp.find(n2) == sp.end()) {
        iv(0) = numeric_limits<float>::max();
    } else {
        iv(0) = sp[n2]->distance;
    }

    if (sp2.find(n1) == sp2.end()) {
        iv(1) = numeric_limits<float>::max();
    } else {
        iv(1) = sp2[n1]->distance;
    }

    return iv.min(); 
}


//////////////////////////////////////////////////////

GInfo* GSearch::RunGInfo() {
    map<string,BestPaths*> nbp;
    for (auto ns: utg->nodes) {
        nbp[ns.first] = BFSOnNodeBestMPathsPerNode(ns.first,5);
    }

    return new GInfo(nbp);
}


// in the event of ties, breaker condition is
// first parent w/ min distance
BestPaths* GSearch::BFSOnNodeBestMPathsPerNode(string n,int p) {
    deque<string> q;
    set<string> c;

    q.push_back(n);
    string s;
    map<string,deque<BasicSearchStruct*>> m;
    BasicSearchStruct* bss = new BasicSearchStruct(n,"",0.);
    m[n] = {bss};
    vector<pair<string,string>> vs;
    while (q.size() > 0) {
        s = q.front();
        q.pop_front();

        // get all the neighbors
        vs = (utg->Node(s))->NeighborsStringVec();
        for (auto v: vs) {
            // case: node has not been considered
            if (c.find(v.first) == c.end()) {
                q.push_back(v.first);
            }else {
            // case: node has already been considered
                continue;
            }

            if (m.find(v.first) == m.end()) {
                bss = new BasicSearchStruct(v.first,s,m[s][m[s].size() - 1]->distance + 1.0);
                m[v.first] = {bss};
            } else {
                float qchx = (m[s][m[s].size() - 1])->distance + 1.0;
                bss = new BasicSearchStruct(v.first,s,qchx);                    
                int ix = 0;
                for (int ix = 0; ix < m[v.first].size();ix++) {
                    if (m[v.first][ix]->distance > qchx) {
                        continue;
                    }

                    break;
                }
                
                // tie-breakers
                int ix2 = ix;
                bool stat = false;
                for (int ix2 = ix; ix2 < m[v.first].size();ix2++) {
                    if (m[v.first][ix2]->parentIdn == s) {
                        if (m[v.first][ix2]->distance < bss->distance) {
                            m[v.first][ix2] = bss;
                        }
                        stat = true;
                        break;
                    }
                }


                if (!stat) {
                    auto it = m[v.first].begin();
                    m[v.first].insert(it + ix, bss); 
                }
                   
                while  (m[v.first].size() > p) {
                    m[v.first].pop_back();
                }    
            }
        }

        c.insert(s);
    }

    /// CAUTION: 
    //free(bss);
    return new BestPaths(n,m);
}


map<string,BasicSearchStruct*> GSearch::BFSOnNode(string n) {

   BestPaths* bp =  BFSOnNodeBestMPathsPerNode(n,1);
    return bp->ShortestPaths();
}
