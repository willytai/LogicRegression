#ifndef ABC__selfdefined__simMgr_h
#define ABC__selfdefined__simMgr_h


#include "base/abc/abc.h"
#include "misc/util/abc_namespaces.h"
#include <assert.h>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <iostream>


ABC_NAMESPACE_HEADER_START
class SimulationMgr;
class SimulationPattern;

class SimulationMgr
{
public:
	SimulationMgr(Abc_Ntk_t * p, char* filename) : pNtk(p){
		nInputs = Abc_NtkPiNum(pNtk);
		nOutputs = Abc_NtkPoNum(pNtk);
		nGates = Abc_NtkNodeNum(pNtk);
		simulatePattern.resize(nInputs+nOutputs+nGates+1); 
		patternFile.open(filename);
	}
	inline bool patternFilereadDone(){
		if (patternFile.peek() == EOF) {
			return true;
		}
		else return false;
	}
	inline void setSimulation(int pos, std::bitset<64> pat){
		assert(pos < nInputs + nOutputs + nGates);
		simulatePattern[pos-1] = pat;
	}
	void simulateNode();
	void readPattern();
	void comparePatternwithSimulation();
private:
	//ABC framework
	Abc_Ntk_t * pNtk;			//current ABC framework
	int nInputs;				//input number of current AIG
	int nOutputs;				//output number of current AIG
	int nGates;					//gate number of current AIG
	//simulation
	std::vector< std::bitset<64> > simulatePattern;	
	//pattern
	std::ifstream patternFile;				//pattern file
	std::vector< std::bitset<64> > Input;		//the given input pattern
	std::vector< std::bitset<64> > Output;	//the given output pattern
	//output
	std::vector< std::string > unSatisfiedPattern;
};


ABC_NAMESPACE_HEADER_END  
#endif
