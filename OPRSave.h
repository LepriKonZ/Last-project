#pragma once
#include "DataLine.h"
#include "SymTable.h"
#define REGISTERS_NUMBERS 32

enum OprType
{
	R = 0,
	I = 1,
	J = 2,
};

typedef struct OperationData
{
	char name[5];
	enum OprType type;
	int funct;
	int opcode;
} OperationData;

typedef struct OperationNode
{
	OperationData* OperationData;
	struct OperationNode* nxt;
} OperationNode, *OprNodePointer;

void OprTableBuilding();

void FreeOprTable();

OperationData* CheckOpr(char* n);

void AddOpr(char* n, enum OprType type, int function, int operation_code);

int OprR(OperationData* DataOPR, char* arg, unsigned int* operation);

int OprI(OperationData* DataOPR, char* arg, unsigned int* operation, Boolian branch_conditional);

int OprJ(OperationData* DataOPR, char* arg, unsigned int* operation);

int HandleOpr(OperationData* DataOPR, char* Line_of_operation);

int GetRegister(int* from_index, char* arg, unsigned char* regRef);

Boolian ConditionalBranchingChecking(char* operation);

Boolian OprContainSymbolChecking(OperationData* data_operation, char* line_operation);

int EndSecPassOpr(OperationData* data_operation, char* line_operation, unsigned int* operation, int now_addr);

int GetSymAddr(char* arg, unsigned int* addr, enum OprType oprType);

int EndOppI(char* arg, unsigned int* operation, int now_addr);

int EndOperationJ(char* arg, unsigned int* operation, int now_addr);

unsigned int MakeROperation(unsigned char function, unsigned char rd, unsigned char rt, unsigned char rs, unsigned char operation_code);

unsigned int MakeIOperation(int immediate, unsigned char rt, unsigned char rs, unsigned char operation_code);

unsigned int MakeJOperation(unsigned int addr, unsigned char registr, unsigned char operation_code);
