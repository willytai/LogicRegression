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
void Mgr::GenerateBLIF(std::string filename)
{
/* Define the function which generate PLA file for abc from pattern */
	cout << "[Mgr]    creating BLIF file from random pattern" << endl;
	std::ofstream blifFile;
	blifFile.open(filename.c_str());
	blifFile << ".model ICCAD_testcase" << endl;
	/* specify input variable name */
	blifFile << ".inputs " ;
	for (std::map<std::string, VariableID>::iterator iter = _input_variable_name_id_map.begin();
			iter != _input_variable_name_id_map.end(); iter++) {
		blifFile << iter->first << " ";
	}
	blifFile << endl;
	/* specify output varaible name */
	blifFile << ".outputs ";
	std::vector<std::string> output_name;
	for (std::map<std::string, VariableID>::iterator iter = _output_variable_name_id_map.begin();
			iter != _output_variable_name_id_map.end(); iter++) {
		blifFile << iter->first << " "; 
		output_name.push_back(iter->first);
	}
	blifFile << endl;
	/* specify pattern number */
	/* specify input_pattern & output_pattern */	
	/* TODO change the function used by output2input_map into equivalent function in self defined hash map */
	size_t size = output2input_map.size();
	cout << "[Mgr]    total patterns = " << size << endl;
	for (int i = 0; i < _numOutput; i++) {
		blifFile << ".names ";
		for (std::map<std::string, VariableID>::iterator iter = _input_variable_name_id_map.begin();
				iter != _input_variable_name_id_map.end();iter++) {
			blifFile << iter->first << " ";
		}
		blifFile << output_name[i] << endl;
		std::vector<std::string> onset;
		std::vector<std::string> offset;
		for (std::map<std::string, std::string>::iterator iter = output2input_map.begin();
				iter != output2input_map.end(); iter++) {
			if ( iter->first[i] == '1') {
				onset.push_back(iter->second);
			}
			else {
				offset.push_back(iter->second);
			}
		}
		
		if (onset.size() != 0){
			for(size_t i = 0; i < onset.size(); i++ ) {
				blifFile << onset[i] << " 1" << endl;
			}
		}
		/* handle the case with no onset */
		else {
			for (size_t i = 0; i < offset.size(); i++) {
				blifFile<< offset[i] << " 0" << endl;
			}
		}
	}
	/* specify end of file */
	blifFile << ".end" << endl;
	cout << "[Mgr]    creating blif file done" << endl;
}
/* end of namespace */
}
