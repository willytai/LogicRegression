#ifndef __MGR_H__
#define __MGR_H__

#include "Variable.h"
#include "patternBank.h"
#include "RandPatGen.h"
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <bitset>

#ifdef PARALLEL
#include <omp.h>
#endif

#define MASK 0x1
#define MAX_ENUMERATE_VAR_NUM 10
#define MIN_ENUMERATE_VAR_NUM 6

using std::cout;
using std::endl;
using std::cerr;

namespace LogicRegression
{

class Mgr
{
public:
    Mgr() {}
    ~Mgr() {}

    static Mgr& get_instance() { static Mgr m; return m; }

    /***********/
    /* Top API */
    /***********/
    void ReadIOInfo        (char*);
    void ReadIOGen         (char*);
    void SetOutput         (char*);
    void GenPattern        ();
    void RunAbc            ();

    /*******************/
    /* Abc's procedure */
    /*******************/
    void InitABC        ();
    void ReadABC        (const std::string filename = "pat.blif");
    void SynthesisABC   ();
	void SimulateABC	(const std::string filename = "pat.pla");
    void TechMapABC     ();
    void EndABC         ();

    /********************/
    /* IO for gnenrator */
    /********************/
    // random generate
    void GenerateInputPattern(std::string filename = "in_pat.txt");
    void WritePattern        (const PatternBank&, std::string filename = "in_pat.txt") const;
    void ReadIORelation      (std::string filename = "io_rel.txt");
    void RunIOGen            () const;

    void FindDependentInput();
    // void CalInfoGain(const int, std::vector<std::pair<double, VariableID> >&);
    void refinePattern(PatternBank&, const std::vector<std::pair<double, VariableID> >&);

    // Generate PLA file for abc
    void GeneratePLA (std::string filename = "pat.pla");

    // Generate BLIF file for abc
    void GenerateBLIF(std::string filename = "pat.blif");
    void CollectOnSetPatterns(std::vector<std::string>&, const int&); // duplicates are removed here
    void Merge(std::string&, const std::string&);
    bool DiffByOne(const std::string&, const std::string&);

private:
    std::string            _benchmark;
    std::vector<Variable>  _input;
    std::vector<Variable>  _output;
    std::string            _iogen;
    int                    _numInput;
    int                    _numOutput;
    char*                  _verilog_output;

    std::vector<std::vector<VariableID> > _fanins; // _fanins[0] would be the fanins of output ID 0

    // the initial number of patterns to do synthesis
    int                    _initPatNum;

    // the end index of the patterns for synthesis _relation_in[id][0] ~ _relation_in[id][_syn_end-1]
    int                    _syn_end;

    std::vector<std::string> _relation_in;
    std::vector<std::string> _relation_out;
    int                      _numPat; // the total patterns in _relation_in/_relation_out

    std::map<std::string, VariableID>  _input_variable_name_id_map;
    std::map<std::string, VariableID>  _output_variable_name_id_map;

    // ABC frame work
    inline void AbcError(const char* command) {
        cerr << "Cannot execute command \"" << command << "\"" << endl;
        exit(-1);
    }
};

inline Mgr& GetMgr() { return Mgr::get_instance(); }

}

#endif /* __MGR_H__ */
