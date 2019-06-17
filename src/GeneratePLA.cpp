#include "Mgr.h"

extern Pattern::Generator Gen;

namespace LogicRegression
{

void Mgr::GeneratePLA(std::string filename)
{
    /* Define the function which generate PLA file for abc from pattern */
    /* Generates 100000 patterns for simulation, getting less than 10 errors would pass the baseline */
    cout << "[  Mgr  ] Random sampling 100000 patterns for testing ..." << endl;
    std::ofstream file; file.open("test_pat.txt");
    file << _numInput << ' ' << 100000 << endl;
    for (int i = 0; i < (int)_input.size(); ++i) {
        file << _input[i]._name;
        if (i < (int)_input.size()-1) file << ' ';
    }
    file << endl;
    std::string newPat(_numInput, 'X');
    for (int i = 0; i < 100000; ++i) {
        Gen(newPat);
        for (int bit = 0; bit < (int)newPat.length(); ++bit) {
            file << newPat[bit];
            if (bit < (int)newPat.length()-1) file << ' ';
        }
        file << endl;
    }
    file.close();

    this->RunIOGen("test_pat.txt", "test_rel.txt");
    this->ReadIORelation("test_rel.txt");

    cout << "[  Mgr  ] Writing to PLA file ... " << std::flush;

    file.open(filename);
    for (int pat_id = 0; pat_id < _numPat; ++pat_id) {
        std::string out;
        for (int PO_id = 0; PO_id < (int)_relation_out[pat_id].size(); ++PO_id) {
            if (!_few_fanin_mask[PO_id]) continue;
            out.push_back(_relation_out[pat_id][PO_id]);
        }
        file << _relation_in[pat_id] << ' ' << out << endl;
    }
    file.close();
 
    cout << "complete." << endl;
}

/* end of namespace */
}
