// writing on a text file
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include "functions.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    string line;     //read in
    string ifilename;//the gene
    string seedfile; //the seed list
    int f;           //Frequency minimum
    string ofilename;//output file
    
    int seedlimit = 1;
    
    if (argc < 5) {
        cout << "Please enter in the format ./phRAIDER gene_file seed_file frequency out_file"<<endl;
    } else {
        ifilename = argv[1];
        seedfile = argv[2];
        f = atoi(argv[3]);
        ofilename = argv[4];
    }
    
    vector<string> seed;


//    cout<<"before seedfile"<<endl;

    ifstream sfile(seedfile.c_str());
    
    if (sfile.is_open()) {
        while (getline(sfile, line))
        {
            seed.push_back(line);
        }
    }
    else
    {
        cout<< "couldn't find seed file"<<endl;
        return 0;
    }
    ifstream ifile(ifilename.c_str());

    map<string, int> keyH;
    vector<string> primaryKey;
    map<int, vector<int> > H;
    map<int, int > H_family;

    int s = seed.at(0).length();
    int loc = 0;


    vector<Family> F;

    queue<int> Q;
//    cout<<"before ifile"<<endl;    
    string prev = "";
    string both = "";
    int offset = 0;
    if (ifile.is_open()) {
        getline(ifile, line);
        while (getline(ifile, line)) {
            if (prev != "") {
                both = prev + line;
                for (int i = 0; i < offset; i++) {
                    if (!Q.empty() && i + loc - offset - F[Q.front()].last_seen > s) {
                        int d_f = Q.front();
                        Q.pop();
                        F[d_f].last_seen = -1;
                        clean_up(H_family, H, F, d_f, s);
                    }
                    string key = "";
                    for (int j = 0; j < s; j++) {
                        key += both[j + i];
                    }    
                    if (keyH.find(key) == keyH.end() )
                    {
                        assignSeed(seed, keyH, primaryKey, key, s);
                    }
                    
                    advanceQueue(Q, F, H_family, H, keyH[key], i + loc - offset, s);
                }
            } else offset = line.length();
            prev = line;
            loc += offset;
        }
        ifile.close();
        for (int i = offset; i < both.length() - s; i++) {
            if (!Q.empty() && i + loc - offset * 2 - F[Q.front()].last_seen > s) {
                int d_f = Q.front();
                Q.pop();
                F[d_f].last_seen = -1;
                clean_up(H_family, H, F, d_f, s);
            }
            string key = "";
            for (int j = 0; j < s; j++) {
                key += both[j + i];
            }
            if (keyH.find(key) == keyH.end() )
            {
                assignSeed(seed, keyH, primaryKey, key,s);
            }
            advanceQueue(Q, F, H_family, H, keyH[key], i + loc - offset * 2, s);
        }
        for (int f = Q.front(); !Q.empty(); f = Q.front()) {
            Q.pop();
            clean_up(H_family, H, F, f, s);
        }
    } else cout << "Unable to open file" << endl;


    ofstream ofile;
    ofile.open(ofilename.c_str());
    if (ofile.is_open()) {
        ofile << "# fam\tele\tstart\tend" << endl;
        int e = 0;
        for (int k = 0; k < F.size(); k++) {
            vector<int> currentfront = H.at(F[k].lmer_list.front());
            vector<int> currentback = H.at(F[k].lmer_list.back());
            cout << k << " " << F[k].lmer_list.size() << " " << currentfront.size() << " " << (currentback.size() == currentfront.size());
            for (int i = 0; i < F[k].lmer_list.size(); i++)
                cout << ", " << F[k].lmer_list[i];
            cout << endl;
            if (currentfront.size() > f)
                for (int i = 0; i < currentfront.size(); i++) {
                    ofile << k << "\t" << ++e << "\t" << currentfront[i] << "\t" << (currentback[i] + s) << endl;

                }
        }

        ofile.close();
    } else cout << "Unable to open file";
}