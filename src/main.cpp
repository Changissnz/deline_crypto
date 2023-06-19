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

/// TODO: 
/// intermittently check w/ Valgrind. 
int main(int argc, char **argv) {
    return 1;
}