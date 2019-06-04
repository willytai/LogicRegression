#include "Mgr.h"
#include <stdio.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
void   Abc_Start();
void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
typedef struct Abc_Frame_t_ Abc_Frame_t;

Abc_Frame_t * Abc_FrameGetGlobalFrame();
int    Cmd_CommandExecute( Abc_Frame_t * pAbc, const char * sCommand );

#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

// frame work variables
static Abc_Frame_t *pAbc;
static clock_t clkRead, clkStrash, clkSweep, clk;

namespace LogicRegression
{

void Mgr::RunABC(const std::string filename) {
    cout << endl;
    cout << "************************************************" << endl;
    cout << "*             ABC Frame Work Start             *" << endl;
    cout << "************************************************" << endl;

    // start the ABC frame work
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    clk  = clock();

    const char* command = ("read " + filename).c_str();
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    clkRead = clock() - clk;
    cout << "[ABC]    Reading: " << (float)(clkRead) / (float)(CLOCKS_PER_SEC) << " sec" << endl;;

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    
    command = "strash";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    clkStrash = clock() - clkRead;
    cout << "[ABC]    Strashing: " << (float)(clkStrash) / (float)(CLOCKS_PER_SEC) << " sec" << endl;;

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);

    cout << "************************************************" << endl;
    cout << "*             ABC Frame Work End               *" << endl;
    cout << "************************************************" << endl;
}

}






