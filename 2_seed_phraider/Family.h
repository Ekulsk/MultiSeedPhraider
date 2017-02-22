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
    vector<int> lmer_list;
    int last;

    bool determineConflict(map<int, list<int> > H, int k, int s);
    
    void setLastLmer(int k);

    void addLmer(int k) ;

    vector<int>::iterator findLmer(int k);

    int first();
    
    Family() ;

    Family(int s);

    Family(int s, int i);

    Family(const Family& F);
private:

};

#endif /* FAMILY_H */

