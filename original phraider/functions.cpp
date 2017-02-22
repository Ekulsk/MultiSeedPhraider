/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>

#include "Family.cpp"
using namespace std;

bool BelongsToFam(Family F, map<string, vector<int> > H, string v) {
    return H[v].size() == H[F.first()].size() && H[v].front() - H[F.first()].front() == H[v].back() - H[F.first()].back();
}

void frequencyShift( map<int, vector<int> >& F_freq, int freq, int fam)
{
    for (vector<int>::iterator it = F_freq[freq-1].begin(); it != F_freq[freq-1].end(); ++it)
        if (*it == fam)
        {
            F_freq[freq-1].erase(it);
            F_freq[freq].push_back(fam);
            return;
        }
}

void split(vector<Family>& F, map<string, vector<int> >& H, map<string, int >& H_family, int f, string key, int s) {

    vector<string>::iterator key_it=F[f].findLmerIt(key);
    vector<string>::iterator end = F[f].lmer_list.end();

    Family newF;

    for (vector<string>::iterator it = key_it + 1; it != end; ++it) {
        newF.addLmer(*it);
        H_family[*it] = F.size();
    }

    F[f].lmer_list.erase(key_it, end);
    newF.last_seen = F[f].last_seen;
    F[f].last_seen = -1;
    F.push_back(newF);
}

void remove(vector<Family>& F, map<string, vector<int> >& H, map<string, int >& H_family, int f, string key, int s){
    int key_index = F[f].findLmerIndex(key);
    bool splitIsNecessary = F[f].determineConflict(H, key_index, s);
    if(splitIsNecessary)
        split(F, H, H_family, f, key, s);
    else
    {
        vector<string>::iterator key_it = F[f].findLmerIt(key);
        F[f].lmer_list.erase(key_it);
    }
}

void remove_range(vector<Family>& F, map<string, vector<int> >& H, map<string, int >& H_family, int f, string key, int s){
    int key_index = F[f].findLmerIndex(key);
    bool splitIsNecessary = F[f].determineConflict(H, key_index, s);
    if(splitIsNecessary)
        split(F, H, H_family, f, key, s);
    else
    {
        vector<string>::iterator key_it = F[f].findLmerIt(key);
        F[f].lmer_list.erase(key_it);
    }
}

void findOrAdd(queue<int>& Q, vector<Family>& F, map<string, int>& H_family,
        map<string, vector<int> >& H, string key, int i) {
    bool fFound = false;
    int f = -1;
    for (int j = 0; j < Q.size(); j++) {
        int f2 = Q.front();
        Q.pop();
        Family F2 = F[f2];
        if (BelongsToFam(F2, H, key)&&!fFound) {
            //Multiple families? So, do we do this multiple times?
            f = f2;
            fFound = true;
        } else {
            Q.push(f2);
        }
    }
    if (fFound) {
        F[f].addLmer(key);
        F[f].last_seen = i;
        H_family[key] = f;
        Q.push(f);
    } else {
        F.push_back(Family(key, i));
        int key_index = F.size() - 1;
        H_family[key] = key_index;
        Q.push(key_index);
    }
}

void advanceQueue(queue<int>& Q, vector<Family>& F, map<string, int>& H_family,
        map<string, vector<int> >& H,/* map<int, vector<int> >& F_freq, */string key, int i, int s) {
    H[key].push_back(i);
    if (H[key].size() == 2) {
        findOrAdd(Q, F, H_family, H, key, i);
    } else if (H[key].size() > 2) {
        int f = H_family[key];
        if (F[f].last_seen == -1){//(key == F[f].first()) {
            Q.push(f); //????????????? written as F.enqueue(v) in pseudocode
        }
        F[f].last_seen = i;
    }
}

void clean_up(map<string, int>& H_family, map<string, vector<int> >& H,
        vector<Family>& F, int f, int s) {
    
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
    vector<string> old_list=F[f].lmer_list;
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