#include "deline_mc.hpp"

int Deline22MC::DelineateFull(string dname, bool cw,bool verbose) {

    int c = 0;
    while (!finished_stat) {
        DelineateNextLabel(dname,cw,verbose);
        c++;
    }

    AssignChildParentRelations(dx22s); 
    ClearAllData();
    return c;
}

void Deline22MC::DelineateNextLabel(string dname,bool cw,bool verbose) {

    if (finished_stat) {
        return;
    }

    if (verbose) { cout << "[x] prepare delineation" << endl;} 

    PrepareDelineation();
    if (finished_stat) {
        return;
    }

    /// TODO: inefficient
    if (verbose) {cout << "[x] preprocessing" << endl;}
    d->Preprocess();

    if (verbose) {cout << "[x] delineating" << endl;}
    d->Delineate(dname,cw);

    if (verbose) {cout << "[x] gathering delineated points" << endl;}
    d->DelineatedPoints();

    if (verbose) {cout << "[x] review delineation target" << endl;}
    ReviewDelineationTarget(d);

    if (verbose) {
        cout << "DELINEATE ON " << d->dname << " OF LABEL " << d->target_label << endl;
        (d->ds)->Display();
    }

    if (verbose) {cout << "[x] subdelineating" << endl;}

    SubDelineateOneFull(d,dname,cw); 
}

void Deline22MC::PrepareDelineation() {
    //// ISSUE: pre-process in Delineation
    ////        destroys indices 
    mat x;
    string s = "D22_" + to_string(idn);
    idn += 1;

    // case: initial delineation (of smallest point)
    if (d == nullptr) {
        x = data;
    } else {
        x = SubDataOfD22(d,true); 
    }

    if (x.n_rows == 0) {
        finished_stat = true;
        return;
    }

    d = new Deline22(x,s); 
}

vector<Deline22*> Deline22MC::SubDelineateOneFull(Deline22* dx, string dmethod,bool cw) {

    vector<Deline22*> dxs = {};
    vector<Deline22*> dxs2;
    deque<Deline22*> dxs_unf = {dx}; // unfinished. 
    Deline22* dxi;
    bool stat = (dxs_unf.size() > 0); 

    while (stat) {
        dxi = dxs_unf[0];
        dxs_unf.pop_front();
        dxs2 = SubDelineateOne(dxi,dmethod,cw);

        for (auto c: dxs2) {
            if (c->finished_stat) {
                dxs.push_back(c);
            } else {
                dxs_unf.push_back(c);
            }
        }

        stat = (dxs_unf.size() > 0);
    }

    copy(dxs.begin(), dxs.end(), back_inserter(dx22s));
    return dxs;
}

pair<mat,vector<IntKeyIntVectorValue>> Deline22MC::ReindexDelineation(Deline22* dx) {
    mat dt = SubDataOfD22(dx,false);
    map<int, vector<int>> x = IndexMapOfIntLabelledData(dt,2);

    vector<IntKeyIntVectorValue> pairs;
    for (auto itr = x.begin(); itr != x.end(); itr++) {
        pairs.push_back(*itr);
    }

    sort(pairs.begin(),pairs.end(), [](IntKeyIntVectorValue l, IntKeyIntVectorValue r)
    {
        return l.second.size() < r.second.size();
    });

    return make_pair(dt,pairs);
}

vector<Deline22*> Deline22MC::SubDelineateOne(Deline22* dx, string dmethod,bool cw) {
    dx->finished_stat = true;
    vector<Deline22*> d22s = {dx};

    if (dx->ddepth >= max_depth) {
        return d22s;
    }

    pair<mat,vector<IntKeyIntVectorValue>> q = ReindexDelineation(dx);

    for (auto p: q.second) {
        if (p.first == d->target_label) {
            continue;
        }

        mat data2 = q.first; 
        Deline22* d2 = PrepareSubDelineation(data2,conv_to<ivec>::from(p.second),p.first,dx->ddepth + 1);
        d2->Delineate(dmethod,cw);
        d2->DelineatedPoints();
        (d2->ds)->Display();
        d22s.push_back(d2);
    }

    return d22s;
}

void Deline22MC::PerformSubDelineation(Deline22* dx, string dmethod,bool cw) {

    dx->Delineate(dmethod,cw);
    dx->DelineatedPoints();
    ReviewDelineationTarget(dx);    
    return;
}

Deline22* Deline22MC::PrepareSubDelineation(mat dat, ivec ti ,int tl,int ddepth) {

    string s = "D22_" + to_string(idn);
    idn += 1;
    Deline22* d = new Deline22(dat,s,ddepth);
    d->target_label = tl;
    d->target_indices = ti;

    return d;
}

 void Deline22MC::ReviewDelineationTarget(Deline22* dx) {
    set<int> si = IntIterToSet(((dx->ds)->m)[dx->target_label]);

    bool b = false;
    for (auto c: dx->target_indices) {
        b = (si.find(c) != si.end());

        if (!b) {
            AddFaultyPoint((dx->data).row(c));
        }
    }
    return; 
 }

bool Deline22MC::AddFaultyPoint(rowvec p) {
    string s = NumIterToStringType(p,",",5);

    if (fpc.find(s) == fpc.end()) {
        fpc[s] = 1;
    } else {
        fpc[s] = fpc[s] + 1;
    }

    return (fpc[s] >= attempts_per_point);
}

/// - description
/// gathers the sub-data of `Deline22` instance minus the points
/// that have more the max `app` number of delineation attempts.
mat Deline22MC::SubDataOfD22(Deline22* dx,bool complement) {
    ivec iv = GatherMapValuesTypeVectorInt((dx->ds)->m); 
    iv = sort(iv);

    mat x;
    if (complement) {
        x = IndicesToComplementarySubArmaMat(dx->data,iv); 
    } else {
        x = IndicesToSubArmaMat(dx->data,iv);
    }

    return FilterFaultyFromData(x);
}

mat Deline22MC::FilterFaultyFromData(mat dat) {

    mat dat2(0,dat.n_cols,fill::zeros);
    string s;
    rowvec r; 
    for (int i = 0; i < dat.n_rows; i++) {
        r = dat.row(i);
        s = NumIterToStringType(r,",",5);
        if (fpc.find(s) != fpc.end()) {
            if (fpc[s] >= attempts_per_point) {
                continue;
            }
        }
        dat2.insert_rows(dat2.n_rows,r);
    }
    return dat2;

}

void Deline22MC::AssignChildParentRelations(vector<Deline22*> d22s) {
    int l = d22s.size();

    for (int i = 0; i < l - 1; i++) {
        for (int j = i + 1; j < l; j++) {
            if (d22s[i]->IsSuperSetOf(d22s[j])) {
                d22s[i]->AddHRelation(d22s[j],true); 
            } else if (d22s[j]->IsSuperSetOf(d22s[i])) {
                d22s[j]->AddHRelation(d22s[i],true); 
            }
        }
    }
}

map<string,int> Deline22MC::HierarchicalClassification(rowvec p) {
    map<string,int> msi = map<string,int>();

    for (auto x: dx22s) {
        if (x->ClassifyPoint(p,0) != -1 || x->ClassifyPoint(p,1) != -1) {
            msi[x->dname] = x->target_label;
        }
    }

    return msi;
}

map<string,int> Deline22MC::BaseClassification(rowvec p) {
    map<string,int> msi = HierarchicalClassification(p);

    vector<Deline22*> ds;
    for (auto c: dx22s) {
        if (msi.find(c->dname) != msi.end()) {
            ds.push_back(c);
        }
    }

    int j = ds.size();
    set<string> exclude = set<string>();
    bool stat;
    for (int i = 0; i < j - 1; i++) {
        stat = true;
        for (int k = i + 1; k < j;k++) {
            if (ds[i]->IsParent(ds[k]->dname)) {
                exclude.insert(ds[i]->dname);
            }

            if (ds[i]->IsChild(ds[k]->dname)) {
                exclude.insert(ds[k]->dname);
            }
        }
    }

    map<string,int> msi2;
    for (auto c2: msi) {
        if (exclude.find(c2.first) == exclude.end()) {
            msi2[c2.first] = c2.second;
        }
    }

    return msi2;
}

void Deline22MC::ClearAllData() {
    // clear out excess data from each Deline22* except for the
    cout << "CLEARING" << endl; 
    for (auto dx_: dx22s) {
        dx_->ClearData();
    }
}

void Deline22MC::Display() {
    for (auto xx: dx22s) {
        xx->Display();
    }
}
