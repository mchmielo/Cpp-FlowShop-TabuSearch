#pragma once
#include <iostream>

class BlockedOperation
{
private:
	std::pair<int, int> operations;
	int machine;
public:
	BlockedOperation();
	BlockedOperation(int arg1, int arg2, int arg3);
	~BlockedOperation();
	void makeBlockedOperation(int oper1, int oper2, int machine);
	std::pair<int, int> getPair();
	bool isEqual(const BlockedOperation &toCompare);
};

