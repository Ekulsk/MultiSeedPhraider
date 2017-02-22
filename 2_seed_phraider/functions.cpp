/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>

#include "Family.cpp"
using namespace std;

bool BelongsToFam(Family F, map<int, vector<int> > H, int v) {
//        cout<<"belongstofam"<<endl;
    return H[v].size() == H[F.first()].size() && H[v].front() - H[F.first()].front() == H[v].back() - H[F.first()].back();
}

void frequencyShift(map<int, vector<int> >& F_freq, int freq, int fam)
{
//        cout<<"fshift"<<endl;
    for (vector<int>::iterator it = F_freq[freq-1].begin(); it != F_freq[freq-1].end(); ++it)
        if (*it == fam)
        {
            F_freq[freq-1].erase(it);
            F_freq[freq].push_back(fam);
            return;
        }
}

void split(vector<Family>& F, map<int, vector<int> >& H, map<int, int >& H_family, int f, int keyNum, int s) {
//    cout<<"split"<<endl;

    vector<int>::iterator key_it=F[f].findLmerIt(keyNum);
    vector<int>::iterator end = F[f].lmer_list.end();

    Family newF;

    for (vector<int>::iterator it = key_it + 1; it != end; ++it) {
        newF.addLmer(*it);
        H_family[*it] = F.size();
    }

    F[f].lmer_list.erase(key_it, end);
    newF.last_seen = F[f].last_seen;
    F[f].last_seen = -1;
    F.push_back(newF);
}

void remove(vector<Family>& F, map<int, vector<int> >& H, map<int, int >& H_family, int f, int keyNum, int s){
//    cout<<"remove"<<endl;
    int key_index = F[f].findLmerIndex(keyNum);
    bool splitIsNecessary = F[f].determineConflict(H, key_index, s);
    if(splitIsNecessary)
        split(F, H, H_family, f, keyNum, s);
    else
    {
        vector<int>::iterator key_it = F[f].findLmerIt(keyNum);
        F[f].lmer_list.erase(key_it);
    }
}

void remove_range(vector<Family>& F, map<int, vector<int> >& H, map<int, int >& H_family, int f, int keyNum, int s){
//    cout<<"remove range"<<endl;
    int key_index = F[f].findLmerIndex(keyNum);
    bool splitIsNecessary = F[f].determineConflict(H, key_index, s);
    if(splitIsNecessary)
        split(F, H, H_family, f, keyNum, s);
    else
    {
        vector<int>::iterator key_it = F[f].findLmerIt(keyNum);
        F[f].lmer_list.erase(key_it);
    }
}

void findOrAdd(queue<int>& Q, vector<Family>& F, map<int, int>& H_family,
        map<int, vector<int> >& H, int keyNum, int i, int s) {
//    cout<<"findoradd"<<endl;
    bool fFound = false;
    int f = -1;
    for (int j = 0; j < Q.size(); j++) {
        int f2 = Q.front();
        Q.pop();
        Family F2 = F[f2];
        if (BelongsToFam(F2, H, keyNum)&&!fFound) {
            //Multiple families? So, do we do this multiple times?
            f = f2;
            fFound = true;
        } else {
            Q.push(f2);
        }
    }
    if (fFound) {
        F[f].addLmer(keyNum);
        F[f].last_seen = i;
        H_family[keyNum] = f;
        Q.push(f);
    } else {
        F.push_back(Family(keyNum, i));
        int key_index = F.size() - 1;
        H_family[keyNum] = key_index;
        Q.push(key_index);
    }
}

void advanceQueue(queue<int>& Q, vector<Family>& F, map<int, int>& H_family,
        map<int, vector<int> >& H, int keyNum, int i, int s) {
//    cout<<"advancequeue"<<endl;
    H[keyNum].push_back(i);
    if (H[keyNum].size() == 2) {
        findOrAdd(Q, F, H_family, H, keyNum, i, s);
    } else if (H[keyNum].size() > 2) {
        int f = H_family[keyNum];
        if (F[f].last_seen == -1){//(key == F[f].first()) {
            Q.push(f); //????????????? written as F.enqueue(v) in pseudocode
        }
        F[f].last_seen = i;
    }
}

void clean_up(map<int, int>& H_family, map<int, vector<int> >& H,
        vector<Family>& F, int f, int s) {
//    cout<<"cleanup"<<endl;
    int Fsize = F[f].lmer_list.size();
    int new_freq[Fsize];
    for (int i = 0; i < Fsize; i++) {
        new_freq[i] = H[F[f].lmer_list[i]].size();
    }
    int F_s = F.size(); 
    for (int i = 1; i < Fsize; i++) {
        if (new_freq[i] > 0 && new_freq[i] != new_freq[0]) {
            
            Family newF;
            newF.last_seen = -1;
            newF.addLmer(F[f].lmer_list[i]);
            bool split = false;
            for (int j = i + 1; j < Fsize && !split; j++) {
                if (new_freq[i] == new_freq[j]) {
                    if (H[F[f].lmer_list[i]].front() - H[F[f].lmer_list[j]].front() >= s) {
                        split=true;
                    } else {
                        newF.addLmer(F[f].lmer_list[j]);
                        H_family[F[f].lmer_list[j]]=F.size();
                        new_freq[j] = 0;
                    }
                }
            }
            F.push_back(newF);
            new_freq[i] = 0;
        }
    }

    if (F_s == F.size())
        return;
    int f_curr=f;
    int curr_loc=H[F[f].lmer_list[0]].front();
    vector<int> old_list=F[f].lmer_list;
    F[f].lmer_list.clear();
    F[f].addLmer(old_list[0]);
    for (int i = 1; i < Fsize; i++) {
        if (new_freq[i]!=0)
        {
            int last_loc=curr_loc;
            curr_loc=H[old_list[i]].front();
            if (curr_loc-last_loc>s)
            {
                f_curr=F.size();
                Family newF;
                F.push_back(newF);
            }
            F[f_curr].addLmer(old_list[i]);
            H_family[old_list[i]]=f_curr;
        }
    }
}

void assignSeed(vector<string> seed, map<string, int>& keyH, vector<string>& primaryKey, string key, int s)
{
//    cout<<"assignseed " <<endl;
    if(!primaryKey.empty())        
    for (int s_i=0;s_i<seed.size();s_i++)
    {
        string seed_i=seed.at(s_i);
        bool nDetected=false;
        for (int i=0;i<s;i++)
            {
                if (seed_i.at(i)=='1' && key.at(i)=='n')
                    nDetected=true;
            }
        if(!nDetected)
        {
            for (int p_i =0; p_i<primaryKey.size(); p_i++)
            {
                string p=primaryKey.at(p_i);
                bool noMatch=false;
                for (int i=0;i<s && !noMatch;i++)
                {
                    if (seed_i.at(i)=='1')
                    {
                        if (p.at(i)!=key.at(i))
                        {
                            noMatch=true;
                        }
                    }
                }
                if(!noMatch)
                {
                    keyH[key]=p_i;
                    return;
                }
            }
        }
    }
    keyH[key]=primaryKey.size();
    primaryKey.push_back(key);
}