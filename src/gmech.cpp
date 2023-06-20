#include "gmech.hpp"
using namespace std;

// makes a partial copy of instance w/o neighbors. 
UTNode* UTNode::PartialCopy() {
    return new UTNode(c,iidt);
}


// deletes one <UTNode*> from `neighbors` based on its identifier `c`.
void UTNode::DeleteNeighbor(string nid) {
    int i = -1;
    for (i = 0; i < neighbors.size();i++) {
        if (neighbors[i]->c == nid) {
            break;
        }
    }

    if (i < neighbors.size()) {
        neighbors.erase(neighbors.begin() + i);
    }
}


bool UTNode::IsNeighbor(string n) {

    for (auto n2: neighbors) {
        if (n2->c == n) {
            return true;
        }
    }
    return false;
}


/// node name
/// iidt (token name identifier)
/// neighbors::(node1.node2,node3.node4,...)
string UTNode::ToString() {

    string sg = "" + c;
    sg += "\n" + iidt + "\n";

    vector<pair<string,string>> nsv = NeighborsStringVec();

    for (auto p: nsv) {
        sg += p.first + ".";
        sg += p.second + ",";
    }

    sg = sg.substr(0,sg.size() - 1);
    return sg;
}

vector<pair<string,string>> UTNode::NeighborsStringVec() {
    vector<pair<string,string>> s;
    for (auto c: neighbors) {
        s.push_back(make_pair(c->c,c->iidt));
    }

    return s;
}

/////////////////////////////////////////////////////////////

void UTGraph::Display() {
    cout << ToString() << endl;

}

string UTGraph::ToString() {
    string s = "";
    for (auto n: nodes) {
        s += (n.second)->ToString() + "\n#$#\n\n";
    }
    return s;
} 

// adds a node using its stringized form `s` to `nodes`.
// For any neighbors listed in `s`, if neighbor node does not
// exist in `nodes`,declares a new node.
void UTGraph::NodeFromString(string s) {

    // split the strings into c,iidt, and neighbors
    vector<string> vs = SplitStringToVector(s, "\n");
    assert (vs.size() == 3);

    AddNode(make_pair(vs[0],vs[1]));
    vector<string> vs2 = SplitStringToVector(vs[2],",");
    vector<string> vs3;
    for (auto vs_: vs2) {
        vs3 = SplitStringToVector(vs_,".");
        AddEdge(make_pair(vs[0],vs[1]),
            make_pair(vs3[0],vs3[1]));
    }
}

/// TODO:

UTGraph UTGraph::FromFile(string fp) {
    DataReader dr = DataReader(fp,50);
    
    UTGraph utg = UTGraph();
    string s;

    while (!dr.finished) {
        s = read_next_node(&dr);
        utg.NodeFromString(s);
    }
    
    return utg;
}

UTGraph UTGraph::FromUTNodeVec(vector<UTNode*> utnv) {
    UTGraph utg = UTGraph();
    for (auto c: utnv) {
        utg.AddNode(c);
    }

    return utg;
}

void UTGraph::AddEdge(pair<string,string> n1, pair<string,string> n2) {
    AddNode(n1);
    AddNode(n2);

    nodes[n1.first]->AddNeighbor(nodes[n2.first]);
    nodes[n2.first]->AddNeighbor(nodes[n1.first]);
}

void UTGraph::WriteToFile(string fp) {
    TravelToBaseDir();

    ofstream fx;
    fx.open(fp,ofstream::trunc);// ofstream::app); 
    

    for (auto c: nodes) {
        fx << (c.second)->ToString() + "\n" + "$#$" + "\n" << endl;
    }

    fx.close(); 
}

/// CAUTION: adds node as new and does not check for 
///         node existence or correct attributes.
bool UTGraph::AddNode(std::pair<std::string,std::string> n) {
    bool stat = (nodes.find(n.first) != nodes.end());
    
    if (stat) {
        assert (nodes[n.first]->iidt == n.second);
        return false;
    }

    UTNode* utn = new UTNode(n.first,n.second);
    nodes[n.first] = utn;
    return true;
}

/// TODO: del_node





/// CAUTION: adds node as is; does not check neighbors
///          and token identifiers. 
void UTGraph::AddNode(UTNode* utn) {
    nodes[utn->c] = utn;
}

UTGraph* UTGraph::SubgraphByNodeset(std::set<std::string> ns) {

    UTGraph* utg2 = new UTGraph();

    // make partial copies for each node
    for (auto s: ns) {
        utg2->nodes[s] = Node(s)->PartialCopy();
    }

    // copy the neighbors
    UTNode* unt;
    for (auto s: ns) {
        unt = Node(s);
        for (auto x: unt->neighbors) {
            if (ns.find(x->c) == ns.end()) {
                continue;
            }
            ((utg2->nodes[s])->neighbors).push_back((utg2->nodes[x->c])); 
        }
    }

    return utg2;

    /// TODO: free the memory??
}


UTNode* UTGraph::Node(string c) {
    if (nodes.find(c) == nodes.end()) {
        return nullptr;
    }
    return nodes[c]; 
}

vector<string> UTGraph::NodesOfToken(string t) {
    vector<string> vs;

    for (auto c: nodes) {
        if (c.second->iidt == t) {
            vs.push_back(c.first);
        }
    }

    return vs;
}

/// used for bijective case of token-to-node placement on graph
string UTGraph::NodeOfToken(string t) {
    vector<string> vs = NodesOfToken(t);
    assert (vs.size() == 1);
    return vs[0];
}

int UTGraph::MaxNodeDegree() {
    int x = -1;
    vector<int> y;
    for (auto c: nodes) {
        y.push_back((c.second)->neighbors.size());
        /*
        cout << "DEG OF " << c.first << " " << (c.second)->neighbors.size() << endl;
        //int y = (c.second)->neighbors.size();
        if ((c.second)->neighbors.size() > x) {
            cout << "YES" << endl;
            x = (c.second)->neighbors.size();
        }

        cout << "AFTER CHECK " << x << endl;
        */
    }

    ivec yz = conv_to<ivec>::from(y);
    return yz.max();
}

/*
float UTGraph::CumulativeTokenDistanceMeasure() {
    vector<string,pair<string,float>> vs; 
    
    for (auto c: nodes) {
        vector<string> vs2 = NodesOfToken(c->first);

    }    
}
*/


