#include "Mgr.h"
#include "myUsage.h"

extern Pattern::Generator Gen;
extern MyUsage usg;

namespace LogicRegression
{

void Mgr::GenPattern() {
    this->GenerateTestPatterns();
    this->GenerateInputPattern("in_pat.txt");
    this->RunIOGen();
    this->ReadIORelation();
    this->FindDependentInput();
    this->EnumerateAndSimulate();
    this->GenerateBLIF(); // patterns for synthesis
}

void Mgr::GenerateTestPatterns() {
    /* Generates 100000 patterns for simulation, getting less than 10 errors would pass the baseline */
    cout << "[  Mgr  ] Random sampling " << 100000+2+2*_numInput << " patterns for testing ..." << endl;
    std::ofstream file; file.open("test_pat.txt");
    file << _numInput << ' ' << 100000+2+2*_numInput << endl;
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
    for (int bit = 0; bit < _numInput; ++bit) {
        file << '1';
        if (bit < _numInput-1) file << ' ';
    }
    file << endl;
    for (int bit = 0; bit < _numInput; ++bit) {
        file << '0';
        if (bit < _numInput-1) file << ' ';
    }
    file << endl;
    for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
        // some special patterns
        for (int bit = 0; bit < _numInput; ++bit) {
            if (bit == PI_id) file << '0';
            else              file << '1';
            if (bit < _numInput-1) file << ' ';
        }
        file << endl;
        for (int bit = 0; bit < _numInput; ++bit) {
            if (bit == PI_id) file << '1';
            else              file << '0';
            if (bit < _numInput-1) file << ' ';
        }
        file << endl;
    }
    file.close();

    this->RunIOGen("test_pat.txt", "test_rel.txt");
    this->ReadIORelation("test_rel.txt", true);
}

void Mgr::GenerateInputPattern(std::string filename) {
    // ex:
    // 0000000 0000000
    // 1000000 0100000
    int fixPat = log2(_numInput-1) * 100;
    int numPat = 4 * _numInput + 2 * _numInput * fixPat;
    cout << "[  Mgr  ] Generating " << numPat << " patterns that differ by one bit ..." << endl;
    std::ofstream file;
    file.open(filename.c_str());

    if ( !file.is_open() ) {
        cout << "[GenerateInputPattern]    Cannot create input file: " << filename << " for " << _iogen << endl;
        exit(-1);
    }

    file << _numInput << ' ' << numPat << endl;
    for (int i = 0; i < (int)_input.size(); ++i) {
        file << _input[i]._name;
        if (i < (int)_input.size()-1) file << ' ';
    }
    file << endl;

    for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
        // some special patterns
        for (int bit = 0; bit < _numInput; ++bit) {
            file << '1';
            if (bit < _numInput-1) file << ' ';
        }
        file << endl;
        for (int bit = 0; bit < _numInput; ++bit) {
            if (bit == PI_id) file << '0';
            else              file << '1';
            if (bit < _numInput-1) file << ' ';
        }
        file << endl;
        for (int bit = 0; bit < _numInput; ++bit) {
            file << '0';
            if (bit < _numInput-1) file << ' ';
        }
        file << endl;
        for (int bit = 0; bit < _numInput; ++bit) {
            if (bit == PI_id) file << '1';
            else              file << '0';
            if (bit < _numInput-1) file << ' ';
        }
        file << endl;

        for (int i = 0; i < fixPat; ++i) {
            std::string pat(_numInput, 'X');
            Gen(pat);
            // the '0' pattern
            for (int bit = 0; bit < (int)pat.length(); ++bit) {
                if (bit == PI_id) file << '0';
                else              file << pat[bit];
                if (bit < (int)pat.length()-1) file << ' ';
            }
            file << endl;
            // the '1' pattern
            for (int bit = 0; bit < (int)pat.length(); ++bit) {
                if (bit == PI_id) file << '1';
                else              file << pat[bit];
                if (bit < (int)pat.length()-1) file << ' ';
            }
            file << endl;
        }
    }
    file.close();
}

void Mgr::RunIOGen(std::string in_pat, std::string io_rel) const {
    cout << "[  Mgr  ] Generating relation from " << _iogen << " ..." << endl;
    std::system(("./"+_iogen+" "+in_pat+" "+io_rel).c_str());
}

void Mgr::FindDependentInput() {
    cout << "[  Mgr  ] Searching for dependent inputs for every output ..." << endl;
    // for every output, find the dependent input by the diff-by-1 patterns
    _fanin_mask.clear();
    _fanin_mask.resize(_numOutput);
    for (auto& val: _fanin_mask)  val.resize(_numInput, false);

    for (int pat_id = 0; pat_id < (int)_relation_in.size(); pat_id += 2) {
        const std::string& in_pat_1 = _relation_in[pat_id];
        const std::string& in_pat_2 = _relation_in[pat_id+1];
        const std::string& out_pat_1 = _relation_out[pat_id];
        const std::string& out_pat_2 = _relation_out[pat_id+1];
        for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
            if (out_pat_1[PO_id] == out_pat_2[PO_id]) continue;
            for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
                if (in_pat_1[PI_id] == in_pat_2[PI_id]) continue;
                _fanin_mask[PO_id][PI_id] = true;
                break;
            }
        }
    }
}

void Mgr::EnumerateAndSimulate() {
    std::vector<std::string> newPatterns;
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) this->Enumerate(newPatterns, PO_id);
    this->WritePattern(newPatterns);
    this->RunIOGen();
    this->ReadIORelation();
    this->Simulate();
}

void Mgr::Enumerate(std::vector<std::string>& newPatterns, int PO_id) {
    int nDependent = this->count(_fanin_mask[PO_id]);
    if (nDependent <= MAX_ENUMERATE_VAR_NUM) { // enumerate
        for (int i = 0; i < (1 << nDependent); ++i) {
            std::string newPat(_numInput, '-');
            int dep_id = 0;
            for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
                bool v;
                if (!_fanin_mask[PO_id][PI_id]) v = Gen(2);
                else v = ( (i >> dep_id++) & MASK );
                if (v) newPat[PI_id] = '1';
                else   newPat[PI_id] = '0';
            }
            newPatterns.push_back(newPat);
        }
    }
    else { // sample a max number of patterns
        for (int i = 0; i < (1 << MAX_ENUMERATE_VAR_NUM); ++i) {
            std::string newPat(_numInput, '-');
            Gen(newPat);
            newPatterns.push_back(newPat);
        }
    }
}

void Mgr::Simulate() {
    std::vector<int> errors(_numOutput, 0);

    // create a hash to store results of outputs
    // use map to store outputs with too many inputs
    bool **table = new bool*[_numOutput];
    std::map<size_t, bool> **m_table = new std::map<size_t, bool>*[_numOutput];
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
        int c = this->count(_fanin_mask[PO_id]);
        if (0 < c && c <= MAX_ENUMERATE_VAR_NUM) {
            table[PO_id] = new bool[(1 << c)];
            m_table[PO_id] = NULL;
        }
        else if (!c) {
            table[PO_id] = new bool[1];
            m_table[PO_id] = NULL;
        }
        else {
            table[PO_id] = NULL;
            m_table[PO_id] = new std::map<size_t, bool>();
        }
    }

    for (int pat_id = 0; pat_id < (int)_relation_in.size(); ++pat_id) {
        const std::string& rel_in = _relation_in[pat_id];
        const std::string& rel_out = _relation_out[pat_id];
        for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
            size_t key = 0x0;
            for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
                if (!_fanin_mask[PO_id][PI_id]) continue;
                key = key << 1;
                key += (rel_in[PI_id] == '1' ? 0x1 : 0x0);
            }
            if (table[PO_id]) table[PO_id][key] = (rel_out[PO_id] == '1' ? true : false);
            else              m_table[PO_id]->operator[](key) = (rel_out[PO_id] == '1' ? true : false);
        }
    }

    std::vector<int> onset_offset_diff(_numOutput, 0); // distance between onset patterns and offset patterns
    for (int test_id = 0; test_id < (int)_test_in.size(); ++test_id) {
        const std::string& t_in = _test_in[test_id];
        const std::string& t_out = _test_out[test_id];
        for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
            size_t key = 0x0;
            for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
                if (!_fanin_mask[PO_id][PI_id]) continue;
                key = key << 1;
                key += (t_in[PI_id] == '1' ? 0x1 : 0x0);
            }
            bool ground_truth = (t_out[PO_id] == '1' ? true : false);
            if (ground_truth) onset_offset_diff[PO_id] += 1;
            else              onset_offset_diff[PO_id] -= 1;
            if (table[PO_id] != NULL) {
                if (ground_truth != table[PO_id][key]) errors[PO_id] += 1;
            }
            else {
                auto it = m_table[PO_id]->find(key);
                if (it == m_table[PO_id]->end()) {
                    _extra_in.push_back(t_in);
                    _extra_out.push_back(t_out);
                }
                else {
                    if (ground_truth != (*it).second) errors[PO_id] += 1;
                }
            }
        }
        cout << '\r' << std::flush;
        cout << "[  Mgr  ] " << test_id+1 << " patterns simulated." << std::flush;
    }

    // set _onset_mask
    _onset_mask.clear(); _onset_mask.resize(_numOutput, true);
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
        if (this->count(_fanin_mask[PO_id]) <= MAX_ENUMERATE_VAR_NUM) continue;
        if (onset_offset_diff[PO_id] > 0) _onset_mask[PO_id] = false;
    }

    cout << endl;
    cout << "[  Mgr  ] Errors:" << endl;
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
        cout << "                 " << _output[PO_id]._name << "(" << std::left << std::setw(2) << this->count(_fanin_mask[PO_id]) << " fanins): ";
        cout << std::left << std::setw(4) << errors[PO_id] << '\t';
        cout << "onset - offset: " << onset_offset_diff[PO_id] << endl;
    }
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
        if (table[PO_id]) delete table[PO_id];
        if (m_table[PO_id]) delete m_table[PO_id];
    }
    delete [] table;
    delete [] m_table;
}

void Mgr::WritePattern(const std::vector<std::string>& newPatterns, std::string filename) const {
    cout << "[  Mgr  ] Writing " << newPatterns.size() << " patterns to " << filename << endl;
    std::ofstream file;
    file.open(filename.c_str());
    file << _numInput << ' ' << newPatterns.size() << endl;
    for (int i = 0; i < _numInput; ++i) {
        file << _input[i]._name;
        if (i < _numInput-1) file << ' ';
    }
    file << endl;
    for (const auto& pat: newPatterns) {
        for (int i = 0; i < (int)pat.length(); ++i) {
            file << pat[i];
            if (i < (int)pat.length()-1) file << ' ';
        }
        file << endl;
    }
}

/* end of namespace */
}
