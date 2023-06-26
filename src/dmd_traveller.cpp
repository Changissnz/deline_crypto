#include "dmd_traveller.hpp"
using namespace std;

/// outputs the next index in the edges
pair<char,int> IncrementNextIndexInEdges(vector<pair<char,int>>* mx, vector<int> szs, char direction) {

    // get the index of direction
    int qi;
    for (int i =0; i < (*mx).size(); i++) {
        if ((*mx)[i].first == direction) {
            qi = i;
            break;
        }
    }

    if (szs[qi] == 0) {
        return (*mx)[qi];
    } 

    (*mx)[qi].second = ((*mx)[qi].second + 1) % szs[qi]; 
    
    return (*mx)[qi];
}

rowvec DeltaOnCoord(rowvec p, pair<float,float> dx, float sgn, int axis, AbstractPRNG* aprng) {
    float c1 = aprng->PRFloatInRange(make_pair(abs(dx.first),abs(dx.second)),5);
    p(axis) = p(axis) + (c1 * sgn);
    return p;
}

/// TODO: test this
rowvec DeltaOnPoint(pair<float,float> rr, rowvec p, float r0, float r1,AbstractPRNG* aprng) {

    float d1 = r0 - p(0);
    float d2 = r1 - p(1);
    float radius = aprng->PRFloatInRange(rr,5);
    rowvec p2;
    if (abs(d1) < abs(d2) && abs(d1) != 0.) {
        vec v = {radius,abs(d1)};
        float xq = v.min();
        int sgn = (d1 < 0) ? -1 : 1;
        xq = (d1 < 0) ? xq * -1. : xq;

        v = {abs(rr.first),abs(xq)};
        pair<float,float> pf = make_pair(abs(v.min()),abs(v.max()));
        p2 = DeltaOnCoord(p,pf,sgn,0,aprng);
    } else {
        vec v = {radius,abs(d2)};
        float xq = v.min();
        int sgn = (d2 < 0) ? -1 : 1;
        xq = (d2 < 0) ? xq * -1. : xq;

        v = {abs(rr.first),abs(xq)};
        pair<float,float> pf = make_pair(abs(v.min()),abs(v.max()));
        p2 = DeltaOnCoord(p,pf,sgn,1,aprng);
    }

    // case: 0-distance for one of the axes 
    if (abs(d1) == 0. || abs(d2) == 0.) {
        return p2;
    }

    float d = EuclideanDistance(p,p2);
    float c2 = radius - d;
    if (abs(d1) < abs(d2)) {
        assert (c2 <= abs(d2));
        float fx = (d2 < 0) ? -1. : 1.;
        p2(1) = p2(1) + (c2 * fx);
    } else {
        assert (c2 <= abs(d1));
        float fx = (d1 < 0) ? -1. : 1.;
        p2(0) = p2(0) + (c2 * fx);
    }

    return p2;
}

/////////////////////////////////////////

/// jitter-pattern momentum
void JPMomentum(Deline22* d,AbstractPRNG* aprng, vec iv) {

    // iterate through the data container of d
    int x = (d->data).n_rows;
    for (int i = 0; i < x; i++) {
        rowvec rv = {d->data(i,0) + iv(0),d->data(i,1) + iv(1),d->data(i,2)}; 
        (d->data).row(i) = rv;
        iv = ModifyMomentum(d, aprng, iv,i);
    }
}

/// modifies the momentum vector `iv` by the following scheme:
/// - assign q as the maximum number of hops to take, q a number output by `aprng` in the range [0,`index`].
/// - for the `index` of d, take at most q hops back starting from the `index` in the range
///   [1,`index'` / 2], where `index'` is the `index` after taking some arbitrary
///   hops. For each `index'`, add it to `iv` by one of these two possibilities:
///
///      - if `aprng` outputs 1, then simply add `iv'` to `d->data[index']`, where `iv'` is the 
///        `iv` during or after the process of applying those changes.  
///      - otherwise (`aprng` outputs 0), add `iv'` to the reverse (flipped indices) of `d->data[index']`.
///         
vec ModifyMomentum(Deline22* d,AbstractPRNG* aprng, vec iv,int index) {

    rowvec rv = conv_to<rowvec>::from(iv);
    // choose the maximum number of additives in the range
    int q = aprng->PRIntInRange(make_pair(0,index));
    int index2 = index;

    while (index2 >= 0 && q > 0) {
        int h = aprng->PRIntInRange(make_pair(0,int(index2 / 2)));
        index2 -= h;

        if (index2 < 0) {
            continue;
        }

        rowvec rv2 = (d->data).row(index2);
        int h2 = aprng->PRIntInRange(make_pair(0,1));

        if (h2) {
            rv(0) = rv(0) + rv2(0);
            rv(1) = rv(1) + rv2(1);
        } else{
            rv(0) = rv(0) + rv2(1);
            rv(1) = rv(1) = rv2(0);
        }
        
        q -= 1;
    }

    return conv_to<vec>::from(rv);
}

/////////////////////////////////////////

void DMDTraveller::AlterDeline22(Deline22* d22,pair<float,float> rr,bool expand) {
    vector<char> vc = (d22->del)->ClockwiseOrder();
    for(auto cc: vc) {
        AlterDeline22Edge(d22,rr,cc,expand);
    }

    AlterDeline22VertexPoints(d22,rr,expand);
    (d22->del)->RebuildDefaultCurveSequence(); 
}

/// alters all non-vertex points on edge
///
/// rr := (min change, max change)
/// direction := l|r|t|b
/// expand := modify edge so area of delineation increases?
void DMDTraveller::AlterDeline22Edge(Deline22* d22,pair<float,float> rr, char direction,bool expand) {
    mat ed = (d22->del)->Edge(direction);
    int sz = ed.n_rows - 1; 
    rowvec p2;
    for (int i = 1; i < sz; i++) {
        p2 = AlterNonVertexPoint(ed.row(i),ed.row(i -1),ed.row(i + 1),direction,expand,rr);
        ed.row(i) = p2;
    }

    (d22->del)->AssignEdge(ed,direction);
}

void DMDTraveller::AlterDeline22VertexPoints(Deline22* d22,pair<float,float> rr, bool expand) {
    vector<char> vc = (d22->del)->ClockwiseOrder();

    int sz = vc.size();
    for (int i = 0; i < sz; i ++) {
        AlterDeline22VertexPointsOfEdge(d22,vc[i],vc[(i + 1) % sz],rr,expand); 
    }
}

// alters the last point of edge `direction` and the first point of edge `postEdgeDir`. 
void DMDTraveller::AlterDeline22VertexPointsOfEdge(Deline22* d22, char direction, char postEdgeDir, pair<float,float> rr, bool expand) {

    int ax = DirectionToAxis(direction);
    int ax1 = (ax + 1) % 2;

    mat mt1 = (d22->del)->Edge(direction);

    if (mt1.n_rows == 0) {
        return;
    }

    rowvec p = mt1.row(mt1.n_rows - 1);

    set<char> tl = {'t','l'};
    set<char> lb = {'b','l'};
    set<char> br = {'b','r'};
    set<char> rt = {'t','r'};

    set<char> tg = {direction,postEdgeDir};
    
    float sgn0,sgn1;
    if (tg == tl) {
        sgn0 = -1.;
        sgn1 = 1.;
    } else if (tg == lb) {
        sgn0 = -1.;
        sgn1 = -1.;
    } else if (tg == br) {
        sgn0 = 1.;
        sgn1 = -1.;
    } else {
        sgn0 = 1.;
        sgn1 = 1.;
    }

    if (!expand) {
        sgn0 = sgn0 * -1;
        sgn1 = sgn1 * -1; 
    }

    float r0 = p(0) + (sgn0 * rr.second);
    float r1 = p(1) + (sgn1 * rr.second);

    rowvec p3 = DeltaOnPoint(rr, p, r0, r1,aprng);
    mt1.row(mt1.n_rows - 1) = p3; 
    (d22->del)->AssignEdge(mt1,direction);
    mat mt2 = (d22->del)->Edge(postEdgeDir);

    if  (mt2.n_rows == 0) {
        return; 
    }

    mt2.row(0) = p3;
    (d22->del)->AssignEdge(mt2,postEdgeDir);
}

/// TODO: test this
rowvec DMDTraveller::AlterNonVertexPoint(rowvec p,rowvec prev, rowvec post, char direction, bool expand, pair<float,float> rr) {

    int ax = DirectionToAxis(direction);
    int ax1 = (ax + 1) % 2;
    float ft = aprng->PRFloatInRange(make_pair(0.,1.),5);

    float r0,r1;
    float dt = expand ? 1.: -1.;//

    if (direction == 'l' || direction == 'b') {
        dt = dt * -1.;
    }
    
    rowvec pref = (ft > 0.5) ? post : prev;

    // maximum delta on axis
    float q1 = p(ax) + (pref(ax) - p(ax)) / 2.0;
    // maximum delta on anti-axis
    float q2 = p(ax1) +  (rr.second * dt);

    if (ax == 1) {
        r1 = q1;
        r0 = q2;
    } else {
        r1 = q2;
        r0 = q1; 
    }

    return DeltaOnPoint(rr, p, r0, r1,aprng);
}

void DMDTraveller::JitterDeline22(Deline22* d22, vec v,function<void (Deline22*,AbstractPRNG*,vec)> jitterPatternFunc) {
    jitterPatternFunc(d22,aprng,v);
}


/// 
rowvec DMDTraveller::MoveOne(Deline22* d22, int index, vector<char> directions,vec pointPace,bool assignToClosest) {
    rowvec v = MoveOnePoint_(d22, index, directions,pointPace);
    ///cout << "point " << v << endl;
    int j = d22->IndexOfClosestPoint(v);
    ///cout << "closest point index: " << j << endl;
    d22->tindex = j;
    if (assignToClosest) {
        ///cout << "\t-- assign closest" << endl;
        travel_points.insert_rows(travel_points.n_rows, d22->data.row(j));
        return d22->data.row(j);
    }

    rowvec v2 = {v(0),v(1),d22->data(j,2)};
    travel_points.insert_rows(travel_points.n_rows,v2);
    return v;
}


rowvec DMDTraveller::MoveOnePoint_(Deline22* d22, int index, vector<char> directions,vec pointPace) {
    // select the points by reference
    mat mxx = SelectPointsByReference(d22,index,directions);
    tp_considered.insert_rows(tp_considered.n_rows,mxx);
    tpc_indices.push_back(tp_considered.n_rows);
    rowvec rv_ = d22->data.row(index);

    rowvec rv = {rv_(0),rv_(1)};
    return ApplyPointDestinations(mxx,rv,pointPace);
}

mat DMDTraveller::SelectPointsByReference(Deline22* d22, int index, vector<char> directions) {

    // collect the edge points into a map
    vector<char> vc = (d22->del)->ClockwiseOrder();
    vector<pair<char,int>> mx;
    vector<int> szs;
    rowvec rv = d22->data.row(index);

    for (auto vc_: vc) {
        mx.push_back(make_pair(vc_,
            d22->IndexOfClosestPointOnEdge(d22->data.row(index),vc_)));
        szs.push_back(((d22->del)->Edge(vc_)).n_rows);
    }

    mat mxx(0,0,fill::zeros);
    pair<char,int> j;
    rowvec rx;
    for (auto d_: directions) {
        j = IncrementNextIndexInEdges(&mx,szs,d_);
        int szr = (d22->del->Edge(j.first)).n_rows;

        // 0-case: if edge does not exist 
        rowvec rq;
        if (szr == 0) {
            rq = d22->data.row(0);
        } else {
            rq = ((d22->del)->Edge(j.first)).row(j.second);
        }

        mxx.insert_rows(mxx.n_rows,rq);
    }

    mxx.shed_col(2);
    return mxx;
    
    ///////////////////////////////////////////////////////





}

void DMDTraveller::ClearTravelData() {
    travel_points.clear();
    tp_considered.clear();
    tpc_indices.clear();
}

Deline22* DMDTraveller::SelectD22(int dmci, int d22i) {
    
    pair<int,int> pi = ModuloOnD22Index(dmci,d22i);
    dmci = pi.first;
    d22i = pi.second;
    return (((dmd->dxs)[dmci])->dx22s)[d22i];
}

Deline22MC* DMDTraveller::SelectDMC(int dmci) {
    pair<int,int> pi = ModuloOnD22Index(dmci,0);
    return (dmd->dxs)[pi.first];
}

/// calculates the information about the delineations:
/// - return:
///     number of Deline22MC's, map<identifier for Deline22MC, number of Deline22's>
pair<int, map<int,int>> DMDTraveller::Info() {
    int z = (dmd->dxs).size();
    map<int,int> d;

    for (auto b: (dmd->dxs)) {
        d[b.first] = ((dmd->dxs[b.first])->dx22s).size();
    }

    return make_pair(z,d); 
}

pair<int,int> DMDTraveller::ModuloOnD22Index(int dmci,int d22i) {
    pair<int,map<int,int>> x = Info();
    dmci = dmci % x.first;
    d22i = d22i % x.second[dmci];
    return make_pair(dmci,d22i);
}

mat DMDTraveller::LastNthTPCSequence(int i) {
    pair<int,int> sr = LastNthSequenceIndices(i);

    vector<int> vi;
    for (int j = sr.first; j < sr.second; j++) {
        vi.push_back(j);
    }

    ivec vi_ = conv_to<ivec>::from(vi);
    return IndicesToSubArmaMat(tp_considered,vi_); 
}

pair<int,int> DMDTraveller::LastNthSequenceIndices(int i) {

    assert(i > 0);
    assert ((i - 1) < tpc_indices.size());

    // default is last
    int index = tpc_indices.size() - i;
    int l = tpc_indices[index];
    int s = 0;
    if (index > 0) {
        s = tpc_indices[index - 1]; 
    }

    return make_pair(s,l);
}

DMDTraveller* DMDTravellerCase1() {
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

    LCG* lcg = new LCG(4,5,6,7);
    return new DMDTraveller(dmd, lcg);
}