#include "deline.hpp"
using namespace std;

void Deline22Score::Display() {
  cout << "positive score: " << ltr.first << " " << ltr.second << endl;
  cout << "negative score: " << ntr.first << " " << ntr.second << endl;

  cout << "-- stats" << endl;
  for (auto c: m) {
    cout << "label " << c.first << " size " << c.second.size() << endl;
  }
}

set<int> Deline22Score::IndicesToSet() {
  ivec iv = GatherMapValuesTypeVectorInt(m);
  set<int> s;
  for (auto iv_: iv) {
      s.insert(iv_);
  }

  return s;
}

/*
loads a pre-process that has already been calculated. 
*/
void Deline22::LoadPreprocess(map<int, vector<int>> m) {
    pair<int, vector<int>> min 
      = *min_element(m.begin(), m.end(), MapValueComparator());

    target_label = min.first;
    target_indices = conv_to<ivec>::from(min.second);
}

void Deline22::Preprocess() {
    map<int, vector<int>> x = IndexMapOfIntLabelledData(data,2);
    pair<int, vector<int>> min 
      = *min_element(x.begin(), x.end(), MapValueComparator());

    target_label = min.first;

    //// CASE: only one point 
    if (min.second.size() == 1) {
      int q = data.n_rows; 

      rowvec rv = {data(min.second[0],0) + 0.2,data(min.second[0],1) - 0.2,float(min.first)};      
      data.insert_rows(data.n_rows,rv);

      rv = {data(min.second[0],0) + 0.2,data(min.second[0],1) + 0.2,float(min.first)};      
      data.insert_rows(data.n_rows,rv);

      rv = {data(min.second[0],0) - 0.2,data(min.second[0],1) + 0.2,float(min.first)};      
      data.insert_rows(data.n_rows,rv);

      rv = {data(min.second[0],0) - 0.2,data(min.second[0],1) - 0.2,float(min.first)};      
      data.insert_rows(data.n_rows,rv);

      min.second.push_back(q);
      min.second.push_back(q + 1);
      min.second.push_back(q + 2);
      min.second.push_back(q + 3);
    }

    target_indices = conv_to<ivec>::from(min.second);
}

void Deline22::Delineate(string dmethod,bool cw) {
    mat data2 = IndicesToSubArmaMat(data,target_indices);
    del = new Delineator(data2,cw,dmethod);
    del->CROps();
    del->BuildDefaultCurveSequence(); 
    return; 
}

/// score-modulate operation
/// get start and end index of data D as D' of relevance for axis
/// for each curve in direction, get its complement set
///     get start and end index of D' for the curve.
///     determine which fit works better.
pair<pair<int,int>,pair<int,int>> Deline22::SMOp(int axis) {
  
  OrderPointsByAxis(axis);
  vector<char> ds;
  if (axis == 1) {
    ds = {'l','r'};
  } else {
    ds = {'t','b'};
  }

  // first direction
  PreSMOpOnDirection(ds[0]);
  pair<int,int> px1 = make_pair(0,0);
  for (int i = cda.first.first; i <= cda.first.second; i++) {
    pair<int,int> px11 = ScoreModulateCurve(i);
    px1.first += px11.first;
    px1.second += px11.second;
  }

  // complementary direction
  PreSMOpOnDirection(ds[1]);
  pair<int,int> px2 = make_pair(0,0);
  for (int i = cda.first.first; i <= cda.first.second; i++) {
    pair<int,int> px22  = ScoreModulateCurve(i);
    px2.first += px22.first;
    px2.second += px22.second;
  }

  return make_pair(px1,px2); 
}

void Deline22::PreSMOpOnDirection(char d) {
  int axis = DirectionToAxis(d);
  cda = CurveDirectionAnalysis(d);
  if (axis) {
    taxis1 = 0;
  } else {
    taxis0 = 0;
  }
}

pair<int,int> Deline22::ScoreModulateCurve(int curve_index) {
  DCurve d = del->curves[curve_index];
  int xi = (d.d == 'l' || d.d == 'r') ? taxis1 : taxis0; 
  vec ext2 = (d.d == 'l' || d.d == 'r') ? d.Ext1() : d.Ext0();

  pair<int,int> pr = RangeOfPointsInExtremum(d.d,xi,data.n_rows - 1,ext2(0),ext2(1));

  if (pr.first == -1) {
    return make_pair(-1,-1);
  }

  pair<int,int> p1 = ClassifyPointsByInfo(curve_index, pr);
  pair<int,int> p2 = p1;

  if (!(d.IsLineFit())) {
    d.Modulate();
    ClassifyPointsByInfo(curve_index, pr);
  }

  if (p1.first >= p2.first) {
    d.Modulate();
  }

  // update the start index
  if (d.d == 'l' || d.d == 'r') {
    taxis1 = pr.second + 1;
  } else {
    taxis0 = pr.second + 1; 
  }

  del->curves[curve_index] = d;
  return (p1.first >= p2.first) ? p1 : p2;
}

/// - return
/// 
pair<int,int> Deline22::ClassifyPointsByInfo(int curve_index, pair<int,int> pr) {
  // determine the orientation
  vector<int> complements = del->ComplementForCurve(curve_index);

  // order vector in ascending order by axis
  char d_ = ComplementaryDirection(del->curves[curve_index].d);
  if (!(del->FlowOfDirection(d_))) {
    reverse(complements.begin(),complements.end());
  }

  int j = 0;
  bool b; 
  pair<int,int> p = make_pair(0,0);
  for (int i = pr.first; i <= pr.second; i++) {
    // if point not in range, increment j
    if (!((del->curves[complements[j]]).PointInRange(data.row(i)))) {
      j += 1;
      assert (j < complements.size()); 
    }

    // classify point by curve pair
    bool b = ClassifyPointByCurvePair(make_pair(curve_index,complements[j]),data.row(i));
    bool b2 = (int(data(i,2)) == target_label);
    if (b && b2) {
      p.first = p.first + 1; 
    } else if (!b && !b2) {
      p.first = p.first + 1;
    }
    p.second = p.second + 1; 
  }

  return p; 
}

bool Deline22::ClassifyPointByCurvePair(pair<int,int> cp,rowvec p) {
  assert ((del->curves[cp.first]).d == ComplementaryDirection((del->curves[cp.second]).d)); 

  int a = DirectionToAxis((del->curves[cp.first]).d);

  rowvec r; 
  if (a) {
    r = {(del->curves[cp.first]).YFit(p(a)), (del->curves[cp.second]).YFit(p(a))};
  } else {
    r = {(del->curves[cp.first]).Fit(p(a)), (del->curves[cp.second]).Fit(p(a))};
  } 
  
  int b = (a + 1) % 2;
  return (p(b) >= RoundDecimalNPlaces(r.min(),5) && p(b) <= RoundDecimalNPlaces(r.max(),5));
}

int Deline22::ClassifyPoint(rowvec p,int axis) {
  int i1=-1;
  int i2=-1;

  if (axis == 1) {
    i1 = del->IndexOfDCurveInPointRange(p,'l');
    i2 = del->IndexOfDCurveInPointRange(p,'r');
  } else {
    i1 = del->IndexOfDCurveInPointRange(p,'t');
    i2 = del->IndexOfDCurveInPointRange(p,'b');
  }

  if (i1 == -1 || i2 == -1) {
    return -1;
  }

  if (ClassifyPointByCurvePair(make_pair(i1,i2),p)) {
    return target_label;
  }

  return -1;
}

void Deline22::DelineatedPoints() {
  vector<int> r = {};
  map<int, vector<int>> x;
  pair<int,int> ps = make_pair(0,0);
  pair<int,int> ns = make_pair(0,0);
  for (int i = 0; i < data.n_rows; i++) {

    // update totals for label and non-label 
    if (int(data(i,2)) == target_label) {
          ps.second = ps.second + 1;
    } else {
        ns.second = ns.second + 1;
    }

    bool blabel = (ClassifyPoint(data.row(i),0) != -1 || ClassifyPoint(data.row(i),1) != -1);
    if (blabel) {
        // add to map
        if (x.find(int(data(i,2))) == x.end()) {
            x[int(data(i,2))] = {i}; 
        } else {
            x[int(data(i,2))].push_back(i); 
        }

        // update score
        if (int(data(i,2)) == target_label) {
          ps.first = ps.first + 1;
        }
    } else {
        if (int(data(i,2)) != target_label) {
          ns.first = ns.first + 1;
        }
    }

  }
  
  finished_stat = (x.size() < 2); 
  ds = new Deline22Score(x,ps,ns); 
}


/// - return
/// (start index of curve direction, end index of curve direction)
/// (minumum value on curve direction axis, maximum value on curve direction axis)
pair<pair<int,int>,pair<float,float>> Deline22::CurveDirectionAnalysis(char direction) {
    int axis = (direction == 'l' || direction == 'r') ? 1: 0;
    pair<int,int> ir = del->IndexRangeForCurveDirection(direction);

    vec x = {(del->curves[ir.first]).First()(axis),
      (del->curves[ir.first]).Second()(axis),
      (del->curves[ir.second]).First()(axis),
      (del->curves[ir.second]).Second()(axis)};
    pair<float,float> xs = make_pair(x.min(),x.max());
    return make_pair(ir,xs); 
}



/// - arguments
/// curve_dir := l|r|t|b
/// data_mini := start index of checking points
/// data_maxi := end index of checking points
/// xmin := minumum point of qualification
/// xmax := maximum point of qualification
///
/// - return
/// (minimum index, maximum index [inclusive]) 
///
/// NOTE: data is assumed to be ordered by axis of `curve_dir`. 
pair<int,int> Deline22::RangeOfPointsInExtremum(char curve_dir,int data_mini, int data_maxi,float xmin,float xmax) {
    // get the curve extremum;
    int axis = (curve_dir == 'l' || curve_dir == 'r') ? 1: 0;
    pair<int,int> r = make_pair(-1,-1);
    for (int i = data_mini; i <= data_maxi; i++) {
      if (data(i,axis) >= xmin && data(i,axis) <= xmax) {
        if (r.first == -1) {
          r.first = i;
          r.second = i;
        } else {
          r.second = i; 
        }
      }

      if (data(i,axis) > xmax) {
        break; 
      }
    }

    return r;
}

void Deline22::OrderPointsByAxis(int axis) { 
    data = SortPoints(data,axis);
}

/// TODO: inefficient, repeats classification on duplicate points
bool Deline22::IsSuperSetOf(Deline22* dx) {
    for (auto c: (dx->del)->curves) {
      mat m = c.f->PointsToMat();
      bool s0 = (ClassifyPoint(m.row(0),0) != -1 || ClassifyPoint(m.row(0),1) != -1);
      bool s1 = (ClassifyPoint(m.row(1),0) != -1 || ClassifyPoint(m.row(1),1) != -1);

      if (!(s0 && s1)) {
        return false;
      }
    }

    return true;
}

void Deline22::AddHRelation(Deline22* dx, bool is_parent) {
  if (is_parent) {
    dh.child_names.insert(dx->dname);
    (dx->dh).parent_names.insert(dname);
  } else {
    dh.parent_names.insert(dx->dname);
    (dx->dh).child_names.insert(dname);
  }
}

bool Deline22::IsParent(std::string name) {
  return (dh.parent_names.find(name) != dh.parent_names.end());
}

bool Deline22::IsChild(std::string name) {
  return (dh.child_names.find(name) != dh.child_names.end());
}

void Deline22::ClearData() {
  
  data.reset();
  (del->data).reset();
  (del->data2).reset();

  (del->edgeL).reset();
  (del->edgeR).reset();
  (del->edgeT).reset();
  (del->edgeB).reset();
  
}

int Deline22::IndexOfClosestPoint(rowvec p) {
  rowvec p_ = {p(0),p(1)};
  mat dat(0,0,fill::zeros);
  dat.insert_cols(0,data.col(0));
  dat.insert_cols(1,data.col(1));
  return MinPointByEuclideanDistance(dat,p_);
}

int Deline22::IndexOfClosestPointOnEdge(rowvec p,char direction) {
  rowvec p_ = {p(0),p(1)};
  mat ed = del->Edge(direction);
  ed.shed_col(2);
  return MinPointByEuclideanDistance(ed,p);
}

void Deline22::Display() {
  cout << "* <Delineation22>: " << dname << "  * target: " << target_label << endl;
  cout << "\t hierarchy" << endl;
  dh.Display();
  cout << "SZ1: " << data.n_rows << " SZ2: " << del->data.n_rows << endl;
  cout << "\t score" << endl;
  ds->Display();
  cout << "\t----" << endl;
  for (auto c: del->curves) {
    c.Display();
  }
}