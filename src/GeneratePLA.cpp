#include "Mgr.h"

/*
 * PLA file format
 * .i input_number
 * .o output_number
 * .ilb input_name
 * .ob output_name
 * .p pattern number
 * input_pattern output_pattern
 * .e 
 */


namespace LogicRegression
{
void Mgr::GeneratePLA(std::string filename)
{
/* Define the function which generate PLA file for abc from pattern */
    cout << "[  Mgr  ] creating PLA file from random pattern" << endl;
    std::ofstream plaFile;
    plaFile.open(filename.c_str());
    /* plaFile << "# created by random simulated pattern " << endl; */
    /* specify input number */
    /* plaFile << ".i " << _numInput << endl; */
    /* specify output number */
    /* plaFile << ".o " << _numOutput << endl; */
    /* specify input variable name */
    /* plaFile << ".ilb "; */
    /* for (int id = 0; id < _numInput; ++id) plaFile << _input[id]._name << ' '; */
    /* plaFile << endl; */
    /* specify output varaible name */
    /* plaFile << ".ob "; */
    /* for (int id = 0; id < _numOutput; ++id) plaFile << _output[id]._name << ' '; */
    /* plaFile << endl; */
    /* specify pattern number */
    /* specify input_pattern & output_pattern */    
    const int& size = _numPat;
    cout << "[  Mgr  ] total patterns = " << size << endl;
    /* plaFile << ".p " << size << endl; */
    int count = 0;
    // only the batches for simulation
    for (int batch = _syn_end; batch < (int)_relation_in[0].size(); ++batch) {
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
