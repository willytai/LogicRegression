#include "Mgr.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
    if ( argc != 4 ) {
        cerr << "usage: ./lrg <io_info.txt> <iogen> <circuit.v>" << endl;
        exit(-1);
    }

    LogicRegression::GetMgr().ReadIOInfo(argv[1]);
    LogicRegression::GetMgr().ReadIOGen(argv[2]);
    LogicRegression::GetMgr().GenerateInputPattern("in_pat.txt", 64);
    LogicRegression::GetMgr().RunIOGen();
    LogicRegression::GetMgr().ReadIORelation();

    return 0;
}
