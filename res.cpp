#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <sstream>
using namespace std;

map<string, int> ops;
void init();

string replace(string&, const string&, const string&);
string intToStr (int);
vector<string> toToken(string);
vector<string> toPostfix(vector<string>);

struct node{
    string atom;
    string tag;
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

    node operator+ (node in){
        node ans;
        for( int i=0; i<in.size(); i++ ) ans.push(in[i]);
        for( int i=0; i<child.size(); i++ ) ans.push(child[i]);
        return ans;
    }

    bool operator== (node in){
        set<string> tmpSet1;
        set<string> tmpSet2;
        for(int i=0;i<child.size();i++){
            tmpSet1.insert(child[i].atom);
        }
        for(int i=0;i<in.size();i++){
            tmpSet2.insert(in[i].atom);
        }
        if(tmpSet1 == tmpSet2) return true;
        else return false;
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
node rmImp(node);
node rmNeg(node);
node distriOr(node, node);
node doCnf(node);
node doDisjunc(node);
node rmParen(node);
node rmDup(node);
bool isTrue(node);
node rmTrue(node);
void tagFacts(map<string,node>&, node, string);
string reverse(string);
bool doReso(node left, node right, node& out, string& del);

string postfixToInfix(node);
string cnfToStr(node);
string termToStr(node);
string findMapTerm(map<string, node> someMap, node term);


int main(int argc, char const* argv[]){
    vector<string> tokens;
    vector<string> postfix;
    node tree;
    string tag;
    static map<string, node> tagMap;
    static map<string, node> resMap;

    // file I/O & ops
    init();
    ifstream in(argv[1]);
    ofstream outputFile(argv[2]);

    string line;
    while(getline(in, line)){
        tree.clear();
        tokens = toToken(line);

        tag = tokens[0];
        replace(tag,":","");
        tokens.erase(tokens.begin());

        postfix = toPostfix(tokens);
        tree = toParseTree(postfix);
        tree = rmIff(tree);
        tree = rmImp(tree);
        tree = rmNeg(tree);
        tree = doCnf(tree);
        tree = rmParen(tree);
        tree = rmDup(tree);
        tree = rmTrue(tree);
        tagFacts(tagMap, tree, tag);
    }
    for(map<string,node>::iterator i=tagMap.begin(); i!=tagMap.end();i++){
        outputFile<<(*i).first<<": ";
        outputFile<<termToStr((*i).second)<<endl;
    }
    outputFile<<"=========================="<<endl;
    node newTerm;
    int newFact=0;
    int mapSize1=0, mapSize2=0;
    while(1){
        mapSize1 = tagMap.size();
        for(map<string,node>::iterator i=tagMap.begin(); i!=tagMap.end();i++){
            for(map<string,node>::iterator k=i; k!=tagMap.end();k++){
                node out;
                string del;
                if( doReso((*i).second, (*k).second, out, del) ){
                    if(findMapTerm(tagMap, out)!="") continue;
                    replace(del,"-","neg ");
                    outputFile<<(*i).first<<" + "<<(*k).first<<" (cancel "<<del<<")"<<endl;
                    outputFile<<"得到"<<endl;
                    tagMap["R"+intToStr(1000+ (newFact++))] = out;
                    outputFile<<termToStr(out)<<endl;
                    if(out.size()==0) goto end;
                    outputFile<<endl;
                }
            }
        }
        mapSize2 = tagMap.size();
        if(mapSize1 == mapSize2) goto end;
    }
end:
    return 0;
}

string findMapTerm(map<string, node> someMap, node term){
    string ans = "";
    for (map<string, node>::iterator it = someMap.begin(); it != someMap.end(); ++it )
        if ((*it).second == term)
            return (*it).first;
    return ans;
}

string termToStr(node term){
    if(term.size()==0) return "空集合";
    vector<string> ans;
    string ansStr;
    for(int i=0;i<term.size();i++){
        ans.push_back(term[i].atom);
        ans.push_back(" or ");
    }
    ans.pop_back();
    for(int i=0;i<ans.size();i++){
         ansStr+=ans[i];
    }
    replace(ansStr,"-","neg ");
    return ansStr;
}

bool doReso(node left, node right, node& out, string& del){
    int maxLen = left.size();
    if(right.size()>maxLen) maxLen = right.size();
    for(int i=0;i<left.size(); i++){
        for(int j=0;j<right.size();j++){
            if(left[i].atom == reverse(right[j].atom)){
                del = left[i].atom;
                left.child.erase(left.child.begin()+i);
                right.child.erase(right.child.begin()+j);
                out = left + right;
                if(out.size()>=maxLen) return false;
                set<string> tmpSet;
                set<string>::iterator it;
                for(int i=0; i<out.size(); i++){
                    tmpSet.insert(out[i].atom);
                }
                out.clear();
                for(it=tmpSet.begin();it!=tmpSet.end();it++){
                    out.push(*it);
                }
                return true;
            }
        }
    }
    return false;
}

string reverse(string element){
    string neg = "-";
    if(element[0] == neg[0]) replace(element,"-","");
    else element = "-" + element;
    return element;
}

void tagFacts(map<string, node> &tagMap, node tree, string tag){
    for(int i=0; i<tree.size();i++){ if(tree.size()==1){
        if(tag == "X"){
            tree[i][0].atom = reverse(tree[i][0].atom);
            tagMap["XX"] = tree[i];
        }else tagMap[tag] = tree[i];
    }else
        tagMap[tag+intToStr(i+1)] = tree[i];
    }
}

string intToStr ( int Number ){
    ostringstream ss;
    ss << Number;
    return ss.str();
}

string cnfToStr(node logic){
    string ans;
    vector<string> token;
    for(int i=0; i<logic.size(); i++){
        if(logic[i].size()>1)token.push_back("(");
        for(int j=0; j<logic[i].size(); j++){
            token.push_back(logic[i][j].atom);
            token.push_back(" or ");
        }
        token.pop_back();
        if(logic[i].size()>1)token.push_back(")");
        token.push_back(" and ");
    }
    if(token.size()==0) return ans;
    token.pop_back();
    for(int i=0; i<token.size(); i++){
         ans+=token[i];
    }
    replace(ans, "-", "neg ");
    return ans;

}

bool isTrue(node term){
    string op;
    string opposite;
    for(int i=0;i<term.size();i++){
        string sym = term[i].atom;
        op = sym[0];
        if(op == "-") opposite = sym.substr(1, sym.length()-1);
        else opposite = "-" + sym;
        for(int j=0;j<term.size();j++){
            string sym2 = term[j].atom;
            if(opposite == sym2) return true;
        }
    }
    return false;
}
node rmTrue(node logic){
    node ans;
    for(int i=0;i<logic.size();i++){
        if(!isTrue(logic[i])) ans.push(logic[i]);
    }
    return ans;
}

node rmDup(node logic){
    node ans;
    node term;
    set<string> sTerm;
    set<string>::iterator it;
    set < set<string> > sLogic;
    set < set<string> >::iterator lit;
    for( int i=0; i<logic.size(); i++ ){
        sTerm.clear();
        for( int j=0; j<logic[i].size(); j++ ){
            sTerm.insert(logic[i][j].atom);
        }
        sLogic.insert(sTerm);
    }
    for(lit=sLogic.begin(); lit!=sLogic.end(); lit++){
        term.clear();
        sTerm=*lit;
        for(it=sTerm.begin(); it!=sTerm.end(); it++){
            term.push(*it);
        }
        ans.push(term);
    }
    return ans;
}

node doDisjunc(node logic){
    node ans;
    if(logic.isAtom()) ans.push(logic);
    else{
        ans = doDisjunc(logic[0]) + doDisjunc(logic[1]);
    }
    return ans;
}

node rmParen(node logic){
    node ans;
    if(logic.isAtom()){
        node tmp;
        tmp.push(logic);
        ans.push(tmp);
    }else{
        string op = logic.tail().atom;
        if( op == "or" ){
            ans.push(doDisjunc(logic[0]) + doDisjunc(logic[1]));
        }else{
            ans = rmParen(logic[0]) + rmParen(logic[1]);
        }
    }
    return ans;
}

node distriOr(node p1, node p2){
    node ans;
    if( (p1.isAtom() == false) && ( p1.tail().atom == "and" ) ){
        ans.push("and");
        ans.push(distriOr(p1[1], p2));
        ans.push(distriOr(p1[0], p2));
    }else if( (p2.isAtom() == false) && ( p2.tail().atom == "and" ) ){
        ans.push("and");
        ans.push(distriOr(p1, p2[1]));
        ans.push(distriOr(p1, p2[0]));
    }else{
        ans.push("or");
        ans.push(p2);
        ans.push(p1);
    }
    return ans;
}

node doCnf(node logic){
    node ans;
    node tree1, tree2;
    if(logic.isAtom()) ans = logic;
    else{
        string op = logic.tail().atom;
        tree1 = doCnf(logic[0]);
        tree2 = doCnf(logic[1]);
        if( op == "and" ){
            ans.push("and");
            ans.push(tree2);
            ans.push(tree1);
        }else{
            ans = distriOr(tree1, tree2);
        }
    }
    return ans;
}

node rmNeg(node logic){
    node ans;
    node tree1, tree2;
    if(logic.isAtom()) ans = logic;
    else{
        string op = logic.tail().atom;
        if(op =="neg"){
            node arg = logic[0];
            if(arg.isAtom()){
                node negArg = arg;
                negArg.atom = "-" + arg.atom;
                ans = negArg;
            }else{
                string innerOp = arg.tail().atom;
                if(innerOp=="neg"){
                    ans = rmNeg(arg[0]);
                }else{
                    // demorgan
                    tree1.push("neg");
                    tree1.push(arg[0]);
                    tree2.push("neg");
                    tree2.push(arg[1]);
                    tree1 = rmNeg(tree1);
                    tree2 = rmNeg(tree2);
                    map<string,string> reverse;
                    reverse["and"] = "or";
                    reverse["or"] = "and";
                    ans.push(reverse[innerOp]);
                    ans.push(tree2);
                    ans.push(tree1);
                }
            }
        }else{
            tree1 = rmNeg(logic[0]);
            tree2 = rmNeg(logic[1]);
            ans.push(op);
            ans.push(tree2);
            ans.push(tree1);
        }
    }
    return ans;
}

node rmImp(node logic){
    node ans;
    if(logic.isAtom()) ans = logic;
    else{
        string op = logic.tail().atom;
        if(op =="neg"){
            node tree = rmImp(logic[0]);
            ans.push("neg");
            ans.push(tree);
        }else{
            node tree1 = rmImp(logic[0]);
            node tree2 = rmImp(logic[1]);
            if(op=="imp"){
                //ans = [ [tree1, 'neg'], tree2, 'or']
                node left;
                left.push("neg");
                left.push(tree1);
                ans.push("or");
                ans.push(tree2);
                ans.push(left);
            }else{
                ans.push(op);
                ans.push(tree2);
                ans.push(tree1);
            }
        }
    }
    return ans;
}

string postfixToInfix(node logic){
    string ans;
    if(logic.isAtom()) ans = logic.atom;
    else{
        string op = logic.tail().atom;
        if(op=="neg"){
            ans = "(neg " + postfixToInfix(logic[0]) + ")";
        }else{
            ans = "(" + postfixToInfix(logic[0]) + " " + op + " " + postfixToInfix(logic[1]) + ")";
        }
    }
    replace(ans, "-", "neg ");
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


vector<string> toToken(string line){
    replace(line, "(", " ( ");
    replace(line, ")", " ) ");
    replace(line, "  ", " ");
    vector<string> inputTokens;
    string token;
    istringstream iss(line);
    while(getline(iss, token,' ')){
        inputTokens.push_back(token);
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

string replace(string& str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
