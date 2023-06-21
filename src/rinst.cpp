#include "rinst.hpp"
using namespace std;

void RInst::Parse() {
    Parse_();
}

/// *format of ANYTIME string*
///     ANYTIME::(var c1)
///     COND conditiones::(var c2) W/
///     a1,a2,...,aN::(cond_args)
///     TAKE REACTION AS 
///     reactiones_statement_1
///     reactiones_statement_2
///             ...         
///     reactiones_statement_n::(reaction_args)
///     ENDREACT
///     *(non-literal commentinas) optional below (if character instructions required as targets for reaction)*
///     ON
///     targets_separated_by_comma::(reaction_targets)
///
/// *format of FOR string*
///     FOR::(var c1) character::(var c2)
///     IF COND conditiones::(var c3) W/ 
///     a1,a2,...,aN::(var cond_args)
///     TAKE REACTION AS
///     reactiones_statement_1
///     reactiones_statement_2
///             ...         
///     reactiones_statement_n::(reaction_args)
///     ENDREACT
///     *(non-literal commentinas) optional below (if character instructions required as targets for reaction)*
///     ON
///     targets_separated_by_comma::(reaction_targets)
/// -------------------------------------
/// Capital words are KEYWORD markers for ANYTIME,FOR statements
/// and must always be included.
/// The strings of form `::(var cx)` indicate that the
/// prefix attached to the `::` are the values assigned to
/// `var cx`. They are NOT to be included in the string command.
void RInst::Parse_() {

    assert(inf.size() > 0);

    // parse the first command
    vector<string> vs = SplitStringToVector(inf[0], " ");
    assert(possible_c1.find(vs[0]) != possible_c1.end());
    c1 = vs[0];

    // parse the second command, a condition
    if (c1 == "ANYTIME") {
        assert(vs.size() == 1);
        vs = SplitStringToVector(inf[1]," ");
        assert(vs.size() == 3);
        assert(vs[0] == "COND" && vs[2] == "W/");
        assert(possible_conds.find(vs[1]) != possible_conds.end());
        c2 = vs[1];
    } else {
        assert(vs.size() == 2);
        c2 = vs[1];
        vs = SplitStringToVector(inf[1], " ");
        assert(vs.size() == 4);
        assert((vs[0] == "IF") && (vs[1] == "COND") && (vs[3] == "W/"));
        assert(possible_conds.find(vs[1]) != possible_conds.end());
        c3 = vs[1];
    }

    // assign the arguments to the condition
    cond_args = vs[2];

    // get the reaction
    vs = SplitStringToVector(inf[3]," ");
    assert(vs.size() == 3);
    assert((vs[0] == "TAKE") && (vs[2] == "AS") && (vs[1] == "REACTION"));

        // get the definition of the reaction
    int i = 4;
    while (true) {

        if (inf[i] == "ENDREACT") {
            break;
        }
        
        reaction_args.push_back(inf[i]);
        i += 1; 
    }

    // get the target of the reaction
    i += 1;
    if (i >= vs.size()) {
        return; 
    }

    assert(inf[i] == "ON");
    i += 1;
    reaction_targets = SplitStringToVector(inf[i],",");
}

/// TODO: incomplete
DInstSeq* RInst::React(DMDTraveller* dmdt,DInstSeq* dis, UTGSwapper* utgs,string chrcter) {
    bool stat = false;

    // case: check FOR character condition
    if (c1 == "FOR") {
        stat = (c2 == chrcter);
    }

        // no reaction 
    if (!stat) {
        return dis;
    }

    // check ckonditional
    string cid = (c1 == "FOR") ? c3 : c2;
    bool rc = RCONDITIONAL(dmdt,dis,cid,cond_args);

        // no reaction
    if (!rc) {
        return dis;
    }

    // case: reaction

    DInstSeq* dis2 = dis;

    return dis2;

}

/// TODO: work on reactions.cpp commands next. 
