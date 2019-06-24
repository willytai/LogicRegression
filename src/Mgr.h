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
#define MAX_ENUMERATE_VAR_NUM 13

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
    void SimulateABC    (const std::string filename = "pat.pla");
    void TechMapABC     ();
    void DumpVerilog    ();
    void EndABC         ();

    /********************/
    /* IO for gnenrator */
    /********************/
    void GenerateTestPatterns();
    void GenerateInputPattern(std::string filename = "in_pat.txt");
    void WritePattern        (const std::vector<std::string>&, std::string filename = "in_pat.txt") const;
    void ReadIORelation      (std::string filename = "io_rel.txt", bool test = false);
    void ReadIORelation      (std::string filename, std::vector<std::string>&, std::vector<std::string>&);
    void RunIOGen            (std::string in_pat = "in_pat.txt", std::string io_rel = "io_rel.txt") const;

    void FindDependentInput();
    void EnumerateAndSimulate();
    void Enumerate(std::vector<std::string>&, int);
    void Simulate();

    // Generate PLA file for abc
    void GeneratePLA (std::string filename = "pat.pla");

    // Generate BLIF file for abc
    void GenerateBLIF(std::string filename = "pat.blif");
    void CollectOnSetPatterns(std::vector<std::string>&, const int&); // duplicates are removed here
    void Merge(std::string&, const std::string&);
    bool DiffByOne(const std::string&, const std::string&);

    // some useful method
    inline int count(const std::vector<bool> v) {
        int count = 0;
        for (const auto& val: v) {
            if (val) ++count;
        }
        return count;
    }

private:
    std::string            _benchmark;
    std::vector<Variable>  _input;
    std::vector<Variable>  _output;
    std::string            _iogen;
    int                    _numInput;
    int                    _numOutput;
    char*                  _verilog_output;

    std::vector<std::string> _relation_in;
    std::vector<std::string> _relation_out;
    int                      _numPat; // the total patterns in _relation_in/_relation_out

    // this contains patterns for testing (the reference patterns)
    std::vector<std::string> _test_in;
    std::vector<std::string> _test_out;

    std::map<std::string, VariableID>  _input_variable_name_id_map;
    std::map<std::string, VariableID>  _output_variable_name_id_map;

    // dependent inputs
    std::vector<std::vector<bool> >        _fanin_mask;

    // ABC frame work
    inline void AbcError(const char* command) {
        cerr << "Cannot execute command \"" << command << "\"" << endl;
        exit(-1);
    }
};

inline Mgr& GetMgr() { return Mgr::get_instance(); }

}

#endif /* __MGR_H__ */
