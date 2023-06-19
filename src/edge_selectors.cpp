#include "edge_selectors.hpp"
#include "assert.h"
#include <vector>

#include <iostream>
using namespace std;

/// NOTE: assume all matrices passed do not contain duplicate values
bool is_valid_point_data(mat PS) {
    // check for correct number of columns
    return PS.n_cols == 3;
}

/// description
/// for direction 

/// return 
/// index of the next point
int index_of_next_point_in_direction(mat PS,rowvec p, char direction,int startIndex,bool orderedMat) {

    // gather all indices of points with closest non-zero distance along axis
    int axis = DirectionToAxis(direction);

    // get the indices of the closest point in PS
    float f1 = numeric_limits<float>::infinity();
    vector<int> indices;
    mat PS2(PS.n_rows,PS.n_cols,fill::zeros);
    int active_rows = 0;
    float f2;

    for (int i = 0; i < PS.n_rows; i++) {
        f2 = PS(i,axis) - p(axis);
        if (f2 <= 0) {
            continue;
        }
        
        if (f2 < f1) {
            indices.clear();
            indices.push_back(i);
            PS2 = mat(PS.n_rows,PS.n_cols,fill::zeros);
            PS2.row(0) = rowvec(PS.row(i)); 
            active_rows = 1;
            f1 = f2;
        } else if (FloatEquals(f2,f1,5)) {
            indices.push_back(i);
            PS2.row(active_rows) = rowvec(PS.row(i)); 
            active_rows += 1;
        } else {
            if (orderedMat) {
                break; 
            }
        }
    }

    // shed all rows after the active row
    if (active_rows < PS.n_rows && PS.n_rows > 0) {
        PS2.shed_rows(active_rows,PS.n_rows - 1);
    }
    
    if (PS2.n_rows == 0) {
        return -1; 
    }

    int k = tiebreak_set(PS2,direction);
    return indices[k]; 
}

/// description
///
/// return
/// index of the best point in the matrix set of ties
int tiebreak_set(mat PS,char direction) {
    int axis;
    if (direction == 'l' || direction == 'r') {
        axis = 0;
    } else {
        axis = 1;
    }

    vec v = PS.col(axis);
    int j;
    if (direction == 'l' || direction == 'b') {
        j = v.index_min();
    } else {
        j = v.index_max();
    }
    return j; 
}

//////////////////////////////////////////////


/// - description
/// eliminates all points in each of edges `es1` and `es2` that cross
/// over into the other side
pair<ivec,ivec> EliminateContraryPointsIndices(mat es1,mat es2,int axis) {
    assert(is_valid_point_data(es1));
    assert(is_valid_point_data(es2));
    
    ivec one(es1.n_rows, fill::none);
    ivec two(es2.n_rows, fill::none);
    one.at(0) = 0;
    two.at(0) = 0;

    // active indices for one and two 
    int active1 = 0;
    int active2 = 0;

    int i1 = 1;
    int i2 = 1;
    int i3;

    int l1 = es1.n_rows;
    int l2 = es2.n_rows;

    pair<rowvec,rowvec> e1;
    pair<rowvec,rowvec> e2;
    pair<bool,bool> cross; 

    while (i1 < l1 && i2 < l2) {
        e1 = make_pair(es1.row(one(active1)),es1.row(i1));        
        i3 = NextPointInRange(es2,e1,axis,i2);

        // case: no relevant points in `es2`; continue w/ next index
        if (i3 == -1) {
            one(active1 + 1) = i1;
            active1 += 1;
            i1 += 1;
            continue;
        }

        // case: relevant point, add all before that point and after the last
        for (int j = i2; j < i3; j++) {
            two(active2 + 1) = j;
            active2 += 1;
        }

        e2 = make_pair(es2.row(two(active2)),es2.row(i3));
        cross = CorrectRectangleCross(e1,e2,axis);

        // case: one does not cross, add it
        if (cross.first) {
            one(active1 + 1) = i1;
            active1 += 1;
        }
        i1 += 1;

        if (cross.second) {
            two(active2 + 1) = i3;
            active2 += 1;
        }
        i2 = i3 + 1;
    }

    // resize to proper size
    one.resize(active1 + 1);
    two.resize(active2 + 1);

    return make_pair(one,two);
}

int NextPointInRange(mat ep,pair<rowvec,rowvec> edge,int axis,int startIndex) {
    float minumum,maximum;
    vec x;
    if (axis == 0) {
        x = {edge.first(0),edge.second(0)};
    } else {
        x = {edge.first(1),edge.second(1)};
    }

    minumum = x.min();
    maximum = x.max();

    rowvec r;
    for (int i = startIndex; i < ep.n_rows; i++) {
            r = ep.row(i); 
            if (r(axis) >= minumum && r(axis) <= maximum) {
                return i;
            }
    }

    return -1;
}

/// - description
/// determines if the endpoints of line segments `l1` and `l2` need to be removed.
pair<bool,bool> CorrectRectangleCross(pair<rowvec,rowvec> l1,pair<rowvec,rowvec> l2,int axis) {
    pair<pair<bool,bool>,pair<bool,bool>> q = LineSegmentsCrossInRectangle(l1,l2);
    bool p1 = true;
    bool p2 = true;

    if (q.first.second) {
        if (CloserEdge(l1.first,l2.first,l1.second,axis) != 1) {
            p1 = false;
        }
    }

    if (q.second.second) {
        if (CloserEdge(l1.first,l2.first,l2.second,axis) != 2) {
            p2 = false;
        }
    }

    return make_pair(p1,p2);

}

pair<pair<bool,bool>,pair<bool,bool>> LineSegmentsCrossInRectangle(pair<rowvec,rowvec> l1,
    pair<rowvec,rowvec> l2) {

    bool s10 = PointInDiagArea(l1.first,l2);
    bool s11 = PointInDiagArea(l1.second,l2);
    bool s20 = PointInDiagArea(l2.first,l1);
    bool s21 = PointInDiagArea(l2.second,l1);

    return make_pair(make_pair(s10,s11),make_pair(s20,s21));
}

bool PointInDiagArea(rowvec p,pair<rowvec,rowvec> d) {

    vec x = {d.first[0],d.second[0]};
    vec y = {d.first[1],d.second[1]};

    return p[0] >= x.min() && p[0] <= x.max() && 
        p[1] >= y.min() && p[1] <= y.max();
}



/// - description
/// determine the edge to keep the point that crosses
int CloserEdge(rowvec e1,rowvec e2,rowvec p,int axis) {
    int oa = (axis + 1) % 2;

    if (abs(e1[oa] - p[oa]) < abs(e2[oa] - p[oa])) {
        return 1;
    }
    return 2;
}

//////////////////////////////////////////////////// 

/// es1 := 3-col matrix with no duplicate points and sorted by axis
/// es2 := 3-col matrix with no duplicate points and sorted by axis
/// axis := 0 for x-axis, 1 for y-axis
pair<mat,mat> EliminateDuplicatePoints(mat es1,mat es2,int axis) {
    assert(is_valid_point_data(es1));
    assert(is_valid_point_data(es2));
    
    // assign by the opposing axis
    int i1 = 0;
    int i2 = 0;
    int i3,assign;

    rowvec em1 = mean(es1);
    rowvec em2 = mean(es2);

    int el1 = es1.n_rows;
    int el2 = es2.n_rows;
    int oa = (axis + 1) % 2;
    while (i1 < es1.n_rows && i2 < es2.n_rows) {
        i3 = SearchMatForRow(es2,es1.row(i1),i2); 

        if (i3 == -1) {
            i1 += 1;
            continue;
        }

        // assign the duplicate point and remove the duplicate point from other
        assign = AssignDuplicatePoint(em1,em2,el1,el2,oa,es1.row(i1));

        // case: delete from 2
        if (assign == 1) {
            es2.shed_row(i3);
        } else {
            es1.shed_row(i1);
        }

        i2 = i3;
    }

    return make_pair(es1,es2); 

}

int AssignDuplicatePoint(rowvec em1,rowvec em2,int el1,int el2,int axis,rowvec p) {
    float x1 = abs(em1.at(axis) - p.at(axis)) / el1;
    float x2 = abs(em2.at(axis) - p.at(axis)) / el2;

    if (x1 < x2) { return 1;}
    return 2;
}

/////////////////////////////////////////////

ivec JaggedEdges(mat PS,char direction) {
    // make assertion here
    assert(is_valid_point_data(PS));
    
    int r = PS.n_rows;
    vector<int> s;
    s.push_back(0);
    int i = 1;

    while (i < r - 1) {
        rowvec r1 = PS.row(s[s.size() - 1]);
        rowvec r2 = PS.row(i);
        if (!IsJagged(r1,r2,direction)) {
            s.push_back(i);
        }
        i++;
    } 

    if (r - 1 != 0) {
        s.push_back(r - 1);
    }

    ivec q = conv_to<ivec>::from(s);
    return q; 
};

/// r := edge
bool IsJagged(rowvec e1,rowvec e2, char direction) {
    // make assertion here

    if (direction == 'l') {
        return e2.at(0) > e1.at(0);
    } else if (direction == 'r') {
        return e2.at(0) < e1.at(0);
    } else if (direction == 't') {
        return e2.at(1) < e1.at(1);
    } else {
        return e2.at(1) > e1.at(1);
    }
}

/// TODO: test
mat SortPoints(mat PS,int col) {
    colvec c = PS.col(col);
    uvec indices = sort_index(c);
    return SortMatByIndices(PS,indices);
}

mat SortMatByIndices(mat PS,uvec indices) {
    mat PS2(PS.n_rows,PS.n_cols,fill::zeros);
    int r = PS.n_rows;
    assert (r == indices.size());

    for (int i = 0; i < r; i++) {
        PS2.row(i) = PS.row(indices[i]);
    }

    return PS2;
}

int SearchOrderedMatForRowMaxIndex(mat m, rowvec r, int start_index) {
    int end_index = m.n_rows;
    bool stat = false;
    int j = -1;
    for (int i = start_index;i < end_index; i++) {
        if (approx_equal(m.row(i),r,"absdiff",0.002)) {
            if (!stat) {
                stat = !stat;
            }
            j = i;
        } else {
            if (stat) {
                break;
            }
        }
    }
    return j;
}

/// return
/// - the minumum index of row starting from `start_index` equal to `r`. 
int SearchMatForRow(mat m, rowvec r, int start_index) {
    int end_index = m.n_rows;

    for (int i = start_index;i < end_index; i++) {
        if (approx_equal(m.row(i),r,"absdiff",0.002)) {
            return i;
        }
    }
    return -1;
}