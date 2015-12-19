#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
using namespace std;

map<string, int> ops;
void init();


int main(int argc, char const* argv[]){
    init();
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    string line;
    while(getline(in, line)){
        out<<line<<endl;
    }


    return 0;
}

void init(){
    ops["iff"] = 1;
    ops["imp"] = 2;
    ops["or"] = 3;
    ops["and"] = 4;
    ops["neg"] = 5;
    ops["("] = 0;
    ops[")"] = 6;
}
