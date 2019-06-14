#include "Mgr.h"

// PLA file
// enumearted input vars
// patterns
// .
// .
// .

namespace LogicRegression
{

void Mgr::GeneratePLA(int PO_id, std::vector<std::pair<double, VariableID> >& info, std::string filename)
{
    cout << "[  Mgr  ] creating PLA file for pattern simulation" << endl;
    std::ofstream plaFile;
    plaFile.open(filename.c_str());
    for (int i = 0; i < (int)info.size(); ++i) {
        plaFile << info[i].second << ' ';
    }
    plaFile << endl;
    int count = 0;
    // only the batches for simulation
    for (int pat_id = _initPatNum; pat_id < _numPat; ++pat_id) {
        for (int input_id = 0; input_id < _numInput; ++input_id) {
            plaFile << _relation_in[input_id][pat_id];
        }
        plaFile << ' ';
        plaFile << _relation_out[PO_id][pat_id];
        plaFile << endl;
        ++count;
    }
    cout << "[  Mgr  ] total patterns = " << count << ", file written." << endl;
}

void Mgr::GeneratePLA(std::string filename)
{
/* Define the function which generate PLA file for abc from pattern */
    cout << "[  Mgr  ] creating PLA file simulation patterns" << endl;
    std::ofstream plaFile;
    plaFile.open(filename.c_str());
    int count = 0;
    const int& size = _numPat;
    // only the batches for simulation
    for (int batch = _initPatNum; batch < (int)_relation_in[0].size(); ++batch) {
        for (int shift = 0; shift < UnitPatSize; ++shift) {
            for (int input_id = 0; input_id < _numInput; ++input_id) {
                plaFile << ( ( _relation_in[input_id][batch] >> shift ) & MASK );
            }
            plaFile << ' ';
            for (int output_id = 0; output_id < _numOutput; ++output_id) {
                plaFile << ( ( _relation_out[output_id][batch] >> shift ) & MASK );
            }
            plaFile << endl;
            if ( ++count == size ) break;
        }
        if ( count == size ) break;
    }
    /* specify end of file */
    /* plaFile << ".e" << endl; */
    cout << "[  Mgr  ] creating PLA file done" << endl;
}
/* end of namespace */
}
