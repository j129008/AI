#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include "lib.h"
using namespace std;
using namespace boost;

map<string, int> ops;
vector<string> parseTree;
void init();


int main(int argc, char const* argv[]){
    init();
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    string line;
    while(getline(in, line)){
        replace(line, "(", " ( ");
        replace(line, ")", " ) ");
        char_separator<char> sp(" ");
        tokenizer< char_separator<char> > tokens(line, sp);
        BOOST_FOREACH (const string& t, tokens) {
            parseTree.push_back(t);
        }
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
