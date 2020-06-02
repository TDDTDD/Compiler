//
// Created by hp on 2020/5/16.
//
#include<bits/stdc++.h>
using namespace std;
struct Tnode {
    int hsid;
    string fa;
};
map<string,vector<int> > v;      // v记录A->B|C|D 每一部分用一个数字哈希指代
map<string,vector<Tnode> > rev;   //rev记录有某个X的全部语法右部和对应的fa（规则左部
map<int,vector<string> > hs;     //每一个id代表一个文法 A->BC,即存储着BC
map<string,set<string> > Fir,Fol;//储存First和Follow
set<string> reservedWord,specialSymbol;//存储保留字和特殊符号
map<pair<string,string>,int >  Firhs,analysisTable;
//Firhs:存储由非终止符推导到终结符所对应的文法 A a (A->BC) ; analysisTable:非终止符到终止符的文法映射
vector<string> terminators,nonTerminators; //存储所有的终结符 和非终结符
map<string,bool> vis; //搜索标记
map<string,bool> over;//Fol求得标记
char str[10];
string start;
int id=0,hsid=0;
// @为空串 i为标识符
static void printSet(int id){//输出映射的具体文法
    for(auto s:hs[id]) cout<<s;
}
static void init() { //初始化读入保留字和特殊字符
    string s;
    ifstream fin1("reserve.txt");
    while(fin1>>s) reservedWord.insert(s);
    ifstream fin2("special.txt");
    while(fin2>>s) specialSymbol.insert(s);
}
static void clears(int num) { //清空读入的数组
    for(int i=0; i<=num; ++i) str[num]='\0';
    id=0;
}
static void readin() { //读入文法规则
    ifstream fin("grammarRule.txt");
    string fa,son;
    string line_tmp;
    string tmp;
    while(getline(fin, line_tmp)) {
//        cout << "--" << line_tmp << endl;
        stringstream ss(line_tmp);
        bool state=false;
        while(ss>>tmp){
            if(state==false){
                fa=tmp;
                state=true;
            }
            else hs[hsid].push_back(tmp);
        }
        v[fa].push_back(hsid);
        for(string ns:hs[hsid]) {
            rev[ns].push_back({hsid, fa}); //存在ns的语法右部和这条规则的左部
        }
        hsid++;
    }
}
static bool isTerminator(string ns) {//是否为终止符
    if(reservedWord.find(ns)!=reservedWord.end() || specialSymbol.find(ns)!=specialSymbol.end())	return true;
    else return false;
}
static void isLL1(string fa,string son,int nhsid){//判断是否在填表的时候发生冲突
    if(analysisTable.find(make_pair(fa,son))!=analysisTable.end() && analysisTable[make_pair(fa,son)]!=nhsid){
        cout<<fa<<"->"<<son<<"  发生冲突！"<<endl;
        printSet(nhsid);cout<<" | ";printSet(analysisTable[make_pair(fa,son)]);
        cout<<" 都可推导出"<<son<<endl;
    }
}
static void solveFir(string s) { //求s的First
    vis[s]=true;
    vector<int> sons=v[s];
    for(auto nhsid:sons) { //没一个id代表文法的一个部分 AB | CD | EF
        int num=0;
        for(auto ns:hs[nhsid]) {//文法每一部分的组成 A A' @
            num++;
            if(isTerminator(ns)) {//即直接就是终止符
                Fir[s].insert(ns);
                analysisTable[make_pair(s,ns)]=nhsid;
                isLL1(s,ns,nhsid);
                break;
            } else { //说明是非终止符（排除 @ 空串
                bool ff=0;
                if(ns=="@") {
                    ff=1;
                } else if(!Fir[ns].empty()) { //Fir已经求出
                    for(string c:Fir[ns]) {
                        if(c!="@") {
                            Fir[s].insert(c);//加入非空
                            isLL1(s,c,nhsid);
                            analysisTable[make_pair(s,c)]=nhsid;
                        }
                        if(c=="@") ff=1;
                    }
                    if(ff==0) break; //无空则结束
                } else if(vis[ns]==false) {//为true就没必要搜下去了，自己在自己，集合不变
                    solveFir(ns);
                    for(string c:Fir[ns]) {
                        if(c!="@") {
                            Fir[s].insert(c);
                            isLL1(s,c,nhsid);
                            analysisTable[make_pair(s,c)]=nhsid;
                        }
                        if(c=="@") ff=1;
                    }
                    if(ff==0) break;//无空则结束
                }
                if(num=(int)hs[nhsid].size() && ff==1) { //说明都含有空串
                    Fir[s].insert("@");
                    isLL1(s,"@",nhsid);
                    analysisTable[make_pair(s,"@")]=nhsid;
                }
            }
        }
    }
    vis[s]=false;
}
static void solveFol(string s) { //求s的Follow
    vis[s]=true;
    vector<Tnode> &svs = rev[s];
    for(int i=0; i<(int)svs.size(); ++i) { //有很多条含有这规则的文法
        int nhsid=svs[i].hsid;//对应的规则右部的映射
        string nfa=svs[i].fa; //对应的规则左部
        bool fd=false;
        for(int j=0; j<hs[nhsid].size(); ++j) {
            string fss=hs[nhsid][j];
            if(fd) { //说明s在前面出现了
                if(isTerminator(fss)) { //终结符
                    Fol[s].insert(fss);
                    if(j+2<(int)hs[nhsid].size() && hs[nhsid][j+1]==s) {//简单考虑文法右部出现两个一样的非终结符
                        j+=1; //A8 else A8 endif
                        continue;
                    }
                    break; //这条规则到此结束
                } else { //非终结符 ，将Fir加入到
                    bool flag=false;
                    for(auto firs:Fir[fss]) {
                        if(firs!="@") Fol[s].insert(firs);
                        if(firs=="@") {
                            flag=true;
                        }
                    }
                    if (flag==false) break;
                    //反之 说明出现了空串，则还需要继续往下
                    if(flag==true && j==hs[nhsid].size()-1) { //说明到结尾都有空串，直接加上Fol[fa]
                        if(over[nfa]) { //如果fa的Fol已经求得
                            for(auto fafol:Fol[nfa]) {
                                Fol[s].insert(fafol);
                            }
                        } else {
                            if(vis[nfa]) continue;//正在求fa的Fol 自己嵌套自己 集合并不会增加
                            else {
                                solveFol(nfa);//反之，就去求fa的Fol
                                for(auto fafol:Fol[nfa]) {
                                    Fol[s].insert(fafol);
                                }
                            }
                        }
                    }
                }
            }
            if(hs[nhsid][j]==s) {
                fd=true;
                if(j==hs[nhsid].size()-1) { //s出现在了规则的最后 ,直接加上Fol[fa]
                    if(over[nfa]) { //如果fa的Fol已经求得
                        for(auto fafol:Fol[nfa]) {
                            Fol[s].insert(fafol);
                        }
                    } else {
                        if(vis[nfa]) continue;//正在求fa的Fol 自己嵌套自己 集合并不会增加
                        else solveFol(nfa);//反之，就去求fa的Fol
                        for(auto fafol:Fol[nfa]) {
                            Fol[s].insert(fafol);
                        }
                    }
                }
            }


        }
    }
    over[s]=true; //s的Fol已经求得
    vis[s]=false;
}
static void runFirst() { //求全局First
    for(auto nfa:v) {
        solveFir(nfa.first);
    }
    //Test out First sets
    ofstream fout("First.txt");
    for(auto nfa:v) {
        fout<<nfa.first<<"的First集合为："<<endl;
        for(auto f:Fir[nfa.first]) {
            fout<<f<<" ";
        }
        fout<<endl;
    }
}
static void runFollow() { //求全局Follow
    Fol[start].insert("$"); //初始S带有$
    for(auto nfa:v) solveFol(nfa.first);
    //Test out Follow sets
    ofstream fout("Follow.txt");
    for(auto nfa:v) {
        fout<<nfa.first<<"的Follow集合为："<<endl;
        for(auto f:Fol[nfa.first]) {
            fout<<f<<" ";
        }
        fout<<endl;
    }
}
static void showAnalysisTableForMe(){ //展现预测分析表
    ofstream fout("analysisTableShowForMe.txt");
    for(auto nfir:Fir){
        fout<<nfir.first<<"的具体文法:"<<endl;
        for(auto t:nfir.second){
            int nhsid=analysisTable[make_pair(nfir.first,t)];
            fout<<t<<" ("<<nhsid<<": "<<nfir.first<<"->";
            for(auto s:hs[nhsid]) fout<<s;
            fout<<")"<<endl;
        }
    }
    fout<<setw(10)<<"Overword";
    for(auto word:terminators){
        fout<<setw(10)<<word;
    }
    fout<<endl;
    for(auto fa:nonTerminators){
        fout<<setw(10)<<fa;
        for(auto son:terminators){
            fout<<setw(10)<<analysisTable[make_pair(fa,son)];
        }
        fout<<endl;
    }
}
static void createAnalysisTable(){ //生成预测分析表
    for(auto s:reservedWord) terminators.push_back(s);
    for(auto s:specialSymbol){
        if(s=="@") continue;
        else	terminators.push_back(s);
    }
    terminators.push_back("$");
    for(auto nfir:Fir){
        string fa=nfir.first;
        nonTerminators.push_back(fa);
        if(analysisTable.find(make_pair(fa,"@"))!=analysisTable.end()){//说明可以推导出空串
            set<string> nFol=Fol[fa];
            for(auto ns:nFol){//即推导出空串的这条语法，可以推导出Follow[fa]
                isLL1(fa,ns,analysisTable[make_pair(fa,"@")]);
                analysisTable[make_pair(fa,ns)]=analysisTable[make_pair(fa,"@")];
            }
        }
    }
    for(auto fa:nonTerminators){
        for(auto son:terminators){
            if(analysisTable.find(make_pair(fa,son))==analysisTable.end()){
                analysisTable[make_pair(fa,son)]=-1;
            }
        }
    }

    ofstream fout("analysisTable.txt");
    for(auto s:terminators) fout<<s<<" ";//终结
    fout<<"---"<<endl;
    for(auto s:nonTerminators) fout<<s<<" ";//非终结
    fout<<"---"<<endl;
//	map<int,vector<string> > hs;     //每一个id代表一个文法 A->BC,即存储着BC
    for(auto it:hs){
        fout<<it.first<<endl;
        for(auto s:it.second){
            fout<<s<<" ";
        }
        fout<<"--"<<endl;
    }
    fout<<"---"<<endl;
    for(auto fa:nonTerminators){
        for(auto son:terminators){
            fout<<analysisTable[make_pair(fa,son)]<<" ";
        }
        fout<<endl;
    }
//    showAnalysisTableForMe();

}
void runAnalysisTable(string ns){
    start=ns;
    init();
    readin();
    runFirst();
    runFollow();
    createAnalysisTable();
}
