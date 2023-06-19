//#include "edge_selectors.hpp" 
//#include "fit22.hpp"
/*
#include "deline_mc.hpp"
#include "data_reader.hpp"
//#include "big_ozz_function_mapper.hpp"
//#include "dmd_traveller.hpp"
#include "accessory_functions.hpp"
*/
#include "gmech.hpp"
#include "gsearch_basic.hpp"
#include "ffact.hpp"
#include "utgs.hpp"


#include "dmd_traveller.hpp"
#include "dinst.hpp"
//#include "keygen.cpp"
//#include "modif_patterns.hpp"

#include "rinst.hpp" 

#include "randumb_check.hpp" 
#include "accessory_functions.hpp"
#include "gen_struct.hpp"

#include <iostream> 
#include <vector>
#include <type_traits>
using namespace std;

/// FOR TESTING LEAKS
/// valgrind --leak-check=summary --show-leak-kinds=definite --track-origins=yes --verbose --log-file=valgrind-out.txt ./deline_crypto_exec

/// TODO list
/// generate test data for `DelineMD`
/// work on `DMDTraveller`
/// work on `accessory_functions`

//////////////////////////////////////////////////////////////////////////////////////////////////////


UTGraph UTGraphSwapCase1DOT1() {
    UTGraph utg1 = UTGraph();
    utg1.AddNode(make_pair("n1","n13"));

    utg1.AddEdge(make_pair("n1","n13"),make_pair("n3","n9"));

    utg1.AddEdge(make_pair("n2","n15"),make_pair("n3","n9"));
    utg1.AddEdge(make_pair("n2","n15"),make_pair("n11","n1"));

    utg1.AddEdge(make_pair("n3","n9"),make_pair("n4","n12"));
    utg1.AddEdge(make_pair("n3","n9"),make_pair("n7","n3"));

    utg1.AddEdge(make_pair("n4","n12"),make_pair("n5","n16"));

    utg1.AddEdge(make_pair("n5","n16"),make_pair("n6","n10"));

    utg1.AddEdge(make_pair("n6","n10"),make_pair("n7","n3"));
    utg1.AddEdge(make_pair("n6","n10"),make_pair("n8","n4"));

    utg1.AddEdge(make_pair("n7","n3"),make_pair("n9","n17"));

    utg1.AddEdge(make_pair("n9","n17"),make_pair("n10","n7"));
    utg1.AddEdge(make_pair("n9","n17"),make_pair("n18","n8"));

    utg1.AddEdge(make_pair("n10","n7"),make_pair("n11","n1"));
    utg1.AddEdge(make_pair("n10","n7"),make_pair("n12","n19"));

    utg1.AddEdge(make_pair("n12","n19"),make_pair("n16","n6"));

    utg1.AddEdge(make_pair("n13","n5"),make_pair("n14","n11"));
    utg1.AddEdge(make_pair("n13","n5"),make_pair("n15","n2"));

    utg1.AddEdge(make_pair("n14","n11"),make_pair("n18","n8"));

    utg1.AddEdge(make_pair("n15","n2"),make_pair("n17","n14"));

    utg1.AddEdge(make_pair("n16","n6"),make_pair("n17","n14"));

    utg1.AddEdge(make_pair("n18","n8"),make_pair("n19","n18"));

    return utg1;
}

UTGraph UTGraphSwapCase2() {

    UTGraph utg1 = UTGraph();

    utg1.AddEdge(make_pair("n1","n4"),make_pair("n2","n6"));
    utg1.AddEdge(make_pair("n1","n4"),make_pair("n3","n5"));
    utg1.AddEdge(make_pair("n1","n4"),make_pair("n4","n8"));

    utg1.AddEdge(make_pair("n2","n6"),make_pair("n3","n5"));
    utg1.AddEdge(make_pair("n2","n6"),make_pair("n4","n8"));
    utg1.AddEdge(make_pair("n2","n6"),make_pair("n5","n7"));

    utg1.AddEdge(make_pair("n3","n5"),make_pair("n4","n8"));
    utg1.AddEdge(make_pair("n3","n5"),make_pair("n5","n7"));
    utg1.AddEdge(make_pair("n3","n5"),make_pair("n6","n2"));

    utg1.AddEdge(make_pair("n4","n8"),make_pair("n5","n7"));
    utg1.AddEdge(make_pair("n4","n8"),make_pair("n6","n2"));
    utg1.AddEdge(make_pair("n4","n8"),make_pair("n7","n3"));

    utg1.AddEdge(make_pair("n5","n7"),make_pair("n6","n2"));
    utg1.AddEdge(make_pair("n5","n7"),make_pair("n7","n3"));
    utg1.AddEdge(make_pair("n5","n7"),make_pair("n8","n1"));

    return utg1;
}


UTGraph UTGraphSwapCase3() {

    UTGraph utg1 = UTGraph();

    utg1.AddEdge(make_pair("n1","n6"),make_pair("n2","n8"));
    utg1.AddEdge(make_pair("n2","n8"),make_pair("n3","n7"));
    utg1.AddEdge(make_pair("n3","n7"),make_pair("n4","n5"));
    utg1.AddEdge(make_pair("n4","n5"),make_pair("n5","n4"));
    utg1.AddEdge(make_pair("n5","n4"),make_pair("n6","n2"));
    utg1.AddEdge(make_pair("n6","n2"),make_pair("n7","n3"));
    utg1.AddEdge(make_pair("n7","n3"),make_pair("n8","n1"));

    return utg1;
}

/// OUTLINE
/// write code for classifier first
/// then go on to crypto/data analysis

/// OUTLINE (old)
// TASKS:
/*
- finish up deline and start working on deline_mc, then on deline_md
- start working on instruction parsing
- calculate theoretical constraints
*/

/// make more data files (see morebs)

/// TODO: 
/// intermittently check w/ Valgrind. 
int main(int argc, char **argv) {
    //// testing out `call`
    /*
    string s1 = " ASS ";
    string s2 = " TECH";
    string q = call(test_call_function,s1,s2, 3);
    cout << "Q " << q << endl;
    */
    ////////////////////////////////////// 

    // check that delineation matches with delineation of data 1

    //////////////////////
    
    //////////////////////////////////////////////////////////////
    /*
    mat m = basic_matrix_read("t4.csv");
    Deline22* d = new Deline22(m);
    d->Preprocess();
    d->Delineate("nodup",true);
    d->DelineatedPoints();

    cout << "TARGET LABEL " << d->target_label << endl;
    cout << "TARGET LABEL INDICES " << 
        d->target_indices.size() << endl;

    cout << "SCORE" << endl;
    (d->ds)->Display();

    cout << "EDGES" << endl;
    cout << "LEFT" << endl;
    cout << (d->del)->edgeL << endl;
    cout << "RIGHT" << endl;
    cout << (d->del)->edgeR << endl;
    cout << "TOP" << endl;
    cout << (d->del)->edgeT << endl;
    cout << "BOTTOM" << endl;
    cout << (d->del)->edgeB << endl;

    for (auto x: d->target_indices) {

        int l1 = d->ClassifyPoint((d->data).row(x));
        int l2 = d->ClassifyPoint((d->data).row(x),0);

        if (l1 == -1 && l2 == -1) {
            cout << "I " << x << "POINT " << (d->data).row(x) << endl;
        }

        /*
        cout << "index " << x << " | " << d->ClassifyPoint((d->data).row(x)) << " " << 
        d->ClassifyPoint((d->data).row(x),0) << endl;
        //////
        /////
    }
    */ 
    

    //////////////////////////////////////
    /*
    Deline22MC d = Deline22MC(m);
    cout << d.finished_stat << endl;
    int c = d.DelineateFull("nodup",true);
    cout << "C " << c << endl;
    cout << "NUMBER OF DELINEATIONS " << d.dx22s.size() << endl;
    */
    //////////////////////////////////////

    /*
    Deline22* d = new Deline22(m);
    d->Preprocess();
    d->Delineate("nojag",true);
    d->DelineatedPoints();

    cout << "SCORE" << endl;
    (d->ds)->Display();
    */
    /////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////

    /*
    mat m = basic_matrix_read("t4.csv");
    Deline22* d = new Deline22(m);
    d->Preprocess();
    d->Delineate("nodup",true);

    //d->DelineatedPoints();

    cout << "TARGET LABEL " << d->target_label << endl;
    cout << "TARGET LABEL INDICES " << 
        d->target_indices.size() << endl;
    /*
    cout << "SCORE" << endl;
    (d->ds)->Display();
    */

    /*
    cout << " " << d->target_indices(0) << endl; 
    int q = d->ClassifyPoint((d->data).row(d->target_indices(0)));
    int q2 = d->ClassifyPoint((d->data).row(d->target_indices(0)),0);

    cout << "index " << d->target_indices(0) << " | " << q  << endl;
    //cout << "index " << d->target_indices(0) << " | " << q << " | " << q2 << endl;
    ////

    
    for (auto x: d->target_indices) {
        cout << "index " << x << " | " << d->ClassifyPoint((d->data).row(x)) << " " << 
        d->ClassifyPoint((d->data).row(x),0) << endl;
    }
    */
    /////////////////////////////////////////
    /*
    Deline22MC d = Deline22MC(m);
    cout << d.finished_stat << endl;

    d.DelineateNextLabel("nojag",true);
    cout << d.finished_stat << endl;
    
    d.DelineateNextLabel("nojag",true);
    cout << d.finished_stat << endl;

    d.DelineateNextLabel("nojag",true);
    cout << d.finished_stat << endl;

    d.DelineateNextLabel("nojag",true);
    cout << d.finished_stat << endl;
    */

    /////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////

    /// TODO: make verbose option for dmd

    ////////////////////////////////////////////////////////////////////////////////
    
    /// TODO: transfer this as a test for <randumb>
    /*
    StdRandGenerator* srg = new StdRandGenerator(11);
    
    pair<float,float> rr = make_pair(5.,8.);
    rowvec p = {14.,15.};
    float r0 = 18.;
    float r1 = 24.5;
    bool d0i = true; 
    bool d1i = true;    

    rowvec p2 = DeltaOnPoint(rr,p, r0, r1,srg);
    cout << p2 << endl;

    p2 = DeltaOnPoint(rr,p, r0, r1,srg);
    cout << p2 << endl;
    */
    ////////////////////////////////////////////////////////////////////////////////

    /////// NEXT: test <DMDTraveller>

    //////////////////////////////////////////
    //////////////////////////////////////////
    //////////////////////////////////////////
    ////////////////////////////////////////// 

    //cout << (r3 == {1}) << endl;
    /*
    set<int> r3 = conv_to<set<int>>::from((r1 <= r2));
    for (auto r3_: r3) {
        cout << "* " << r3_ << endl;
    }
    */ 

    //// ALSO: review delineations of previous datasets.
    ////////////////////////////////////////////////////

    //// RIGHT NOW: test out LCG
    /*
    // LCG lc = LCG(1,2,1,17);
    LCG lc = LCG(3,4,1,1000);

    for (int i = 0; i < 20; i++) {
        cout << lc.DefaultPRIntInRange() << endl;
    }
    */

    //////////////////////////////////////////////////
    /// TODO: write graph from and to file. 
    /*
    UTGraph utg1 = UTGraph();

    utg1.AddNode(make_pair("n1","t2"));

    utg1.AddEdge(make_pair("n1","t2"),make_pair("n3","t1"));
    utg1.AddEdge(make_pair("n1","t2"),make_pair("n4","t5"));

    //utg1.Display();

    utg1.WriteToFile("catastr.txt");
    cout << "displaying graph#2" << endl;

    UTGraph utg2 = UTGraph::FromFile("catastr.txt");
    utg2.Display();

    assert(utg2.ToString() == utg1.ToString());
    */
    /////////////////////////////////////////////////////////////
    
    /// TODO: test out UTModule
    
    /*
    GSearch gs = GSearch(&utg1);

    auto res = gs.BFSOnNodeBestMPathsPerNode("n0",5);

    cout << res->ToString();
    GInfo* gi = gs.RunGInfo();

    //set<string> vs = {"n1","n4","n3"};
    set<string> vs = {"n0","n1","n2","n3","n4","n5",
                "n6","n7","n8","n9"};

    auto gi2 = gi->SubGInfo(vs);

        ////
    auto xs = gi->nbp;
    xs.erase(xs.find("n7"));

    cout << "SIZE XS: " << xs.size() << endl; 
    cout << "SIZE GI: " << gi->nbp.size() << endl; 
    */
    //////////////////////////////////////


    /// TODO: this is the area of focus now
    /*
    UTGraph utg = UTGraphSwapCase1DOT1();
    LCG lc = LCG(3,4,1,1000);
    bool record = true;

    UTGraph* utg1;
    utg1 = &utg;
    LCG* lc1 = &lc; 

    ///cout << "UTG1: " << utg1->nodes.size() << endl;
    UTGSwapper utgs = UTGSwapper(utg1,lc1,record);
    utgs.Preprocess();
    utgs.InitialRunTypeRoute();
    
    (utgs.usi)->DisplayAllTokenDistances();
    
    cout << "SWAP COUNT: " << ((utgs.usi)->sh)->swap_count << endl;
    cout << "DISTRACTION SWAPS OF INTEREST: " << ((utgs.usi)->sh)->ioi.size() << endl;
    */
    //////////////////////////////////////////////////

    //char* cx = "sdfsadfsadfsdafsd";

    //DataReader dr = DataReader("yoquistas.txt",20);
    /*
    DataReader dr = DataReader("sk1/cinst_map.txt",20);

    while (!dr.finished) {
        string s = dr.ReadNextLine();
        cout << "S:_" << s << "_" << endl;
    }
    */

    //DBot

    /////////////////////////////////////////////////
    /*
    StdRandGenerator* srg = new StdRandGenerator(9);
    mat m2 = basic_matrix_read("t6.csv"); 

    vector<pair<string,bool>> x1 = {
        make_pair("nojag",true),
        make_pair("nocross",true),        
        make_pair("nojag",false),
        make_pair("nodup",false)};
    
    vector<int> vi = {1,2};
    DelineInstr* di = new DelineInstr(x1,vi);

    DelineMD* dmd = new DelineMD(m2, di, true);
    dmd->DelineateFull();
    
    cout << "DATA SIZES" << endl;
    cout << "TOTAL SIZE " << dmd->data.n_rows << endl; 
    cout << "DISPLAY" << endl;
    //dmd->Display();



    LCG* lcg = new LCG(4,5,6,7);
    DMDTraveller* dt = new DMDTraveller(dmd, lcg);
        /// CHECKING: data sizes of delineations and total data
    */
    ///////

    /*
    auto info = dt->Info();

    for (auto x: info.second) {
        cout << "F1: " << x.first << " F2: " << x.second << endl;
    }
    */

/////////////////////////////// 
    /// TODO: check the index of JX.
    /*
    int four = 4;
    vector<int> vi = {14,5,13,57,4,4,12,87,4,91};

    vi.erase(
    remove_if(
        vi.begin(), 
        vi.end(), 
    [&four](const int& item) { return (four == item); }), 
        vi.end());

    cout << conv_to<ivec>::from(vi) << endl;
    */

    //////////
    
    /*
all_items.erase(
  std::remove_if(
    all_items.begin(), 
    all_items.end(), 
    [&bad_ids](const mystruct& item) { return std::find(bad_ids.begin(), bad_ids.end(), item.id) != bad_ids.end(); }), 
  all_items.end());
    */
    ///////////////////////////////////////////////

        //////
    /// TODO: incomplete
    /// TEST: load an instruction into DInstSeq using DBot

    /*
    string ifp = ;
    string rfp = "";
    string dfp = ;
    vector<char> character_sequence = {"A"};

    DBot* db = new DBot();
    */

        ///////

    /// TODO: 
    /// TEST: KeyGen

    /// TEST: APRNGGauge
    /*
    LCG* lcg = new LCG(11,3,2,13);
    APRNGGauge* ag = new APRNGGauge(lcg, make_pair(0,12),0.5);
    ag->MeasureCycle(true);
    cout << "CYCLE SCORES" << endl;
    cout << ag->cycle_scores << endl;
    cout << lcg->PRIntInRange(make_pair(0,0)) << endl;
    */
    ////////////////////////////////////////////////////////////

    //// TODO: test <SelectiveStretchGenerator>
     //// SelectiveStretchGenerator(ivec iv,std::function<bool (ivec,int)> f,std::function<int (ivec)> f2);
    /*
    LCG* lcg = new LCG(11,3,2,13);

    for (int i = 0; i < 10; i++) {
        cout << lcg->DefaultPRFloatInRange(5) << endl;
    }
    */

    //// TODO: test out <CommandSwap>
    //LCG* lcg = new LCG(17,3,4,130);
        /// TODO: test out RSwap
    /*
    LCG* lcg = new LCG(3,6,12,500);
    PermLCG* plcg = new PermLCG(4,2,3,27);
    UTGraphGen* utgg = new UTGraphGen(STD_NODE_LIST,make_pair(2,STD_NODE_LIST.size() - 10), lcg,plcg);
    utgg->LoadUTGraph();
    UTGSwapper* utgs = new UTGSwapper(utgg->utg,lcg,true);
    */
    /////////
    
    LCG* lcg = new LCG(3,6,12,500);
    //UTGraph utg = UTGraphSwapCase1DOT1();
    UTGraph utg = UTGraphSwapCase3();

    utg.Display();
    UTGSwapper* utgs = new UTGSwapper(&utg,lcg,true);
    utgs->Preprocess();
    cout << "AFTER PREPROCESSING" << endl;

        /// NOTE: printing out the component information.
    
    int sz = (utgs->utcg)->key_index.size();
    cout << "COMPONENT INFO: " << sz << endl;

    for (auto at: (utgs->utcg)->key_index) {
        cout << "\t- " << at.first << endl;
        for (auto at_: at.second) {
            cout << "\t* " << at_ << " "; 
        }
        cout << endl;
    }
    
    utgs->InitialRunTypeRoute();
    cout << "SUMMARIO" << endl;
    ((utgs->usi)->sh)->Summarize(true);

    for (auto n : utg.nodes) {
        cout << "distance: " << (utgs->usi)->BijectiveTokenDistance(n.first) << endl;
    }

    //////
    /*
    utgs->PreloadInitialRunTypeRoute();
    //cout << "AFTER PREPROCESSING: " << endl;
    cout << "SWAP TYPE ROUTE" << endl;
    int i = 0; 
    //for (int i = 0; i < 100; i++) {
    while (!utgs->type_route_stat) {
        utgs->SwapOneTypeRoute();
        cout << "IX: " << i << endl;
        i += 1;
    }
    
    cout << "SUMMARIO" << endl;
    ((utgs->usi)->sh)->Summarize(true);
    */

    /*
    for (auto n : utg.nodes) {
        cout << "NODE: " << n.first << " DISTANCE: " << (utgs->usi)->BijectiveTokenDistance(n.first) << endl;
    }
    */
    
    /// TODO: check for the appropriate number of components

    /// NOTE: GSearch cases
    /*    
    UTGraph utg = UTGraphSwapCase1DOT1();
    //    utg.Display();
    /*
    LCG* lcg = new LCG(3,6,12,500);
    PermLCG* plcg = new PermLCG(4,2,3,27);
    UTGraphGen* utgg = new UTGraphGen(STD_NODE_LIST,make_pair(2,STD_NODE_LIST.size() - 10), lcg,plcg);
    utgg->LoadUTGraph();
    GSearch gs = GSearch(utgg->utg);
    auto res = gs.BFSOnNodeBestMPathsPerNode("-",2);

    for (auto bp_: res->bp) {
        auto xs = res->BackTrace(bp_.first);
        cout << "ROUTES FOR " << bp_.first << ": " << xs.size() << endl;
    }
    //////

    GSearch gs = GSearch(&utg);
    auto res = gs.BFSOnNodeBestMPathsPerNode("n2",2);
    for (auto rs: res->bp) {
        cout << "SRC: " << rs.first << endl;
        for (auto rs2: rs.second) {
            cout << "\t-- " << rs2->ToString() << endl;
        }
    }
    */
}

/////////////////////////// TESTS TO WRITE
/*
test out Delineator::CROps() on different modes;
*/

/*
test out these methods:
- UTGraph* UTGraph::SubgraphByNodeset(std::set<std::string> ns);
- float GInfo::AverageDistanceToNodeset(set<string> ns,string rn); 
- void UTGSwapper::BasicNodeAnalysis(string sx);
*/



/// CHECK: the Delineation. 
