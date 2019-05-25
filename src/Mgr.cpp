#include "Mgr.h"

namespace LogicRegression
{

void Mgr::GenerateInputPattern(std::string filename, int numPat) {
    std::ofstream file;
    file.open(filename.c_str());

    if ( !file.is_open() ) {
        cout << "[Mgr]    Cannot create input file: " << filename << " for " << _iogen << endl;
        exit(-1);
    }

    file << _numInput << ' ' << numPat << endl;
    for (int i = 0; i < (int)_input.size(); ++i) {
        file << _input[i]._name;
        if (i < (int)_input.size()-1) file << ' ';
    }
    file << endl;

    Pattern::Generator Gen(5); // fix random seed
    std::vector<Pat> patterns; patterns.resize(_numInput);
    int count = 0;
    while (count < numPat) {
        Gen(patterns);
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < _numInput; ++j) {
                file << (patterns[j] & MASK);
                patterns[j] = patterns[j] >> 1;
                if (j < _numInput-1) file << ' ';
            }
            file << endl;
        }
    count += 64;
    }
    file.close();
}

void Mgr::RunIOGen() {
    cout << "[Mgr]    Generating relation from " << _iogen << " ..." << endl;
    std::system(("./"+_iogen+" in_pat.txt io_rel.txt").c_str());
}

}
