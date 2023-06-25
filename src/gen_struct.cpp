#include "gen_struct.hpp"
using namespace std;

void UGraphGen::Generate() {

    for (int i = 0; i < num_nodes; i++) {
        ConnectTargetNode();
    }
}

/// TODO: buggy 
void UGraphGen::ConnectTargetNode() {
    // choose a node not in excluded
    vector<int> vi;
    for (int i = 0; i < num_nodes; i++) {
        if (excluded.find(i) != excluded.end()) {
            continue;
        }
        vi.push_back(i);
    }
    
    // case: no more
    if (vi.size() == 0) {
        return;
    }

    int j = aprng->PRIntInRange(make_pair(0,vi.size() - 1));
    int tn = vi[j]; 
    excluded.insert(tn);

    // get the connectivity of the node
    colvec cv = mx.col(tn);
    double dx = accu(cv);

    // get the nodes to be considered for new connections;    
    // excluded from new connections are
    //      neighbors of tn, tn
    vi.clear();
    for (int i = 0; i < num_nodes; i++) {
        if (excluded.find(i) != excluded.end()) {
            continue;
        }

        bool stat1 = (mx(tn,i) == 0.);
        double dx2 = accu(mx.col(i));
        bool stat2 = (dx2 < deg_range.second);
        if (stat1 && stat2) {
            vi.push_back(i);
        }
    }

    // get the calibrated `deg_range`
    pair<int,int> dr2 = make_pair(dx, deg_range.second);

    // get the wanted number of additional connections
    int wc = aprng->PRIntInRange(dr2) - dx;
    for (int i = 0; i < wc; i++) {
        if (vi.size() == 0) {
            break;
        }
        int j = aprng->PRIntInRange(make_pair(0,vi.size() - 1));
        int tn2 = vi[j];

        /// NOTE: unnecessary
        assert (tn != tn2);

        mx(tn,tn2) = 1.;
        mx(tn2,tn) = 1.;
        vi.erase(vi.begin() + j);
    }
}

void UTGraphGen::LoadUTGraph() {
    // build the graph matrix
    ugg->Generate();
    // generate the token ordering
    pair<int,int> pix = make_pair(int(0),char_list.size() -1);
    plcg->SetRangeData(pix);
    ivec iv = conv_to<ivec>::from(plcg->CycleOne(false,3));
    
    // use a map to store the int-string pairs
    map<int,string> mis;
    for (int i = 0; i < ugg->num_nodes; i++) {
        mis[i] = char_list[i];
    }

    // build the UTGraph
    utg = new UTGraph();

    // iterate through matrix and assign nodes
    // and edges to `utg`.
    pair<string,string> p1;
    pair<string,string> p2;
    int es3 = 0;
    vector<int> disconn;
    for (int i = 0; i < ugg->mx.n_rows; i++) {
        p1 = make_pair(char_list[i],char_list[iv(i)]);

        // case: disconnected, find one to connect to
        int es2 = accu(ugg->mx.row(i));
        if (es2 == 0) {
            for (int j = 0; j < ugg->mx.n_cols;j++) { 
                if (j == i) {
                    continue;
                }

                int es3 = accu(ugg->mx.row(i));
                if (es3 < ugg->deg_range.second) {
                    p2 = make_pair(char_list[j],char_list[iv(j)]);
                    utg->AddEdge(p1,p2); 
                }
            }
            continue;
        }
    
        for (int j = 0; j < ugg->mx.n_cols;j++) {
            if (ugg->mx(i,j) == 1.) {
                p2 = make_pair(char_list[j],char_list[iv(j)]);
                utg->AddEdge(p1,p2); 
            }
        }

    }

}

