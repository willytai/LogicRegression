#include "Mgr.h"
#include "myUsage.h"

extern Pattern::Generator Gen;
extern MyUsage usg;

namespace LogicRegression
{

void Mgr::GenPattern() {
    this->GenerateInputPattern("in_pat.txt");
    this->RunIOGen();
    this->ReadIORelation();
    this->FindDependentInput();
    this->Enumerate();
    this->RunIOGen();
    this->ReadIORelation();
    exit(0);

    // PatternBank patBank;

    // for (int i = 0; i < (int)_output.size(); ++i) {
        // std::vector<std::pair<double, VariableID> > info;
        // this->CalInfoGain(i, info);
        // this->refinePattern(patBank, info);
    // }
    // patBank.random_sample();

    this->GenerateBLIF(); // initial patterns
    this->GeneratePLA();  // patterns for simulation
}

void Mgr::GenerateInputPattern(std::string filename) {
    // ex:
    // 0000000 0000000
    // 1000000 0100000
    int fixPat = log2(_numInput-1) * 100;
    int numPat = 2 * _numInput * fixPat;
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

void Mgr::RunIOGen() const {
    cout << "[  Mgr  ] Generating relation from " << _iogen << " ..." << endl;
    std::system(("./"+_iogen+" in_pat.txt io_rel.txt").c_str());
}

void Mgr::FindDependentInput() {
    cout << "[  Mgr  ] Searching for dependent inputs for every output ..." << endl;
    // for every output, find the dependent input by the diff-by-1 patterns
    _fanins.resize(_numOutput);
    for (int pat_id = 0; pat_id < _numPat; pat_id += 2) {
        const std::string& in_pat_1 = _relation_in[pat_id];
        const std::string& in_pat_2 = _relation_in[pat_id+1];
        const std::string& out_pat_1 = _relation_out[pat_id];
        const std::string& out_pat_2 = _relation_out[pat_id+1];
        for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
            if (out_pat_1[PO_id] == out_pat_2[PO_id]) continue;
            for (int PI_id = 0; PI_id < _numInput; ++PI_id) {
                if (in_pat_1[PI_id] == in_pat_2[PI_id]) continue;
                auto it = std::find(_fanins[PO_id].begin(), _fanins[PO_id].end(), PI_id);
                if (it == _fanins[PO_id].end()) _fanins[PO_id].push_back(PI_id);
                break;
            }
        }
    }
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
        cout << _output[PO_id]._name << "'s fanins(" << _fanins[PO_id].size() << "):";
        for (int i = 0; i < (int)_fanins[PO_id].size(); ++i) cout << ' ' << _input[_fanins[PO_id][i]]._name;
        cout << endl;
    }
    cout << endl;
}

void Mgr::Enumerate() {
    cout << "[  Mgr  ] Enumerating patterns ... ";
    PatternBank patBank;
    for (int PO_id = 0; PO_id < _numOutput; ++PO_id) {
        int nDependent = _fanins[PO_id].size();
        if (nDependent <= MAX_ENUMERATE_VAR_NUM) { // enumerate
            for (int i = 0; i < (1 << nDependent); ++i) {
                std::string newPat(_numInput, 'X');
                for (int dep_id = 0; dep_id < nDependent; ++dep_id) {
                    bool v = ( (i >> dep_id) & MASK );
                    if (v) newPat[_fanins[PO_id][dep_id]] = '1';
                    else   newPat[_fanins[PO_id][dep_id]] = '0';
                }
                for (int bit = 0; bit < (int)newPat.size(); ++bit) {
                    if (newPat[bit] != 'X') continue;
                    if (Gen(2)) newPat[bit] = '1';
                    else        newPat[bit] = '0';
                }
                patBank.insert(newPat);
            }
        }
        else { // sample

        }
    }
    this->WritePattern(patBank);
    cout << patBank.size() << " patterns enumearted." << endl;
}

/*
void Mgr::CalInfoGain(const int PO_id, std::vector<std::pair<double, VariableID> >& info) {
    cout << endl;
    cout << "[  Mgr  ] Finding input variables with great information gain in terms of " << _output[PO_id]._name << endl;
    assert(PO_id < (int)_output.size());

    // calculate entropy for the corresponding output
    const std::vector<Pat>& PO_pat = _relation_out[PO_id];
    double PO_entropy, positive = 0.0, negative = 0.0;

    for (int i = 0; i < (int)PO_pat.size(); ++i) {
        for (int j = 0; j < UnitPatSize; ++j) {
            if ( (PO_pat[i] >> j) & MASK ) ++positive;
            else                           ++negative;
        }
    }
    if (positive == 0.0) positive = std::numeric_limits<double>::min();
    if (negative == 0.0) negative = std::numeric_limits<double>::min();
    double tmp = positive + negative;
    positive /= tmp;
    negative /= tmp;
    PO_entropy = -log2(positive)*positive - log2(negative)*negative;
    // cout << _output[PO_id]._name << "'s entropy: " << PO_entropy << endl;

    // calculate the information gain of each input
    std::vector<std::pair<double, VariableID> > info_gain(_input.size());
    for (int child_id = 0; child_id < (int)info_gain.size(); ++child_id) {
        const std::vector<Pat>& PI_pat = _relation_in[child_id];
        double p_child_p = 0.0, p_child_n = 0.0, n_child_p = 0.0, n_child_n = 0.0;
        for (int pat_id = 0; pat_id < (int)PO_pat.size(); ++pat_id) {
            for (int j = 0; j < UnitPatSize; ++j) {
                if ( (PI_pat[pat_id] >> j) & MASK ) {
                    if ( (PO_pat[pat_id] >> j) & MASK ) {
                        ++p_child_p;
                    }
                    else {
                        ++p_child_n;
                    }
                }
                else {
                    if ( (PO_pat[pat_id] >> j) & MASK ) {
                        ++n_child_p;
                    }
                    else {
                        ++n_child_n;
                    }
                }
            }
        }
        double p_weight = (p_child_p + p_child_n) / (p_child_p + p_child_n + n_child_p + n_child_n);
        double n_weight = (n_child_p + n_child_n) / (p_child_p + p_child_n + n_child_p + n_child_n);
        if (p_child_p == 0.0) p_child_p = std::numeric_limits<double>::min();
        if (p_child_n == 0.0) p_child_n = std::numeric_limits<double>::min();
        if (n_child_p == 0.0) n_child_p = std::numeric_limits<double>::min();
        if (n_child_n == 0.0) n_child_n = std::numeric_limits<double>::min();
        tmp = p_child_p + p_child_n;
        p_child_p /= tmp;
        p_child_n /= tmp;
        tmp = n_child_p + n_child_n;
        n_child_p /= tmp;
        n_child_n /= tmp;
        double p_entropy = -log2(p_child_p)*p_child_p - log2(p_child_n)*p_child_n;
        double n_entropy = -log2(n_child_p)*n_child_p - log2(n_child_n)*n_child_n;
        double gain      = PO_entropy - p_weight*p_entropy - n_weight*n_entropy;
        info_gain[child_id] = std::pair<double, VariableID>(gain, child_id);
    }

    // sort in descending order
    std::sort(info_gain.begin(), info_gain.end(), std::greater<std::pair<double, VariableID> >());
    info.swap(info_gain);
}

void Mgr::refinePattern
(PatternBank& patBank, const std::vector<std::pair<double, VariableID> >& info) {

    // for visualization
    int old = patBank.size();

    // find best partition
    int partition_index = info.size();
    while (partition_index == (int)info.size() || partition_index >= MAX_ENUMERATE_VAR_NUM) {
        int limit = partition_index;
        double margin = 0.0;
        for (int i = 0; i < limit-1; ++i) {
            double curMargin = info[i].first - info[i+1].first;
            if (curMargin > margin) {
                margin = curMargin;
                partition_index = i+1;
            }
        }
    }
    if (partition_index < MIN_ENUMERATE_VAR_NUM) partition_index = MIN_ENUMERATE_VAR_NUM;
    cout << "[  Mgr  ] Number of chosen input variables: " << partition_index << endl;
    cout << "[  Mgr  ] Enumerating and combining patterns ..." << endl;

    const int& chosenVarNum = partition_index;
    for (int i = 0; i < ( 1 << chosenVarNum ); ++i) {
        std::string pattern(_numInput, 'X');
        int patternKey = 0;
        for (int j = 0; j < chosenVarNum; ++j) {
            bool   tmp = ( ( i >> j ) & MASK );
            int in_id  = info[partition_index-1-j].second;
            pattern[in_id] = ( tmp ? '1' : '0' );
            patternKey += tmp;
        }
        patBank.insert(pattern);
    }
    cout << "[  Mgr  ] " << patBank.size()-old << " additional base patterns generated." << endl;
}
*/

void Mgr::WritePattern(const PatternBank& patBank, std::string filename) const {
    std::ofstream file;
    file.open(filename.c_str());
    file << _numInput << ' ' << patBank.size() << endl;
    for (int i = 0; i < _numInput; ++i) {
        file << _input[i]._name;
        if (i < _numInput-1) file << ' ';
    }
    file << endl;
    patBank.WritePattern(file);
}

/* end of namespace */
}
