#include "Mgr.h"
#include <bitset>

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
	std::map< std::string, std::string> output2input;
	std::map< std::string, std::string>::iterator  mapIter;
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
	size_t  pattern_num = _relation_in[0].size();
	cout << "[Mgr]    finding identical output" << endl;
	for (size_t i = 0; i < pattern_num; i++){
		std::vector<std::bitset<64> > inputPattern;
		std::vector<std::bitset<64> > outputPattern;
		/* transfer input_pattern & output_pattern into 64 bits bitset object */
		for (int j = 0; j < _numInput; j++){
			std::bitset<64> b(_relation_in[j][i]);
			inputPattern.push_back(b);	
		}
		for (int j = 0; j < _numOutput; j++){
			std::bitset<64> b(_relation_out[j][i]);
			outputPattern.push_back(b);
		}
		for (int k = 0; k < 64; k++){
			/* transfer pattern into a sequence of input pattern corresponding to output pattern */
			std::string b_in;
			std::string b_out;
			for(int j = 0; j < _numInput; j++){
				char t;
				if (inputPattern[j][k] == 0) t = '0';
				else t = '1';
				b_in.push_back(t);
			}
			for (int j = 0; j < _numOutput; j++){
				char t;
				if (outputPattern[j][k] == 0) t = '0';
				else t = '1';
				b_out.push_back(t);
			}
			/* convert input pattern from bitset to string to handle don't care terms */
			mapIter = output2input.find(b_out);
			if(mapIter == output2input.end())
				output2input.insert(std::make_pair(b_out, b_in));
			else {
				std::string str = output2input[b_out];
				for (int i = 0; i < _numInput; i++){
					if(str[i] != b_in[i]) {
						str[i] = '-';
					}
				}
				output2input[b_out] = str;
			}
		}	
	}
	/* specify input_pattern & output_pattern */	
	size_t size = output2input.size();
	cout << "[Mgr]    total patterns = " << size << endl;
	plaFile << ".p " << size << endl;
	for(mapIter = output2input.begin(); mapIter != output2input.end(); mapIter++){
		plaFile << mapIter->second << " " << mapIter->first << endl;
	}
	/* specify end of file */
	plaFile << ".e" << endl;
	cout << "[Mgr]    creating PLA file done" << endl;
}
/* end of namespace */
}
