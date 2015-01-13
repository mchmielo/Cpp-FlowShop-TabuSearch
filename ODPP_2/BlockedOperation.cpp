#include "BlockedOperation.h"


BlockedOperation::BlockedOperation()
{
}

BlockedOperation::BlockedOperation(int arg1, int arg2, int arg3) 
: machine(arg3)
{
	this->operations = std::make_pair(arg1, arg2);
}

BlockedOperation::~BlockedOperation()
{
}

void BlockedOperation::makeBlockedOperation(int oper1, int oper2, int machine){
	this->operations = std::make_pair(oper1, oper2);
	this->machine = machine;
}

std::pair<int, int> BlockedOperation::getPair(){
	return this->operations;
}

bool BlockedOperation::isEqual(const BlockedOperation &toCompare){
	if (toCompare.machine == this->machine && 
		toCompare.operations.first == this->operations.first && 
		toCompare.operations.second == this->operations.second)
		return true;
	return false;
}