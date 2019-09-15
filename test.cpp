#include <iostream>
#include "linear/link.h"
#include <fstream>

int main() {
    std::ofstream fout("out", std::ios::out);
    std::ifstream fin("in", std::ios::in);
    DS::Link<int> link1;
    int n;
    fin >> n;
    for(int i = 0; i < n; ++i) {
        int x, val, pos;
        fin >> x;
        if(x == 0) {
            fin >> val;
            link1.push(val);
        }
        else if(x == 1) {
            fin >> pos;
            try {
                fout << link1.get(pos) << std::endl;   
            }
            catch(...) {
            }
        }
        else if(x == 2) {
            fin >> pos >> val;    
            link1.insert(val, pos);
        }
        else if(x == 3) {
            fin >> pos;    
            link1.remove(pos);
        }
    }
    fout.close();
    fin.close();
    return 0;
}