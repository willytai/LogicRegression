#include "Mgr.h"

namespace LogicRegression
{

void Mgr::GenerateBLIF(std::string filename) {
    cout << "[Mgr]    creating BLIF file from random pattern ..." << endl;
    std::ofstream blifFile;
    blifFile.open(filename.c_str());
    blifFile << ".model ICCAD " << _benchmark << endl;

    /* input variable names in order */
    blifFile << ".inputs";
    for (int i = 0; i < _numInput; ++i) {
        blifFile << ' ' << _input[i]._name;
    }
    blifFile << endl;

    /* output variable names in order */
    blifFile << ".outputs";
    for (int i = 0; i < _numOutput; ++i) {
        blifFile << ' ' << _output[i]._name;
    }
    blifFile << endl;

    for (int i = 0; i < _numOutput; ++i) {
        std::vector<std::string> patterns;
        this->CollectOnSetPatterns(patterns, i);
    }
}

void Mgr::CollectOnSetPatterns(std::vector<std::string>& patterns, const int& PO_id) {
    cout << "[Mgr]    Collecting and merging onset patterns for " << _output[PO_id]._name << " ..." << endl;

    // collect patterns
    int count = 0;
    for (int batch = 0; batch < (int)_relation_out[PO_id].size(); ++batch) {
        for (int shift = 0; shift < UnitPatSize; ++shift) {
            if ( !( ( _relation_out[PO_id][batch] >> shift ) & MASK ) ) {
                ++count;
                continue;
            }
            std::string temp(_numInput, '-');
            for (int i = 0; i < _numInput; ++i) {
                if ( ( _relation_in[i][batch] >> shift ) & MASK ) temp[i] = '1';
                else temp[i] = '0';
            }
            patterns.push_back(temp);
            if ( ++count == _numPat ) break;
        }
        if ( count == _numPat ) break;
    }

    // merge patterns that only differ by 1 bit
    
}

}
