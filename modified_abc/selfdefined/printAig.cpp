#include "selfdefined.h"
#include "aig/aig/aig.h"
#include <iostream>

ABC_NAMESPACE_IMPL_START

int Abc_printAig( Abc_Ntk_t * pNtk){
	if (pNtk == NULL) return 1;
	Abc_Obj_t * pNode;
	int i;
	Abc_NtkForEachPi( pNtk, pNode, i ){
		std::cout << "PI: " << pNode->Id << std::endl;
	}
	Abc_NtkForEachNode( pNtk, pNode, i ){
		std::cout << "AND: " << pNode->Id << " ";
		if (Abc_ObjFaninC0(pNode)) 
			std::cout << "!";
		std::cout << Abc_ObjFaninId0(pNode) << " ";
		if (Abc_ObjFaninC1(pNode))
			std::cout << "!";
		std::cout << Abc_ObjFaninId1(pNode) << std::endl;
	}
	Abc_NtkForEachPo( pNtk, pNode, i ){
		std::cout << "PO " << pNode->Id << " ";
		if (Abc_ObjFaninC0(pNode))
			std::cout << "!";
		std::cout << Abc_ObjFaninId0(pNode) << std::endl;
	}
	return 0;
}




ABC_NAMESPACE_IMPL_END
