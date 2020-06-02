#include "lexicalAnalysis.h"
#include "grammarAnalysis.h"
#include "createAnalysisTable.h"
#include "shared_base.h"
#include "printOut.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
extern vector<Tnode> lexRes;
extern vector<string> wrong;
extern vector<Quaternary> qs;

int main() {
//    runAnalysisTable("S"); // 根据所给的文法 生成First 和 Follow 并判断符合LL1，生成预测分析表
    lexicalInit();
    lexicalGetToken("code5.txt");
    grammarInit();
    grammarPDA();
    printRes();

    return 0;
}