//
// Created by hp on 2020/5/16.
//
#include<bits/stdc++.h>
#include "lexicalAnalysis.h"
#include "shared_base.h"
#define pb push_back
using namespace std;
const int N=1e3+5;

extern vector<Tnode> lexRes;
extern vector<string> wrong;

static set<string> reservedWord,specialSymbol;//存储保留字和特殊符号
static int linenum=1,id=0;
static string line;
static char s[N],c;

//--------------------------------------------------------------------

void lexicalInit(){//初始化标识符和保留字
    string s;
    ifstream fin1("reserve.txt");
    while(fin1>>s) reservedWord.insert(s);
    ifstream fin2("special.txt");
    while(fin2>>s) specialSymbol.insert(s);
}
static void printOut(){//输出
    ofstream tout("lexicalRes.txt");
    for(auto it:lexRes){
        tout<<it.id<<" "<<it.type<<" "<<it.value<<endl;
    }
}
static bool isSplitter(char c){//检查是否是分隔符
    string ns;ns=c;
    return (c==' ' || c=='\t' || c=='\n' || c==':' ||  c==EOF || !(specialSymbol.find(ns)==specialSymbol.end()));
}
static void clears(int num){
    for(int i=0;i<=num;++i) s[num]='\0';
    id=0;
}
void lexicalGetToken(string url){//用于词法分析
    freopen(url.c_str(),"r",stdin);
    c=getchar();
    while(c!=EOF){
        if(c==' ' || c=='\n' || c=='\t'){//空格换行直接过滤
            if(c=='\n'){//读到换行 行号++，并输出上一行
                linenum++;
            }
            else  line+=c;
            c=getchar();
        }
        else{
            string str;
            if((c>='a' && c<='z') || (c>='A' && c<='Z')){//判断字符串或保留字
                s[id++]=c;
                int state=1;
                while(!isSplitter((c=getchar()))) {
                    s[id++]=c;
                    if((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_') continue;
                    else state=5;
                }
                s[id]='\0',str=s;
                if(state==1){
                    if(reservedWord.find(str)!=reservedWord.end()){
                    //    lexRes.pb({linenum,"reservedWord",str});
                        lexRes.pb({linenum,str,str});
                    }
                    else{
                        lexRes.pb({linenum,"identifier",str});
                    }
                }
                else{ //出现错误！！！
                    addWrong(linenum,str," is a wrong word");
//                    wrong.push_back("line "+to_string(linenum)+"："+str+" is a wrong word");
                    lexRes.pb({linenum,"identifier",str});//将错误的词 记作标识符继续分析
                }
            }
            else if(c>='0' && c<='9'){//判断数字
                s[id++]=c;
                int state=2;//2:整型 3：浮点型 5：错误
                while(!isSplitter((c=getchar()))){
                    s[id++]=c;
                    if(c=='.' && state==2) state=3;
                    else if((c=='.' && state==3) || !((c>='0' && c<='9') || c=='.')) state=5;
                }
                s[id]='\0',str=s;
                if(state==2 || state==3){
                    lexRes.pb({linenum,"number",str});
                }
                else{//出现错误！！!
                    addWrong(linenum,str," is a wrong word");
                    lexRes.pb({linenum,"identifier",str});//将错误的词 记作标识符继续分析
//                    lexRes.pb({linenum,"wrong",str+" is a wrong number!"});
                }
            }
            else{//判断特殊字符
                s[id++]=c;
//                cout<<"special "<<c<<endl;
                if(c!='<' && c!='>' && c!='=' && c!='!'){
                    s[id]='\0',str=s;
                    if(specialSymbol.find(str)!=specialSymbol.end()){
                        lexRes.pb({linenum,str,str});
                    }
                    else {//出现错误！！！
                        addWrong(linenum,str," is a wrong character");
                        lexRes.pb({linenum,"identifier",str});//将错误的词 记作标识符继续分析
                    //    lexRes.pb({linenum,"wrong",str+" is a wrong character!"});
                    }
                    c=getchar();
                }
                else{
                    if(c=='<' || c=='>' || c=='='){
                        if((c=getchar())=='='){
                            s[id++]=c,s[id]='\0',str=s;
                            lexRes.pb({linenum,str,str});
                            c=getchar();
                        }
                        else{
                            s[id]='\0',str=s;
                            lexRes.pb({linenum,str,str});
                        }
                    }
                    else if(c=='!'){
                        if((c=getchar())=='='){
                            s[id++]=c,s[id]='\0',str=s;
                            lexRes.pb({linenum,str,str});
                            c=getchar();
                        }
                        else{
                            s[id]='\0',str=s;
                            addWrong(linenum,str," is a wrong character");
                            lexRes.pb({linenum,"identifier",str});//将错误的词 记作标识符继续分析
                        }
                    }
                    else{
                        s[id]='\0',str=s;
                        addWrong(linenum,str," is a wrong character");
                        lexRes.pb({linenum,"identifier",str});//将错误的词 记作标识符继续分析
                    }

                }

            }
            line+=str;
            clears(id);
        }
    }
    printOut();
}



