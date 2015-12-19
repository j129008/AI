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
void init();

template<typename T>
void printVec(vector<T>);
vector<string> toToken(string);
vector<string> toPostfix(vector<string>);


int main(int argc, char const* argv[]){
    vector<string> tokens;
    vector<string> postfix;

    // file I/O & ops
    init();
    ifstream in(argv[1]);
    ofstream out(argv[2]);

    string line;
    while(getline(in, line)){
        tokens = toToken(line);
        postfix = toPostfix(tokens);
    }
    printVec(postfix);

    return 0;
}

template<typename T>
void printVec(vector<T> vec){
    BOOST_FOREACH (const T& ele, vec) {
        cout<<ele<<" ";
    }
    cout<<endl;
}

vector<string> toToken(string line){
    replace(line, "(", " ( ");
    replace(line, ")", " ) ");
    vector<string> inputTokens;
    char_separator<char> sp(" ");
    tokenizer< char_separator<char> > tokens(line, sp);
    BOOST_FOREACH (const string& t, tokens) {
        inputTokens.push_back(t);
    }
    return inputTokens;
}

vector<string> toPostfix(vector<string> tokens){
    while(tokens.size()){
        string token = tokens[0];
        tokens.erase(tokens.begin());
        //if(token == "("){

        //}
    }
    return tokens;
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
