/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Family.cpp
 * Author: luke
 * 
 * Created on March 8, 2016, 9:10 AM
 */

//#include "Family.h"

#include <vector>

using namespace std;

class Family {
public:
    int last_seen;
    vector<string> lmer_list;

    bool determineConflict(map<string, vector<int> > H, int key_index, int s) {
        if(key_index==0||key_index==lmer_list.size()-1)
            return false;
        if (H[lmer_list[key_index+1]].front() - H[lmer_list[key_index-1]].front() > s)
            return true;
        return false;
    }

    int findLmerIndex(string k) {
        for (int i=0;i<lmer_list.size();i++)
            if (lmer_list[i] == k)
                return i;
        return -1;
    }

    void addLmer(string k) {
        lmer_list.push_back(k);
    }

    vector<string>::iterator findLmerIt(string k) {
        for (vector<string>::iterator it = lmer_list.begin(); it != lmer_list.end(); ++it)
            if (*it == k)
                return it;
        return lmer_list.end();
    }

    string first() {
        return lmer_list[0];
    }

    Family() {
        last_seen = -1;
    }

    Family(string s) {
        lmer_list.push_back(s);
        last_seen = -1;
    }

    Family(string s, int i) {
        lmer_list.push_back(s);
        last_seen = i;
    }

    Family(const Family& F) {
        for (int i = 0; i < F.lmer_list.size(); i++)
            lmer_list.push_back(F.lmer_list[i]);
        last_seen = F.last_seen;
    }

};
