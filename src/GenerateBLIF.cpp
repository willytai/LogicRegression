#include "Mgr.h"

namespace LogicRegression
{

void Mgr::GenerateBLIF(std::string filename) {
    cout << "[  Mgr  ] creating BLIF file ..." << endl;
    std::ofstream blifFile;
    blifFile.open(filename.c_str());
    blifFile << ".model top" << endl;

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
        char refset = (_onset_mask[i] ? '1' : '0');
        std::vector<std::string> patterns;
        this->CollectPatterns(patterns, i);
        if (patterns.size()) {
            blifFile << ".names";
            for (int j = 0; j < _numInput; ++j) blifFile << ' ' << _input[j]._name;
            blifFile << ' ' << _output[i]._name << endl;
            for (int j = 0; j < (int)patterns.size(); ++j) blifFile << patterns[j] << ' ' << refset << endl;
        }
        else {
            blifFile << ".names " << _output[i]._name << endl;
            if (refset == '1') blifFile << '1' << endl;
            cout << "[  BLIF ] Warning: " << _output[i]._name << " specified as constant " << refset << endl;
        }
    }
    blifFile << ".end" << endl;
    cout << "[  Mgr  ] Finished writing BLIF file." << endl;
}

void Mgr::CollectPatterns(std::vector<std::string>& patterns, const int& PO_id) {

    if (this->count(_fanin_mask[PO_id]) == 0) return;

    char refset = (_onset_mask[PO_id] ? '1' : '0');

    // collect patterns
    for (int pat_id = 0; pat_id < (int)_relation_in.size(); ++pat_id) {
        if (_relation_out[pat_id][PO_id] != refset) continue;
        else patterns.push_back(_relation_in[pat_id]);
    }
    // collect the extra patterns if it is the too many inputs' output
    if (this->count(_fanin_mask[PO_id]) > MAX_ENUMERATE_VAR_NUM) {
        for (int pat_id = 0; pat_id < (int)_extra_in.size(); ++pat_id) {
            if (_extra_out[pat_id][PO_id] != refset) continue;
            else patterns.push_back(_extra_in[pat_id]);
        }
    }
    for (int pat_id = 0; pat_id < (int)patterns.size(); ++pat_id) {
        for (int bit = 0; bit < (int)patterns[pat_id].size(); ++bit) {
            if (!_fanin_mask[PO_id][bit]) patterns[pat_id][bit] = '-';
        }
    }

    /* this seems too costly by now
    // merge patterns that only differ by 1 bit
    // repeat until no merges can be found
    while (true) {
        int test = 0;
        for (int i = 0; i < (int)patterns.size()-1; ++i) {
            for (int j = i+1; j < (int)patterns.size(); ++j) {
                if (this->DiffByOne(patterns[i], patterns[j])) {
                    this->Merge(patterns[i], patterns[j]);
                    std::swap(patterns[j], patterns.back());
                    patterns.pop_back();
                    --j;
                    ++test;
                }
            }
        }
        cout << '\r' << std::flush;
        cout << "[  Mgr  ] Collecting and merging patterns for " << _output[PO_id]._name << " ... " << std::right << std::setw(6) << test << std::flush;
        if (!test) break;
    }
    cout << endl;
    */
}

void Mgr::Merge(std::string& base, const std::string& candidate) {
    // if the two strings are identical, they will be automatically merged as well
    for (int bit = 0; bit < (int)base.length(); ++bit) {
        if (base[bit] != candidate[bit]) {
            base[bit] = '-';
            break;
        }
    }
}

bool Mgr::DiffByOne(const std::string& s1, const std::string& s2) {
    // this function returns true when the two strings are identical
    int count = 0;
    for (int bit = 0; bit < (int)s1.size(); ++bit) {
        if (s1[bit] != s2[bit]) ++count;
        if (count > 1) return false;
    }
    return true;
}

}
