#include "deline_md.hpp"


void MDClassification::AddClassification(int c, map<string,int> m) {
    mp[c] = m; 
}

/*
*/
pair<string,bool> DelineInstr::Next() {
    pair<string,bool> p = instr[index];
    index = (index + modulo_hop_cycle[index2]) % (instr.size());
    index2 = (index2 + 1) % (modulo_hop_cycle.size());
    return p;
}

void DelineMD::DelineateFull() {
    int c = data.n_cols - 1;
    for (int i = 0; i < c; i++) {
        if (i == axis) {
            continue;
        }

        DelineateAxis(i);
    }
}

void DelineMD::DelineateAxis(int col) {
    mat mx = SubmatOnCol(col);
    Deline22MC* dmc = new Deline22MC(mx);
    DelineateByModularInstr(dmc);
    dxs[col] = dmc;
}

/*
*/
void DelineMD::DelineateByModularInstr(Deline22MC* dmc) {
    pair<string,bool> sb;
    while (!dmc->finished_stat) {
        sb = di->Next();
        dmc->DelineateNextLabel(sb.first,sb.second,false);
    }

    
    if (clear_data) {
        dmc->ClearAllData();
    }
    
}

MDClassification* DelineMD::MDClassify(rowvec p) {
    MDClassification* mdc = new MDClassification(axis);

    int c = data.n_cols - 1;
    rowvec r;
    map<string,int> m;
    for (int i = 0; i < c; i++) {
        if (i == axis) {
            continue;
        }

        r = {p(i),p(axis)};
        m = dxs[i]->BaseClassification(r);
        mdc->AddClassification(i,m);
    }

    return mdc;
}

mat DelineMD::SubmatOnCol(int c) {
    vector<int> cs = {c,axis,int(data.n_cols) - 1};
    return Submat(cs);
}

mat DelineMD::Submat(vector<int> indices) {
    mat mx(0,0,fill::zeros);

    for (auto c: indices) {
        mx.insert_cols(mx.n_cols,data.col(c));
    }

    return mx;
}

void DelineMD::Display() {
    for (auto x: dxs) {
        cout << "* <Deline22MC>: " << x.first << endl;
        (x.second)->Display();
    }
}
