#include "Mgr.h"

static Pattern::Generator Gen;

namespace LogicRegression
{

void Mgr::GenPattern() {
    cout << "[Mgr]    Generating " << _numInput*UnitPatSize*60 << " relations to calculate information gain" << endl;
    this->GenerateInputPattern("in_pat.txt", _numInput*UnitPatSize*60);
    this->RunIOGen();
    this->ReadIORelation();

    std::vector<std::vector<Pat> > refinedPatternIn;
    std::vector<std::vector<Pat> > refinedPatternOut;

    for (int i = 0; i < (int)_output.size(); ++i) {
        std::vector<std::pair<double, VariableID> > info;
        this->CalInfoGain(i, info);
        this->refinePattern(refinedPatternIn, info);
        this->removeDuplicates(refinedPatternIn);
    }
    this->WritePattern(refinedPatternIn);
    this->RunIOGen();
    this->ReadIORelation();
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
    cout << "[Mgr]    Generating relation from " << _iogen << " ..." << endl;
    std::system(("./"+_iogen+" in_pat.txt io_rel.txt").c_str());
}

void Mgr::CalInfoGain(const int PO_id, std::vector<std::pair<double, VariableID> >& info) {
    cout << endl;
    cout << "[Mgr]    Finding input variables with great information gain in terms of " << _output[PO_id]._name << endl;
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
    std::vector<std::pair<double, VariableID> > info_gain; info_gain.resize(_input.size());
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
(std::vector<std::vector<Pat> >& refinedPatternIn, const std::vector<std::pair<double, VariableID> >& info) {

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
    cout << "[Mgr]    Number of chosen input variables: " << partition_index;
    // cout << " (";
    // for (int i = 0; i < partition_index; ++i) {
    //     cout << _input[info[i].second]._name;
    //     if (i < partition_index - 1) cout << ' ';
    // } cout << "), ";

    // enumerate the chosen variables
    std::vector<Pat> temp_pat(_numInput, 0x0);
    std::vector<bool> ex_map; ex_map.resize(_numInput, false);
    int curPatCount = 0;
    const int& chosenVarNum = partition_index;
    for (int i = 0; i < (1 << chosenVarNum); ++i) {
        // create base
        std::vector<bool> base(_numInput, 0);
        for (int j = chosenVarNum-1; j >= 0; --j) {
            bool   tmp = ( ( i >> (j) ) & MASK );
            int bit_id = info[chosenVarNum-1-j].second;
            base[bit_id]   = tmp;
            ex_map[bit_id] = true;
        }

        // random generate patterns for each enumearted pattern
        int randPatCount = (_numInput-chosenVarNum)*UnitPatSize/4;
        if (!randPatCount) randPatCount = 1;
        for (int k = 0; k < randPatCount; ++k) {
            ++curPatCount;
            std::vector<bool> newPat(base);
            for (int j = 0; j < _numInput; ++j) {
                if (ex_map[j]) continue;
                newPat[j] = Gen(2);
            }

            // pack pattern
            for (int varID = 0; varID < _numInput; ++varID) {
                temp_pat[varID] = temp_pat[varID] << 1;
                temp_pat[varID] += (Pat)newPat[varID];
            }
            if (curPatCount % UnitPatSize == 0) {
                refinedPatternIn.push_back(temp_pat);
                std::vector<Pat> newContainer(temp_pat.size(), 0x0);
                temp_pat.swap(newContainer);
            }
        }
    }
    if (curPatCount % UnitPatSize) refinedPatternIn.push_back(temp_pat);

    cout << ", " << curPatCount << " refined patterns generated" << endl;
}

void Mgr::removeDuplicates(std::vector<std::vector<Pat> >& refinedPatternIn) {
    // not sure if this is needed
    // this function is costly
    // time complexity O(n^2)
    return;
}

void Mgr::WritePattern(const std::vector<std::vector<Pat> >& refinedPatternIn, std::string filename) const {
    cout << "[Mgr]    Writing " << 64*refinedPatternIn.size() << " refiend patterns to file ..." << endl;
    std::ofstream file;
    file.open(filename.c_str());

    if ( !file.is_open() ) {
        cout << "[WritePattern]    Cannot create input file: " << filename << " for " << _iogen << endl;
        exit(-1);
    }

    file << _numInput << ' ' << 64*refinedPatternIn.size() << endl;
    for (int i = 0; i < _numInput; ++i) {
        file << _input[i]._name;
        if (i < (int)_input.size()-1) file << ' ';
    }
    file << endl;

    for (int batch = 0; batch < (int)refinedPatternIn.size(); ++batch) {
        for (int bit_id = UnitPatSize-1; bit_id >= 0; --bit_id) {
            for (int varID = 0; varID < (int)refinedPatternIn[batch].size(); ++varID) {
                const Pat& curPat = refinedPatternIn[batch][varID];
                file << ( ( curPat >> bit_id ) & MASK );
                if (varID < _numInput - 1) file << ' ';
            }
            file << endl;
        }
    }
    file.close();
}

}
