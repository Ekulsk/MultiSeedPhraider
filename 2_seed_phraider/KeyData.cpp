/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>
#include <string>

using namespace std;

class KeyData {
public:
    void addKey(string nK)
    {
        for(int i=0; i<nK.length() ;i++ )
        {
            if(nK[i]=='a' || nK[i]=='A')
            {
                a[i]++;
            }
            else if(nK[i]=='c'|| nK[i]=='C')
            {
                c[i]++;
            }
            else if(nK[i]=='g' || nK[i]=='G')
            {
                g[i]++;
            }
            else if(nK[i]=='t' || nK[i]=='T')
            {
                t[i]++;
            }
        }
    }
    string averageString()
    {
        string result="";
        for(int i=0; i<primaryKey.length() ;i++ )
        {
            result+=max_at(i);
        }
        return result;
    }
    string max_at(int i)
    {
        int maxNum =a[i];
        string r = "a";
        
        if (c[i]>maxNum)
        {
            r="c";
            maxNum=c[i];
        }
        else if(c[i]==maxNum)
        {
            r+="c";
        }
        
        if (g[i]>maxNum)
        {
            r="g";
            maxNum=g[i];
        }
        else if(g[i]==maxNum)
        {
            r+="g";
        }
        
        if (t[i]>maxNum)
        {
            r="t";
            maxNum=t[i];
        }
        else if(t[i]==maxNum)
        {
            r+="t";
        }
        
        if (r.length()>1)
            r="["+r+"]";
        return r;
    }
    void dumpData(KeyData other)
    {
        for(int i=0; i<primaryKey.length() ;i++ )
        {
            a[i]+=other.a[i];
            c[i]+=other.c[i];
            g[i]+=other.g[i];
            t[i]+=other.t[i];
        }
    }
    KeyData(string pK)
    {
        primaryKey=pK;
        for(int i=0; i<pK.length();i++ )
        {
            if(pK[i]=='a' || pK[i]=='A')
            {
                a.push_back(1);
            }
            else
                a.push_back(0);
            if(pK[i]=='c'|| pK[i]=='C')
            {
                c.push_back(1);
            }
            else
                c.push_back(0);
            if(pK[i]=='g' || pK[i]=='G')
            {
                g.push_back(1);
            }
            else
                g.push_back(0);
            if(pK[i]=='t' || pK[i]=='T')
            {
                t.push_back(1);
            }
            else
                t.push_back(0);
        }
    }
    KeyData()
    {
        
    }
private:    
    string primaryKey;
    vector<int> a;
    vector<int> c;
    vector<int> g;
    vector<int> t;
};