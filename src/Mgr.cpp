#include "Mgr.h"
#include "myUsage.h"

extern Pattern::Generator Gen;
extern MyUsage usg;

namespace LogicRegression
{

void Mgr::DetermineInitParam(int estimateInputNum) {
    // determine the initial number of patterns to synthesize
    // proportional to _numInput only (one output at a time)
    _initPatNum = std::log10(estimateInputNum) / std::log10(1.2) * 600;
    _initPatNum = std::pow(2, estimateInputNum);
    // round to a multiple of UnitPatSize for simplisity
    /*
    int offset = 0;
    while (true) {
        if ( (_initPatNum + offset) % UnitPatSize == 0) {
            _initPatNum += offset;
            break;
        }
        if ( (_initPatNum - offset) % UnitPatSize == 0) {
            _initPatNum -= offset;
            break;
        }
        ++offset;
    }
    */
    cout << "Initial number of patterns: " << _initPatNum << ' ';
}

void Mgr::IncrementalSyn() {
    // Synthesize evey output seperately
    // after all of the accuracy reached 99.99%
    // collect all the used patterns and perform a whole synthesis
    for (int i = 0; i < _numOutput; ++i) {
        cout << endl << "[  Mgr  ] Synthesis for output: " << _output[i]._name << endl;
        this->GenPattern(i);
        this->RunAbc();
        break;
    }
}

void Mgr::GenPattern(int PO_id) {
    int initPatNumFactor = 20;
    cout << "[  Mgr  ] Generating " << _numInput*UnitPatSize*initPatNumFactor << " relations to calculate information gain" << endl;
    this->GenerateInputPattern("in_pat.txt", _numInput*UnitPatSize*initPatNumFactor);
    this->RunIOGen();
    this->ReadIORelation();

    PatternBank patBank;

    std::vector<std::pair<double, VariableID> > info;
    this->GenRandInfo(info);
    // this->CalInfoGain(PO_id, info);
    this->refinePattern(patBank, info);

    patBank.random_sample();

    this->WritePattern(patBank);
    this->RunIOGen();
    this->ReadIORelation();
    this->GenerateBLIF(PO_id, info); // initial patterns
    this->GeneratePLA(PO_id, info);  // patterns for simulation
}

void Mgr::GenerateInputPattern(std::string filename, int numPat) {
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

    std::vector<Pat> patterns; patterns.resize(_numInput);
    int count = 0;
    while (count < numPat) {
        Gen(patterns);
        for (int i = 0; i < UnitPatSize; ++i) {
            for (int j = 0; j < _numInput; ++j) {
                file << (patterns[j] & MASK);
                patterns[j] = patterns[j] >> 1;
                if (j < _numInput-1) file << ' ';
            }
            file << endl;
        }
    count += UnitPatSize;
    }
    file.close();
}

void Mgr::RunIOGen() const {
    cout << "[  Mgr  ] Generating relation from " << _iogen << " ..." << endl;
    std::system(("./"+_iogen+" in_pat.txt io_rel.txt").c_str());
}

void Mgr::GenRandInfo(std::vector<std::pair<double, VariableID> >& info) {
    cout << "[  Mgr  ] sampling inputs randomly" << endl;
    std::vector<VariableID> vec;
    for (int i = 0; i < _numInput; ++i) vec.push_back(i);
    std::random_shuffle(vec.begin(), vec.end());
    for (int i = 0; i < _numInput; ++i) info.push_back(std::pair<double, VariableID>(Gen(100), vec[i]));
}

void Mgr::CalInfoGain(const int PO_id, std::vector<std::pair<double, VariableID> >& info) {
    cout << "[  Mgr  ] Finding input var with great information gain in terms of " << _output[PO_id]._name << endl;
    assert(PO_id < (int)_output.size());

    // calculate entropy for the corresponding output
    const std::string& PO_pat = _relation_out[PO_id];
    double PO_entropy, positive = 0.0, negative = 0.0;

    for (int i = 0; i < (int)PO_pat.size(); ++i) {
        if ( PO_pat[i] == '1' ) ++positive;
        else                    ++negative;
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
        const std::string& PI_pat = _relation_in[child_id];
        double p_child_p = 0.0, p_child_n = 0.0, n_child_p = 0.0, n_child_n = 0.0;
        for (int pat_id = 0; pat_id < (int)PO_pat.size(); ++pat_id) {
            if ( PI_pat[pat_id] == '1' ) {
                if ( PO_pat[pat_id] == '1' ) {
                    ++p_child_p;
                }
                else {
                    ++p_child_n;
                }
            }
            else {
                if ( PO_pat[pat_id] == '1' ) {
                    ++n_child_p;
                }
                else {
                    ++n_child_n;
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
(PatternBank& patBank, std::vector<std::pair<double, VariableID> >& info) {

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
    partition_index = 1;
    cout << "[  Mgr  ] Number of chosen input variables: " << partition_index << ' ';
    this->DetermineInitParam(partition_index);
    cout << "Enumerating and combining patterns ..." << endl;

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
    info = std::vector<std::pair<double, VariableID> >(info.begin(), info.begin()+chosenVarNum);
}

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
