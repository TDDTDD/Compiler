//
// Created by hp on 2020/5/16.
//

#include<bits/stdc++.h>
#include "grammarAnalysis.h"
#include "shared_base.h"
using namespace std;
extern vector<Tnode> lexRes;
extern vector<string> wrong;
extern vector<Quaternary> qs;//四元式数组


static map<int,vector<string> > grammarHash;
static map<pair<string,string>,int >  analysisTable;
static vector<string> terminators,nonTerminators;
static int hsid=0;

stack<string> ps;//分析栈
stack<string> tys;//类型栈
stack<string> semantic_stk;//语义栈

string OP; //操作符号
int RSID=0;//操作序号
int QID=0;//四元式序号

static bool isTerminator(string s){
    if(find(terminators.begin(),terminators.end(),s)!=terminators.end()){
        return true;
    }else return false;
}
void grammarInit(){
    //读入预测分析表
    ifstream fin("analysisTableExtend.txt");
    string s;
    while(fin>>s){
        if(s=="---") break;
        terminators.push_back(s);
    }
    while(fin>>s){
        if(s=="---") break;
        nonTerminators.push_back(s);
    }
    int id;
    bool state=false;
    while(fin>>s){
        if(s=="---") break;
        else if(s=="--") state=false;
        else{
            if(state==false){
                id=atoi(s.c_str());
                state=true;
            }
            else grammarHash[id].push_back(s);
        }
    }
    for(auto fa:nonTerminators){
        for(auto son:terminators){
            fin>>id;
            analysisTable[make_pair(fa,son)]=id;
        }
    }

}
static int cnotAnalysis(string state,string type){
    if(!isTerminator(state) && analysisTable[make_pair(state,type)]==-1) return -1;
    else if(!isTerminator(state) && analysisTable[make_pair(state,type)]!=-1) return 1;
    else if(isTerminator(state) && state!=type) return -2;
    else if(isTerminator(state) && state==type) return 2;
}
static void solveType(int lexId){
    string type1=tys.top();
    tys.pop();
    string type2=tys.top();
    tys.pop();
    if(type1 == type2){
        tys.push(type1);
    }
    else{
        addWrong(lexRes[lexId].id,"$$"," operation type does not match");
        tys.push(type1);
    }
}
static void gao(int lexId){//生成算数运算的四元式
    if(semantic_stk.size()<2) return ;
    string arg1=semantic_stk.top();
    semantic_stk.pop();
    string arg2=semantic_stk.top();
    semantic_stk.pop();
    string nRSid="T"+to_string(RSID);
    Quaternary nq={OP,arg2,arg1,nRSid};
    qs.push_back(nq);
    semantic_stk.push(nRSid);
    //生成新的四元式压栈  新的运算结果压栈
    solveType(lexId);//
    RSID++;
    QID++;
}
static void action(string na,string value,int lexId){
    ps.pop();
    if(na=="#Add_Sub" || na=="#Mul_Div"){//暂无操作

    }
    if(na=="#Add"){//生成运算四元式
        OP="+";
        gao(lexId);
    }
    else if(na=="#Sub"){
        OP="-";
        gao(lexId);
    }
    else if(na=="#Mul"){
        OP="*";
        gao(lexId);
    }
    else if(na=="#Div"){
        OP="/";
        gao(lexId);
    }
    else if(na=="#Tran_A1C1"){

    }
    else if(na=="#Ass_C1"){
        semantic_stk.push(value);
        if(value.find(".")!=value.npos){//为int
            tys.push("Double");
        }
        else{//为double
            tys.push("Int");
        }
    }
    else if(na=="#Ass_A2"){
        semantic_stk.push(value);
        tys.push("null");
    }
    else if(na=="#Eq"){//生成赋值的四元式
        string arg1=semantic_stk.top();// T*
        semantic_stk.pop();
        string arg2=semantic_stk.top();// i
        semantic_stk.pop();
        Quaternary nq={"=",arg1,"/",arg2};
        qs.push_back(nq);
        semantic_stk.push(arg2);
        //生成新的四元式压栈  新的运算结果压栈
//        solveType(lexId);//
        QID++;
    }
    else if(na=="#Comp"){ //生成bool表达式
//		showSemantic(); //
        string arg1=semantic_stk.top();
        semantic_stk.pop();
        string arg2=semantic_stk.top();
        semantic_stk.pop();
        string nRSid="T"+to_string(RSID);
        Quaternary nq={OP,arg2,arg1,nRSid};
        qs.push_back(nq);
        semantic_stk.push(nRSid);
        //生成新的四元式压栈  新的运算结果压栈
//        solveType(lexId);//
        RSID++;
        QID++;
    }
    else if(na=="#Comp_<"){
        OP="<";
    }
    else if(na=="#Comp_>"){
        OP=">";
    }
    else if(na=="#Comp_>="){
        OP=">=";
    }
    else if(na=="#Comp_=="){
        OP="==";
    }
    else if(na=="#Comp_!="){
        OP="!=";
    }
    else if(na=="#Comp_or"){
        OP="or";
    }
    else if(na=="#Comp_and"){
        OP="and";
    }
    else if(na=="#Int"){

    }
    else if(na=="#Double"){

    }
    else if(na=="#Repeat"){
        Quaternary nq={"Repeat_Start","/","/",to_string(QID+1)};
        qs.push_back(nq);
        QID++;
    }
    else if(na=="#Repeat_RJ"){
        Quaternary nq={"Repeat_False",semantic_stk.top(),"/","%"};
        for(int i=(int)qs.size()-1;i>=0;--i){
            if(qs[i].op=="Repeat_Start"){
                nq.result=to_string(i);
                break;
            }
        }
        qs.push_back(nq);
        QID++;
    }
    else if(na=="#If_FJ"){//生成IF_false 的跳转四元式
        Quaternary nq={"If_False_JUMP",semantic_stk.top(),"/","/"};
        qs.push_back(nq);
        QID++;
    }
    else if(na=="#If_FJ_BackPatch"){ //回填If false的跳转
        for(int i=(int)qs.size()-1;i>=0;--i){
            if(qs[i].op=="If_False_JUMP"){
                qs[i].result=to_string(QID+1);
                break;
            }
        }
    }
    else if(na=="#If_RJ"){
        Quaternary nq={"If_True_JUMP","/","/","/"};
        qs.push_back(nq);
        QID++;
    }
    else if(na=="#If_RJ_BackPatch"){//回填If true的跳转
        for(int i=(int)qs.size()-1;i>=0;--i){
            if(qs[i].op=="If_True_JUMP"){
                qs[i].result=to_string(QID);
                break;
            }
        }
    }

}
void grammarPDA(){
    ps.push("$");ps.push("S");
    lexRes.push_back({(int)lexRes.size(),"$","$"});
    int lexId=0;
    while(!ps.empty()) {
        string ntop = ps.top();
//        cout<<"match "<<ps.top()<<" "<<lexRes[lexId].type<<endl;
        if (ntop == "$" && lexRes[lexId].type == "$") {
            return;
        } else if (ntop[0] == '#') {//动作符号
            action(ntop, lexRes[lexId].value,lexId);
        } else if (isTerminator(ntop)) {//是终止符
            if (ntop == lexRes[lexId].type) {
                ps.pop();
                lexId++;
            }
            else {//终止符不匹配
                addWrong(lexRes[lexId].id, "Cannot match ", lexRes[lexId].value);
                ps.pop();//分析栈pop掉无法匹配的元素 继续判错
                return;
            }
        } else {//是非终止符
            if (analysisTable[make_pair(ps.top(), lexRes[lexId].type)] != -1) {//可推导出
                int nhsid = analysisTable[make_pair(ps.top(), lexRes[lexId].type)];
                if (ps.top() != "$") ps.pop();
                int i = grammarHash[nhsid].size() - 1;
                for (; i >= 0; --i) { //倒着加入栈
                    if (grammarHash[nhsid][i] == "@") {
                        continue;//空串不加入栈
                    }
                    ps.push(grammarHash[nhsid][i]);
                }
            } else {
                addWrong(lexRes[lexId].id, "Cannot match ", lexRes[lexId].value);
                ps.pop();//分析栈pop掉无法匹配的元素 继续判错
                return;
            }

        }
    }
}

