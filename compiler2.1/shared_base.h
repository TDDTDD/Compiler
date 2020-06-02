#ifndef COMPILE_SHARED_BASE_H
#define COMPILE_SHARED_BASE_H

#include <string>
#include <vector>
using namespace std;
struct Tnode{
    int id;
    string type;
    string value;
};
struct Quaternary{//四元式
    string op,arg1,arg2,result;
};
void addWrong(int linenum,string value,string type);
#endif //COMPILE_SHARED_BASE_H
