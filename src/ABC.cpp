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
static Abc_Frame_t *pAbc;
static clock_t clkRead, clkStrash, clk, clkCurrentProcedure, clkLastProcedure;
static const char* command;

namespace LogicRegression
{

void Mgr::RunAbc() {
    this->InitABC();
    this->ReadABC();
    this->SynthesisABC();
	this->SimulateABC();
    this->TechMapABC();
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
    clk  = clock();
}


/* read PLA file into ABC */
void Mgr::ReadABC(const std::string filename){
    /* read PLA file & convert it to AIG */
    command = ("read " + filename).c_str();
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    clkRead = clock() - clk;
    cout << "[ABC]    Reading: " << (float)(clkRead) / (float)(CLOCKS_PER_SEC) << " sec" << endl;
    cout << endl;

    command = "strash";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
    clkStrash = clock() - clkRead;
    cout << "[ABC]    Strashing: " << (float)(clkStrash) / (float)(CLOCKS_PER_SEC) << " sec" << endl;

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
}


/* logic synthesis procedure */
void Mgr::SynthesisABC(){

    /* perform logic synthesis using aig */
    /* the synthesis precedure is introduced in paper "DAG-Aware AIG Rewriting" */
    clkLastProcedure = clkStrash;
    cout << "[ABC]    Start synthesis" << endl;

    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    balance: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    rewrite: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);

    
    command = "refactor";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    refactor: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    balance: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    rewrite: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite -z";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    rewrite -z: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    balance: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "refactor -z";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    refactor -z: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "rewrite -z";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    rewrite -z: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);


    command = "balance";
    if (Cmd_CommandExecute(pAbc, command)) this->AbcError(command);
    clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    balance: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 

    command = "print_stats";
    if (Cmd_CommandExecute( pAbc, command)) this->AbcError(command);

    cout << "[ABC]    End synthesis" << endl;
}

void Mgr::SimulateABC(std::string filename){
	cout << "[ABC]    Start simulation: " << endl;
	clkLastProcedure = clock();
    command = ("sim_aig " + filename).c_str();
    if (Cmd_CommandExecute( pAbc, command )) this->AbcError(command);
	clkCurrentProcedure = clock() - clkLastProcedure;
    cout << "[ABC]    simulation: " << (float)(clkCurrentProcedure) / (float)(CLOCKS_PER_SEC) << " sec" << endl; 
	cout << "[ABC]    End simulation" << endl;
}

/* perform technology mapping & ouput verilog file */
void Mgr::TechMapABC(){

}

/* end ABC framework */
void Mgr::EndABC(){
    cout << "************************************************" << endl;
    cout << "*             ABC Framework End                *" << endl;
    cout << "************************************************" << endl;
    Abc_Stop();
}

}






