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

// framework variables
Abc_Frame_t *pAbc;
clock_t clkElapse, clkLast;
const char* command;

namespace LogicRegression
{

void Mgr::RunAbc() {
    this->InitABC();
    this->ReadABC();
    this->SynthesisABC();
    this->TechMapABC();
    this->DumpVerilog();
    this->EndABC();
}

/* start ABC framework */
void Mgr::InitABC() {
    cout << endl;
    cout << "************************************************" << endl;
    cout << "*             ABC Framework Start              *" << endl;
    cout << "************************************************" << endl;

    // start the ABC frame work
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    clkLast  = clock();
}


/* read PLA file into ABC */
void Mgr::ReadABC(const std::string filename){
    /* read PLA file & convert it to AIG */
    command = ("read " + filename).c_str();
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] Reading: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl;
    cout << endl;

    command = "strash";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] Strashing: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl;

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
}


/* logic synthesis procedure */
void Mgr::SynthesisABC(){

    /* perform logic synthesis using aig */
    /* the synthesis precedure is introduced in paper "DAG-Aware AIG Rewriting" */
    clkLast = clock();
    cout << "[  ABC  ] Start synthesis" << endl;

    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] balance: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] rewrite: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);

    
    command = "refactor";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] refactor: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] balance: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] rewrite: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite -z";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] rewrite -z: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] balance: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "refactor -z";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] refactor -z: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite -z";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] rewrite -z: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] balance: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);

    cout << "[  ABC  ] End synthesis" << endl;
}

void Mgr::SimulateABC(std::string filename){
	cout << "[  ABC  ] Start simulation: " << endl;
	clkLast = clock();
    command = ("sim_aig " + filename).c_str();
    std::string test(command);
    if (Cmd_CommandExecute( pAbc, command )) {
        std::string fuck(command);
        assert(fuck == test);
        this->AbcError(command);
    }
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] simulation: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 
	cout << "[  ABC  ] End simulation" << endl;
}

/* perform technology mapping & ouput verilog file */
void Mgr::TechMapABC(){
    // Technology mapping
    cout << "[  ABC  ] Start Technology mapping: " << endl;
    clkLast = clock();
    command = "read_library gates/lib.sic";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);

    command = "map";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] Technology mapping: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << "sec" << endl;
    cout << "[  ABC  ] End Technology mapping" << endl;
}

void Mgr::DumpVerilog() {
    // Output Verilog
    command = ("write_verilog " + std::string(_verilog_output)).c_str();
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkElapse = clock() - clkLast;
    clkLast += clkElapse;
    cout << "[  ABC  ] write_verilog: " << (float)(clkElapse) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 
}

/* end ABC framework */
void Mgr::EndABC(){
    cout << "************************************************" << endl;
    cout << "*             ABC Framework End                *" << endl;
    cout << "************************************************" << endl;
    Abc_Stop();
}

}

