#ifndef __MGR_H__
#define __MGR_H__

#include "Variable.h"
#include "RandPatGen.h"
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <iostream>

#define MASK 0x1

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

    // for parser
    void ReadIOInfo     (char*);
    void ReadIOGen      (char*);
    void ReadIORelation (std::string filename = "io_rel.txt");

    // IO for gnenrator
    void GenerateInputPattern(std::string filename = "in_pat.txt", int numPat = 64);

    void RunIOGen();

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
};

inline Mgr& GetMgr() { return Mgr::get_instance(); }

}

#endif /* __MGR_H__ */
