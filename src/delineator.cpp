#include "delineator.hpp"
using namespace std;

void Delineator::SimpleAnalysis() {
    int i1 = data.col(0).index_min(); 
    int i2 = data.col(0).index_max();

    int j1 = data.col(1).index_min(); 
    int j2 = data.col(1).index_max();
    extremum = make_pair(make_pair(i1,i2),make_pair(j1,j2));
    mn = mean(data);
}

void Delineator::CROps() {
    OrderPointsByAxis(1);
    CarveEdge('l');
    CarveEdge('r');
    OrderPointsByAxis(0);
    CarveEdge('t');
    CarveEdge('b');

    RefineEdge(1);
    RefineEdge(0);
}

// sequence of LogFit22's

/*
clockwise: r->b->l->t
counter-clockwise: t->l->b->r
*/
void Delineator::BuildDefaultCurveSequence() {
    // check non-null edges
    /* BAD
    assert (edgeL.n_rows > 0);
    assert (edgeR.n_rows > 0);
    assert (edgeT.n_rows > 0);
    assert (edgeB.n_rows > 0);
    */

    vector<char> order = ClockwiseOrder();

    for (auto a:order) {
        AddEdgeToCurveSequence(a);
    }
}

vector<char> Delineator::ClockwiseOrder() {

    if (cw) {
        return {'r','b','l','t'};
    }

    return {'t','l','b','r'};
}

void Delineator::RebuildDefaultCurveSequence() {

    curves.clear();
    BuildDefaultCurveSequence();
}

void Delineator::AddEdgeToCurveSequence(char direction) {
    mat ex = Edge(direction);
    if (cw) {
        if (direction == 'r' || direction == 'b') {
            ex = reverse(ex);
        }
    } else {
        if (direction == 'l' || direction == 't') {
            ex = reverse(ex);
        }
    }

    int r = ex.n_rows;
    rowvec r1,r2; 
    for (int i = 0; i < r - 1; i++) {
        r1 = {ex(i,0),ex(i,1)};
        r2 = {ex(i+1,0),ex(i+1,1)};
        if (FloatEquals(r1(0),r2(0),5) || FloatEquals(r1(1),r2(1),5)) {
            LineFit* ln = new LineFit(make_pair(r1,r2)); 
            curves.push_back(DCurve(ln,direction));
        } else {
            LogFit22* lf = new LogFit22(make_pair(r1,r2));
            curves.push_back(DCurve(lf,direction));
        }

        
    }
}

mat Delineator::Edge(char direction) {
    if (direction == 'r') {
        return edgeR; 
    } else if (direction == 'b') {
        return edgeB; 
    } else if (direction == 't') {
        return edgeT; 
    } else {
        return edgeL; 
    }
}

void Delineator::AssignEdge(mat mx, char direction) {

    if (direction == 'r') {
        edgeR = mx; 
    } else if (direction == 'b') {
        edgeB = mx;
    } else if (direction == 't') {
        edgeT = mx;  
    } else {
        edgeL = mx; 
    }
}


rowvec Delineator::InitializeCarve(char direction) {

    if (direction == 'l' || direction == 'r') {
        rowvec r = rowvec(data.row(extremum.second.first));
        return {r(0),r(1) - 1.};
    }  else {
        rowvec r = rowvec(data.row(extremum.first.first));
        return {r(0) - 1.,r(1)};
    }
}


void Delineator::CarveEdge(char direction) {
    mat x = (direction == 'l' || direction == 'r') ? mat(data2) : mat(data);
    mat y = mat(0,3,fill::zeros);
    rowvec r= InitializeCarve(direction);
    int index = 0;

    while (index != -1) {
        index = index_of_next_point_in_direction(x,r,direction,index,true);
        if (index == -1) {
            continue;
        }
        y.insert_rows(y.n_rows,rowvec(x.row(index)));
        r = rowvec(x.row(index));
        x.shed_row(index);
    }

    if (direction == 'l') {
        edgeL = y;
    } else if (direction == 'r') {
        edgeR = y;
    } else if (direction == 't') {
        edgeT = y;
    } else {
        edgeB = y;
    }
}

void Delineator::RefineEdge(int axis) {
    mat z1,z2;

    if (axis == 1) {
        z1 = edgeL;
        z2 = edgeR;
    } else {
        z1 = edgeT;
        z2 = edgeB;
    }

    if (app == "nojag") {
        
        char s1,s2;

        if (axis == 1) {
            s1 = 'l';
            s2 = 'r';
        } else {
            s1 = 't';
            s2 = 'b';
        }

        /// TODO: caution here. 
        ivec v1 = JaggedEdges(z1,s1);
        ivec v2 = JaggedEdges(z2,s2);
        z1 = IndicesToSubArmaMat(z1,v1);
        z2 = IndicesToSubArmaMat(z2,v2);
    } else if (app == "nodup") {
        pair<mat,mat> x2 = EliminateDuplicatePoints(z1,z2,axis);
        z1 = x2.first;
        z2 = x2.second;
    } else {
        pair<ivec,ivec> x3 = EliminateContraryPointsIndices(z1,z2,axis);
        z1 = IndicesToSubArmaMat(z1,x3.first);
        z2 = IndicesToSubArmaMat(z2,x3.second);
    }

    if (axis == 1) {
        edgeL = z1;
        edgeR = z2;
    } else {
        edgeT = z1;
        edgeB = z2;
    }
}

void Delineator::OrderPointsByAxis(int axis) {
    if (axis == 0) {
        data = SortPoints(data,0);
    } else {
        data2 = SortPoints(data,1);
    }

}

/// [V]
pair<int,int> Delineator::IndexRangeForCurveDirection(char direction) {
    pair<int,int> x = make_pair(-1,-1);
    int j = curves.size();
    for (int i = 0; i < j; i++) {
        if (curves[i].d == direction) {
            if (x.first == -1) {
                x.first = i;
                x.second = i;
            } else {
                x.second = i;
            }
        } else {
            if (x.second != -1) {
                break;
            }
        }
    }

    return x;
}

vector<int> Delineator::ComplementForCurve(int curve_index) {
    char d = ComplementaryDirection(curves[curve_index].d);
    vector<int> v = {};
    pair<int,int> p = IndexRangeForCurveDirection(d);
    int axis = (curves[curve_index].d == 'l' || curves[curve_index].d == 'r') ? 1: 0; 

    vec x = {((curves[curve_index].f)->pts).first(axis),
        ((curves[curve_index].f)->pts).second(axis)};
    float fmin = x.min();
    float fmax = x.max();
    for (int i = p.first; i < p.second + 1; i++) {
        
        if (curves[curve_index].IsComplement(curves[i])) {
            v.push_back(i);
        } else {
            /// CAUTION: short-cut here.
            if (v.size() > 0) {
                break; 
            }
        }
    }

    return v;
}

/// - return
/// True if increasing, False if decreasing
bool Delineator::FlowOfDirection(char direction) {
  bool lt = (direction == 'l' || direction == 't'); 
  if (cw) {
    return lt; 
  } else {
    return !lt; 
  }
}

int Delineator::IndexOfDCurveInPointRange(rowvec p, char d) {
    pair<int,int> p2 = IndexRangeForCurveDirection(d);
    if (p2.first == -1 || p2.second == -1) {
        return -1;
    }

    for (int i = p2.first; i <= p2.second; i++) {
        if (curves[i].PointInRange(p)) {
            return i; 
        }
    }
    return -1; 
}
