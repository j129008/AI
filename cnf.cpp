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

    bool isAtom(){
         if(child.size() == 0) return true;
         else return false;
    }

    node tail(){
        return child.back();
    }

    int size(){
         return child.size();
    }

    void status(){
        if(child.size() == 0){
            if(atom == "")cout<<"null";
            else cout<<atom<<endl;
        }else{
            cout<<"[";
            for(int i=0; i<child.size(); i++){
                if(child[i].size() > 0){
                    child[i].status();
                }
                else cout<<" "<<child[i].atom<<" ";
            }
            cout<<"]";
        }
    }

    void clear(){
         atom = "";
         child.clear();
    }
};
node toParseTree(vector<string>);
node rmIff(node);
string postfixToInfix(node);


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
        printVec(tokens);
        postfix = toPostfix(tokens);
        printVec(postfix);
        tree = toParseTree(postfix);
        tree = rmIff(tree);
        cout<<postfixToInfix(tree)<<endl;
        tree.status();
        getchar();
    }

    return 0;
}

string postfixToInfix(node logic){
    string ans;
    if(logic.isAtom()) ans = logic.atom;
    else{
        string op = logic.tail().atom;
        if(op=="neg"){
            ans = "( neg " + postfixToInfix(logic[0]) + " )";
        }else{
            ans = "( " + postfixToInfix(logic[0]) + " " + op + " " + postfixToInfix(logic[1]) + " )";
        }
    }
    return ans;
}

node rmIff(node logic){
    node ans;
    if(logic.isAtom()) ans = logic;
    else{
        string op = logic.tail().atom;
        if(op =="neg"){
             node tree = rmIff(logic[0]);
             ans.push("neg");
             ans.push(tree);
        }else{
            node tree1 = rmIff(logic[0]);
            node tree2 = rmIff(logic[1]);
            if(op=="iff"){
                //ans = [ [rmIff(tree1), rmIff(tree2), 'imp'], [rmIff(tree2), rmIff(tree1), 'imp'], 'and' ]
                node leftImp;
                node rightImp;
                leftImp.push("imp");
                leftImp.push(rmIff(tree2));
                leftImp.push(rmIff(tree1));

                rightImp.push("imp");
                rightImp.push(rmIff(tree1));
                rightImp.push(rmIff(tree2));

                ans.push("and");
                ans.push(rightImp);
                ans.push(leftImp);
            }else{
                ans.push(op);
                ans.push(tree2);
                ans.push(tree1);
            }
        }
    }
    return ans;
}
node toParseTree(vector<string> postfix){
    node stack;
    string element;
    node action;
    while( postfix.size()>0 ){
        element = postfix[0];
        action.clear();
        postfix.erase(postfix.begin());
        if( ops.find(element) == ops.end() ){
            stack.push(element);
        }else if(element == "neg"){
            node operand = stack.pop();
            action.push(element);
            action.push(operand);
            stack.push(action);
        }else{
            node operand2 = stack.pop();
            node operand1 = stack.pop();
            action.push(element);
            action.push(operand2);
            action.push(operand1);
            stack.push(action);
        }
    }
    return stack[0];
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
