#include "Mgr.h"
#include "myUsage.h"
#include <cstdlib>

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
    LogicRegression::GetMgr().GenPattern();
	LogicRegression::GetMgr().findingDCinput();
	LogicRegression::GetMgr().GeneratePLA();

    usg.report(1, 1);
    return 0;
}
