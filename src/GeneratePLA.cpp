#include "Mgr.h"

extern Pattern::Generator Gen;

namespace LogicRegression
{

void Mgr::GeneratePLA(std::string filename)
{
    /* Define the function which generate PLA file for abc from pattern */
    cout << "[  Mgr  ] Writing to PLA file ... " << std::flush;

    std::ofstream file;

    file.open(filename);
    for (int pat_id = 0; pat_id < (int)_relation_out.size(); ++pat_id) {
        std::string out;
        for (int PO_id = 0; PO_id < (int)_relation_out[pat_id].size(); ++PO_id) {
            out.push_back(_relation_out[pat_id][PO_id]);
        }
        file << _relation_in[pat_id] << ' ' << out << endl;
    }
    file.close();
 
    cout << "complete." << endl;
}

/* end of namespace */
}
