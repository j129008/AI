#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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

struct node{
    string atom;
    vector<node> child;

    node(string x){atom = x;};
    node(){};

    void push(node x){
        child.insert(child.begin(), x);
    }

    void push(string x){
        node tmp;
        tmp.atom = x;
        child.insert(child.begin(), tmp);
    }

    node pop(){
        node tmp;
        tmp.atom = child[0].atom;
        tmp.child = child[0].child;
        child.erase(child.begin());
        return tmp;
    }

    node& operator [](int i){
        return child[i];
    }

    int size(){
         return child.size();
    }

    void status(){
        if(child.size() == 0) cout<<atom<<endl;
        else{
            cout<<"[";
            for(int i=0; i<child.size(); i++){
                if(child[i].size() > 0) cout<<" list ";
                else cout<<" "<<child[i].atom<<" ";
            }
            cout<<"]"<<endl;
        }
    }

    void clear(){
         atom = "";
         child.clear();
    }
};


int main(int argc, char const* argv[]){
    vector<string> tokens;
    vector<string> postfix;
    node tree;

    // file I/O & ops
    init();
    ifstream in(argv[1]);
    ofstream out(argv[2]);

    string line;
    while(getline(in, line)){
        tree.clear();
        tokens = toToken(line);
        postfix = toPostfix(tokens);
        printVec(postfix);
        node test("a");
        test.status();
        getchar();
    }

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
    vector<string> opStack;
    vector<string> postfix;
    while(tokens.size()){
        string token = tokens[0];
        tokens.erase(tokens.begin());
        if(token == "("){
            opStack.insert(opStack.begin(), "(");
        }else if(token == ")"){
            while(opStack[0] != "("){
                 postfix.push_back(opStack[0]);
                 opStack.erase(opStack.begin());
            }
            opStack.erase(opStack.begin());
        }else if(ops.find(token) != ops.end()){
            if(opStack.size() == 0){
                 opStack.insert(opStack.begin(), token);
                 continue;
            }
            while(ops[token] <= ops[opStack[0]]){
                postfix.push_back(opStack[0]);
                opStack.erase(opStack.begin());
                if(opStack.size() == 0) break;
            }
            opStack.insert(opStack.begin(), token);
        }else{
             postfix.push_back(token);
        }
    }
    while(opStack.size()>0){
        postfix.push_back(opStack[0]);
        opStack.erase(opStack.begin());
    }
    return postfix;
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
