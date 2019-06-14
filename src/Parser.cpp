#include "Mgr.h"

namespace LogicRegression
{

void Mgr::SetOutput(char* filename) {
    _verilog_output = filename;
    cout << "[ Parser] output filename: " << _verilog_output << endl;
}


void Mgr::ReadIOInfo(char* filename) {
    std::ifstream file;
    file.open(filename);
    if ( !file.is_open() ) {
        cerr << "[ Parser] Cannot open file: " << filename << endl;
        exit(-1);
    }

    _benchmark = std::string(filename).substr(0, std::string(filename).find("io"));

    file >> _numInput >> _numOutput;
    while ( (int)_input.size() != _numInput ) {
        std::string buffer;
        file >> buffer;
        _input.push_back(Variable(buffer));
        _input_variable_name_id_map[buffer] = _input.size() - 1;
    }
    while ( (int)_output.size() != _numOutput ) {
        std::string buffer;
        file >> buffer;
        _output.push_back(Variable(buffer));
        _output_variable_name_id_map[buffer] = _output.size() - 1;
    }

    cout << "[ Parser] Input  Variables (" << _numInput << "):";
    // for (int i = 0; i < _numInput; ++i) cout << ' ' << _input[i]._name;
    cout << endl;
    cout << "[ Parser] Output Variables (" << _numOutput << "):";
    // for (int i = 0; i < _numOutput; ++i) cout << ' ' << _output[i]._name;
    cout << endl;

    _relation_in.resize(_numInput);
    _relation_out.resize(_numOutput);
}

void Mgr::ReadIOGen(char* filename) {
    _iogen = filename;
    cout << "[ Parser] IO Generator: " << _iogen << endl;
}

void Mgr::ReadIORelation(std::string filename) {

    cout << "[ Parser] Reading IO relation ..." << endl;

    _relation_in.clear();
    _relation_out.clear();
    _relation_in.resize(_numInput);
    _relation_out.resize(_numOutput);

    int numRelation;
    std::ifstream file;
    file.open(filename.c_str());
    file >> numRelation >> numRelation >> numRelation;
    for (int i = 0; i < _numInput; ++i) _relation_in[i].resize(numRelation);
    for (int i = 0; i < _numOutput; ++i) _relation_out[i].resize(numRelation);

    std::vector<std::string> variableNames;
    std::string buffer;
    while ( (int)variableNames.size() < (_numInput + _numOutput) ) {
        file >> buffer;
        variableNames.push_back(buffer);
    }

    int relCount = 0;
    std::string temp((_numInput + _numOutput), 'X');
    while (relCount < numRelation) {
        for (int i = 0; i < (int)temp.size(); ++i) {
            file >> temp[i];
        }
        for (int i = 0; i < (int)temp.size(); ++i) {
            std::map<std::string, VariableID>::iterator it = _input_variable_name_id_map.find(variableNames[i]);
            if (it == _input_variable_name_id_map.end()) {
                it = _output_variable_name_id_map.find(variableNames[i]);
                VariableID id = (*it).second;
                _relation_out[id][relCount] = temp[i];
            }
            else {
                VariableID id = (*it).second;
                _relation_in[id][relCount] = temp[i];
            }
        }
        temp.clear();
        temp.resize((_numInput + _numOutput), 'X');
        ++relCount;
    }

    _numPat = numRelation;
}

}
