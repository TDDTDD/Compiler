//
// Created by hp on 2020/5/16.
//

#include "shared_base.h"

vector<Tnode> lexRes;
vector<string> wrong;
vector<Quaternary> qs;
void addWrong(int linenum,string value,string type){
    wrong.push_back("line "+to_string(linenum)+" : "+value+type);
}