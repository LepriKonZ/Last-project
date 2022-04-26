#include "DataLine.h"
#include "SymTable.h"

SymNodePointer head_table_symbol = NULL;
ExternalNodePointer ext_list_head = NULL;

SymNodePointer GetTableSymbolHead()
{
	return head_table_symbol;
}

ExternalNodePointer GetListExternalHead()
{
	return ext_list_head;
}

SymDataPointer SymbolCheck(char* sgl_type)
{
	SymNodePointer now_STNode = head_table_symbol;
	while (now_STNode != NULL)
	{
		if (strcmp(now_STNode->symbolData->name, sgl_type) == 0)
		{
			return now_STNode->symbolData;
		}
		now_STNode = now_STNode->nxt;
	}
	return NULL;
}
/*this method gets an information about new symbol to be created such as : name , value , attribute (external or entery) and the current index
 * it create new symbol structure  and also a new node , puts the new node to point to the symbol structre and insert the
 * new node to the symbol table which is a linked list*/
int SymAdding(char* sgl_type, int val, symbol_traite trait, int now_index_line)
{
	SymNodePointer now_node = head_table_symbol, node_symbol;
	SymDataPointer symbolDataPtr = (SymDataPointer)malloc(sizeof(DataSym));
	if (SymbolCheck(sgl_type) != NULL)
	{
		printf("\tLine %d: error, duplicated lables.\n", now_index_line);
		return;
	}
	symbolDataPtr->name = sgl_type;
	symbolDataPtr->value = val;
	symbolDataPtr->attribute = trait;
	symbolDataPtr->isEntry = False;
	node_symbol = (SymNodePointer)malloc(sizeof(NodeSym));
	node_symbol->symbolData = symbolDataPtr;
	node_symbol->nxt = head_table_symbol;
	head_table_symbol = node_symbol;
}
/*this methode get a string represent line which is an external line and treat it */
int ExtHandle(char* row, int now_row_index)
{
	int index = 7;/*7 because this is the index in the string after the word 'external'*/
	char* lable;
	if ((lable = GetEntryLableOrExternLable(index, row)) == NULL) /*here we extract from the line the label which defined as extren - if null we return 0 because it is not valid*/
	{
		return 0;
	}
	SymAdding(lable, 0, EXTERNAL, now_row_index);
	return 1;
}
/*this methode handle in the seconde pass all the entery line*/
int EndHandle(char* row, int now_row_index)
{
	int index = 6;
	char* lable;
	SymDataPointer symbolData;
	if ((lable = GetEntryLableOrExternLable(index, row)) == NULL) return 0;
	if ((symbolData = SymbolCheck(lable)) == NULL) return 0;
	symbolData->isEntry = True;
	return 1;
}

int Entrycounter()
{
	int ctr = 0;
	SymNodePointer now_STNode = head_table_symbol;
	while (now_STNode != NULL)
	{
		if (now_STNode->symbolData->isEntry == True)
		{
			ctr++;
		}
		now_STNode = now_STNode->nxt;
	}
	return ctr;
}

Boolian ExternalListAdding(char* sym, int addr)
{
	ExternalNodePointer ext_node;
	DataExternalPointer ext_data;

	ext_data = (DataExternalPointer)malloc(sizeof(DataExternal));
	ext_data->address = addr;
	strcpy(ext_data->symbol, sym);
	ext_node = (ExternalNodePointer)malloc(sizeof(NodeExternal));
	ext_node->data = ext_data;
	ext_node->nxt = ext_list_head;
	ext_list_head = ext_node;
}

void SymTabFree()
{
	SymNodePointer now_node_symbol = head_table_symbol, node_temp;
	while (now_node_symbol != NULL)
	{
		node_temp = now_node_symbol;
		now_node_symbol = now_node_symbol->nxt;
		free(node_temp->symbolData);
		free(node_temp);
	}
	head_table_symbol = NULL;
}

void ExtrnListFree()
{
	ExternalNodePointer now_ext_node = ext_list_head, node_temp;
	while (now_ext_node != NULL)
	{
		node_temp = now_ext_node;
		now_ext_node = now_ext_node->nxt;
		free(node_temp->data);
		free(node_temp);
	}
	ext_list_head = NULL;
}

void ICaddingToSymbols(int icf)
{
	SymNodePointer currentSTNode = head_table_symbol;
	while (currentSTNode != NULL)
	{
		if (currentSTNode->symbolData->attribute == DATA)
		{
			currentSTNode->symbolData->value += icf;
		}
		currentSTNode = currentSTNode->nxt;
	}
}
