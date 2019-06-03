#ifndef __MGR_H__
#define __MGR_H__

#include "Variable.h"
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
#define MAX_PATTERN_NUM 5000000

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

    /**********/
    /* Top API */
    /**********/
    void ReadIOInfo     (char*);
    void ReadIOGen      (char*);
    void GenPattern     ();

    /********************/
    /* IO for gnenrator */
    /********************/
    // random generate
    void GenerateInputPattern(std::string filename = "in_pat.txt", int numPat = 64);
    // write specified patterns to file
    void WritePattern(const std::vector<std::vector<Pat> >&, std::string filename = "in_pat.txt") const;
    void ReadIORelation (std::string filename = "io_rel.txt");
    void RunIOGen() const;

    void CalInfoGain(const int, std::vector<std::pair<double, VariableID> >&);
    void refinePattern(std::vector<std::vector<Pat> >&, const std::vector<std::pair<double, VariableID> >&);
	//finding identical output pattern and generate don't care term
	//TODO : change std::map into self defined hash map in Mgr.cpp
	void findingDCinput();

	//Generate PLA file for abc
	void GeneratePLA(std::string filename = "pat.pla");

private:
    std::vector<Variable>  _input;
    std::vector<Variable>  _output;
    std::string            _iogen;
    int                    _numInput;
    int                    _numOutput;

    std::vector<std::vector<Pat> > _relation_in;
    std::vector<std::vector<Pat> > _relation_out;

    std::map<std::string, VariableID>  _input_variable_name_id_map;
    std::map<std::string, VariableID>  _output_variable_name_id_map;
	//TODO : output2input_map should be a self defined hash map
	std::map<std::string, std::string> output2input_map;
};

inline Mgr& GetMgr() { return Mgr::get_instance(); }

}

#endif /* __MGR_H__ */
