#include "Mgr.h"
#include "myUsage.h"
#include <cstdlib>

Pattern::Generator Gen(101011387);
MyUsage usg;

int main(int argc, char *argv[])
{
    if ( argc != 4 ) {
        cerr << "usage: ./lrg <io_info.txt> <iogen> <circuit.v>" << endl;
        exit(-1);
    }

    usg.reset();
    LogicRegression::GetMgr().ReadIOInfo(argv[1]);
    LogicRegression::GetMgr().ReadIOGen(argv[2]);
    LogicRegression::GetMgr().SetOutput(argv[3]);
    // LogicRegression::GetMgr().DetermineInitParam();
    LogicRegression::GetMgr().IncrementalSyn();
    // LogicRegression::GetMgr().GenPattern();
    LogicRegression::GetMgr().RunAbc();

    usg.report(1, 1);
    return 0;
}
