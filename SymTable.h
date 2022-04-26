#include "DataLine.h"
#define LABLE_SIZE 32

typedef enum symbol_traite 
{
	DATA, CODE, EXTERNAL
} symbol_traite;

typedef struct DataSym
{
	char* name;
	int value;
	symbol_traite attribute;
	Boolian isEntry;
} DataSym, *SymDataPointer;

typedef struct NodeSym
{
	SymDataPointer symbolData;
	struct NodeSym* nxt;
} NodeSym, *SymNodePointer;

typedef struct DataExternal
{
	char symbol[LABLE_SIZE];
	int address;
} DataExternal, *DataExternalPointer;

#pragma once
typedef struct NodeExternal
{
	DataExternalPointer data;
	struct NodeExternal* nxt;
} NodeExternal, *ExternalNodePointer;

int SymAdding(char* n, int val, symbol_traite attr, int now_line_index);

SymNodePointer GetTableSymbolHead();

ExternalNodePointer GetListExternalHead();

SymDataPointer SymbolCheck(char* n);

int ExtHandle(char* row, int now_line_index);

int EndHandle(char* row, int now_line_index);

int Entrycounter();

Boolian ExternalListAdding(char *sym, int addr);

void SymTabFree();

void ExtrnListFree();

void ICaddingToSymbols(int icf);

Boolian IsComment(char* row);

Boolian IsLabel(char* row);

Boolian IsEmpty(char* row);

Boolian IsEntry(char* row);

Boolian IsExtern(char* row);

Boolian alpha(char charator);

Boolian IsDigit(char charator);

void SpaceDelete(char* row);

void SpaceSkipping(int* indx, char* row);

void lbl_skip(int* indx, char* row);

char* GetLable(char* row);

char* GetNameOfOperation(char* row);

char* GetEntryLableOrExternLable(int from_index, char* row);

char* GetString(char* row);

int GetStringsNublers(char* s, int* now_index, char ending_flag, int* res);
