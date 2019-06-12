#include "selfdefined.h"
#include "simMgr.h"

/* self defined function to run simulation on AIG */
/* type sim_aig pattern_file_name to toggle this command */
ABC_NAMESPACE_IMPL_START


int Abc_SimAig(Abc_Ntk_t * p, char* pFileName) {
	SimulationMgr Mgr(p, pFileName);
	int i = 0;
	while(!Mgr.patternFilereadDone()){
		i++;
		Mgr.readPattern();
		Mgr.simulateNode();
		Mgr.comparePatternwithSimulation();
	}
	std::cout << std::endl;
	std::cout << "simulate " << i*64 << " pattern" << std::endl;
	return 0;
}


/* SimulationMgr's function defined */
void SimulationMgr::readPattern(){
	std::vector<std::string> inputPattern;
	std::vector<std::string> outputPattern;
	std::string tempPattern;
	std::string splitPattern;
	std::bitset<64> pattern2size_t;
	/* pattern file format */
	/* inputPattern outputPattern */
	for (size_t i = 0; i < 64; i++){
		tempPattern.clear();
		splitPattern.clear();
		if(getline(patternFile, tempPattern)) {
			size_t pos = tempPattern.find(" ");
			splitPattern = tempPattern.substr(0, pos);
			inputPattern.push_back(splitPattern);
			splitPattern = tempPattern.substr(pos+1, tempPattern.size() - pos -1);
			outputPattern.push_back(splitPattern);
		}
		else break;
	}
	/* make sure that the pattern size = 64 */
	while(inputPattern.size() != 64){
		inputPattern.push_back(inputPattern.back());
		outputPattern.push_back(outputPattern.back());
	}
	/* make sure the size of the pattern is matched to the ABC current AIG */
	assert(inputPattern[0].size() == nInputs);
	assert(outputPattern[0].size() == nOutputs);
	Input.clear();
	Output.clear();
	/* convert the 64 pattern into size_t to perform parallel simulation using bitset */
	for (int i = 0; i < nInputs; i++){
		pattern2size_t.reset();
		for (size_t j = 0; j < 64; j++) {
			if(inputPattern[j][i] == '1') pattern2size_t.set(j);			
		}
		Input.push_back(pattern2size_t);
	}
	for (int i = 0; i < nOutputs; i++) {
		pattern2size_t.reset();
		for (size_t j = 0; j < 64; j++) {
			if (outputPattern[j][i] == '1') pattern2size_t.set(j);
		}
		Output.push_back(pattern2size_t);
	}
}

void SimulationMgr::simulateNode(){
	Abc_Obj_t * pNode;
	int i;
	std::bitset<64> const1;
	const1.set();
	simulatePattern[0] = const1;
	//ID of ABC start from 1
	Abc_NtkForEachPi( pNtk, pNode, i ){
		int Id = pNode->Id;
		simulatePattern[Id] = Input[Id-1];
	}
	Abc_NtkForEachNode( pNtk, pNode, i){
		int Id = pNode->Id;
		int fanin0ID = Abc_ObjFaninId0(pNode);
		int fanin1ID = Abc_ObjFaninId1(pNode);
		std::bitset<64> fanin0 = simulatePattern[fanin0ID];
		std::bitset<64> fanin1 = simulatePattern[fanin1ID];
		if (Abc_ObjFaninC0(pNode)) {fanin0.flip();}
		if (Abc_ObjFaninC1(pNode)) {fanin1.flip();}
		/* perform bitwise and  */
		simulatePattern[Id] = (fanin0 &= fanin1);
	}
	Abc_NtkForEachPo( pNtk, pNode, i){
		int Id = pNode->Id;
		int faninID = Abc_ObjFaninId0(pNode);
		std::bitset<64> fanin = simulatePattern[faninID];
		if (Abc_ObjFaninC0(pNode)) fanin.flip();
		simulatePattern[Id] = fanin;
	}
}

void SimulationMgr::comparePatternwithSimulation(){
	bool isDiff[64] = {false};
	for (int i = 0; i < nOutputs; i++){
		/* perform bitwise XOR */
		std::bitset<64> comparison = (Output[i] ^= simulatePattern[i+nInputs+1]);
		/* if all the bits are 0 => simulation result match the given pattern */
		if (comparison.none()) continue;
		/* else find the index that the bit = 1 => that pattern is not cover by the current network */
		else {
			for (int j = 0; j < 64; j++){
				if(comparison[j] == 1) isDiff[j] = true;
			}
		}
	}
	std::string pattern;
	int number = 0;
	for (int i = 0; i < 64; i++){
		if (isDiff[i] == true){
			number++;
			pattern.clear();
			for (int j = 0; j < nInputs; j++){
				if (Input[j][i] == 0)
					pattern.push_back('0');
				else pattern.push_back('1');
			}
			pattern.push_back(' ');
			for (int j = 0; j < nOutputs; j++){
				if (Output[j][i] == 0) pattern.push_back('0');
				else pattern.push_back('1');
			}
			unSatisfiedPattern.push_back(pattern);
		}
	}
	std::cout << '\r' << std::flush;
	std::cout << "finding " << unSatisfiedPattern.size() << " unsatisfied pattern" << std::flush;
}




ABC_NAMESPACE_IMPL_END
