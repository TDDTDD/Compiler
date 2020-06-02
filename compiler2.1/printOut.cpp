//
// Created by hp on 2020/5/18.
//
#include<bits/stdc++.h>
#include "printOut.h"
#include "shared_base.h"
using namespace std;
extern vector<string> wrong;
extern vector<Quaternary> qs;//四元式数组

void printRes(){
    if((int)wrong.size()>0){
        ofstream eout("Error.txt");
        cout<<"compile fail!!!"<<endl;
        sort(wrong.begin(),wrong.end());
        for(int i=0;i<(int)wrong.size();++i){
            if(i>0){
                if(wrong[i]==wrong[i-1]) continue;//错误一样
                else{
                    cout<<wrong[i]<<endl;
                    eout<<wrong[i]<<endl;
                }
            }
            else{
                cout<<wrong[i]<<endl;
                eout<<wrong[i]<<endl;
            }
        }
    }
    else{
        cout<<"compile success!!!"<<endl;//编译成功输出中间代码
        for(int i=0;i<(int)qs.size();++i){
            cout<<setw(5)<<i<<" "<<"("<<setw(15)<<qs[i].op<<" ,"<<setw(10)
                <<qs[i].arg1<<" ,"<<setw(10)<<qs[i].arg2<<" ,"<<setw(10)<<qs[i].result<<")"<<endl;
        }
    }
}