#include "Mgr.h"

namespace LogicRegression
{

void Mgr::ReadIOInfo(char* filename) {
    std::ifstream file;
    file.open(filename);
    if ( !file.is_open() ) {
        cerr << "[Parser] Cannot open file: " << filename << endl;
        exit(-1);
    }

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

    cout << "[Parser] Input  Variables (" << _numInput << "):";
    for (int i = 0; i < _numInput; ++i) cout << ' ' << _input[i]._name;
    cout << endl;
    cout << "[Parser] Output Variables (" << _numOutput << "):";
    for (int i = 0; i < _numOutput; ++i) cout << ' ' << _output[i]._name;
    cout << endl;

    _relation_in.resize(_numInput);
    _relation_out.resize(_numOutput);
}

void Mgr::ReadIOGen(char* filename) {
    _iogen = filename;
    cout << "[Parser] IO Generator: " << _iogen << endl;
}

void Mgr::ReadIORelation(std::string filename) {
    int numRelation;
    std::ifstream file;
    file.open(filename.c_str());
    file >> numRelation >> numRelation >> numRelation;

    std::vector<std::string> variableNames;
    std::string buffer;
    while ( (int)variableNames.size() < (_numInput + _numOutput) ) {
        file >> buffer;
        variableNames.push_back(buffer);
    }

    int relCount = 0;
    std::vector<Pat> temp; temp.resize((_numInput + _numOutput), 0);
    while (relCount < numRelation) {
        for (int i = 0; i < (int)temp.size(); ++i) {
            temp[i] = temp[i] << 1;
            int value;
            file >> value;
            temp[i] += value;
        }
        if (++relCount % 64 == 0) {
            for (int i = 0; i < (int)temp.size(); ++i) {
                std::map<std::string, VariableID>::iterator it = _input_variable_name_id_map.find(variableNames[i]);
                if (it == _input_variable_name_id_map.end()) {
                    it = _output_variable_name_id_map.find(variableNames[i]);
                    VariableID id = (*it).second;
                    _relation_out[id].push_back(temp[i]);
                }
                else {
                    VariableID id = (*it).second;
                    _relation_in[id].push_back(temp[i]);
                }
            }
        }
    }

    /*
    for (int i = 0; i < _relation_in.size(); ++i) {
        cout << ((_relation_in[i][1]&2)>>1) << ' ';
    }
    cout << endl;
    for (int i = 0; i < _relation_out.size(); ++i) {
        cout << ((_relation_out[i][1]&2)>>1) << ' ';
    }
    cout << endl;
    */
}

}
