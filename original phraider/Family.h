/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Family.h
 * Author: luke
 *
 * Created on March 8, 2016, 9:10 AM
 */

#include <vector>

using namespace std;
#ifndef FAMILY_H
#define FAMILY_H

class Family {
public:
    int last_seen;
    vector<string> lmer_list;
    string last;

    bool determineConflict(map<string, list<int> > H, string k, int s);
    
    void setLastLmer(string k);

    void addLmer(string k) ;

    vector<string>::iterator findLmer(string k);

    string first();
    
    Family() ;

    Family(string s);

    Family(string s, int i);

    Family(const Family& F);
private:

};

#endif /* FAMILY_H */

