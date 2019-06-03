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
	cout << "[Mgr]    creating PLA file from random pattern" << endl;
	std::ofstream plaFile;
	plaFile.open(filename.c_str());
	plaFile << "# create by random simulated pattern " << endl;
	/* specify input number */
	plaFile << ".i " << _numInput << endl;
	/* specify output number */
	plaFile << ".o " << _numOutput << endl;
	/* specify input variable name */
	plaFile << ".ilb ";
	for (std::map<std::string, VariableID>::iterator iter = _input_variable_name_id_map.begin();
			iter != _input_variable_name_id_map.end(); iter++ ) {
		plaFile << iter->first << " ";
	}
	plaFile << endl;
	/* specify output varaible name */
	plaFile << ".ob ";
	for (std::map<std::string, VariableID>::iterator iter = _output_variable_name_id_map.begin();
			iter != _output_variable_name_id_map.end(); iter++) {
		plaFile << iter->first << " "; 
	}
	plaFile << endl;
	/* specify pattern number */
	/* specify input_pattern & output_pattern */	
	/* TODO change the function used by output2input_map into equivalent function in self defined hash map */
	size_t size = output2input_map.size();
	cout << "[Mgr]    total patterns = " << size << endl;
	plaFile << ".p " << size << endl;
	std::map< std::string, std::string>::iterator  mapIter;
	for(mapIter = output2input_map.begin(); mapIter != output2input_map.end(); mapIter++){
		plaFile << mapIter->second << " " << mapIter->first << endl;
	}
	/* specify end of file */
	plaFile << ".e" << endl;
	cout << "[Mgr]    creating PLA file done" << endl;
}
/* end of namespace */
}
