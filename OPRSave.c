#include "OPRSave.h"

OprNodePointer opration_table_head = NULL;


void AddOpr(char* n, enum OprType type, int function, int opcode)
{
	OperationData* data_operation = (OperationData*)malloc(sizeof(OperationData));
	data_operation->opcode = opcode;
	data_operation->type = type;
	strcpy(data_operation->name, n);
	data_operation->funct = function;
	OprNodePointer oprNode = (OprNodePointer)malloc(sizeof(OperationNode));
	oprNode->OperationData = data_operation;
	oprNode->nxt = opration_table_head;
	opration_table_head = oprNode;
}

/*in the project we got an list of instructions which we should translat to binary representation each instruction has some properties
 * we decided to save the instructions in a linked list which each node is an instruction with all the information about the instruction*/
void OprTableBuilding()
{
	opration_table_head = NULL;
	AddOpr("add", R, 1, 0);
	AddOpr("sub", R, 2, 0);
	AddOpr("and", R, 3, 0);
	AddOpr("or", R, 4, 0);
	AddOpr("nor", R, 5, 0);
	AddOpr("move", R, 1, 1);
	AddOpr("mvhi", R, 2, 1);
	AddOpr("mvlo", R, 3, 1);
	AddOpr("addi", I, 0, 10);
	AddOpr("subi", I, 0, 11);
	AddOpr("andi", I, 0, 12);
	AddOpr("ori", I, 0, 13);
	AddOpr("nori", I, 0, 14);
	AddOpr("bne", I, 0, 15);
	AddOpr("beq", I, 0, 16);
	AddOpr("blt", I, 0, 17);
	AddOpr("bgt", I, 0, 18);
	AddOpr("lb", I, 0, 19);
	AddOpr("sb", I, 0, 20);
	AddOpr("lw", I, 0, 21);
	AddOpr("sw", I, 0, 22);
	AddOpr("lh", I, 0, 23);
	AddOpr("sh", I, 0, 24);
	AddOpr("jmp", J, 0, 30);
	AddOpr("la", J, 0, 31);
	AddOpr("call", J, 0, 32);
	AddOpr("stop", J, 0, 63);
}

/*this methode gets as string represent the argument for the instruction and extract the next register in the argument*/
int GetRegister(int* sIndex, char* arg, unsigned char* rgRef)
{
	int res = 0;
	int index = *sIndex;
	if (arg[index] != '$')
	{
		return 0;
	}
	(*sIndex)++;
	if (GetStringsNublers(arg, sIndex, ',', &res) == 0 || res < 0 || res >= REGISTERS_NUMBERS)
	{
		return 0;
	}
	*rgRef = ((unsigned char)res);
	return 1;
}
/*this methode handle all R operations
 * this methode gets as argument 3 things:
 * node represent the R instruction
 * string represent the arguments of the instruction i.e from the first $ caracter
 * and the int variable which we will encode inside the instruction*/
int OprR(OperationData* data_operation, char* arg, unsigned int* operation)
{
	int from_index = 0;
	unsigned char rd = 0, rt = 0, rs = 0, reg;
	if (strcmp(data_operation->name, "move") == 0
		|| strcmp(data_operation->name, "mvhi") == 0
		|| strcmp(data_operation->name, "mvlo") == 0)
	{
		if (GetRegister(&from_index, arg, &rs) == 0) {
			return 0;
		}
		from_index++;
		if (GetRegister(&from_index, arg, &rd) == 0) {
			return 0;
		}
	}
	else
	{
		if (GetRegister(&from_index, arg, &rs) == 0) {
			return 0;
		}
		from_index++;
		if (GetRegister(&from_index, arg, &rt) == 0) {
			return 0;
		}
		from_index++;
		if (GetRegister(&from_index, arg, &rd) == 0) {
			return 0;
		}
	}
	*operation = MakeROperation(data_operation->funct, rd, rt, rs, data_operation->opcode);
	return 1;
}

/*this methode gets instruction , the string of arguments and pointer int to encode inside the instruction
 * and also flag which say wheather this instruction is an conditional brunch operation*/
int OprI(OperationData* data_operation, char* arg, unsigned int* operation, Boolian conditional_branching)
{
	int fromm_index = 0;
	unsigned int addr;
	int immediate;
	unsigned char rt = 0, rs = 0, reg;
	if (GetRegister(&fromm_index, arg, &rs) == 0) return 0;
	fromm_index++;
	if (conditional_branching == True)
	{
		if (GetRegister(&fromm_index, arg, &rt) == 0) return 0;
		immediate = 0;
	}
	else
	{
		if (GetStringsNublers(arg, &fromm_index, ',', &immediate) == 0) return 0;
		fromm_index++;
		if (GetRegister(&fromm_index, arg, &rt) == 0) return 0;
	}
	*operation = MakeIOperation(immediate, rt, rs, data_operation->opcode);
	return 1;
}


/*this method get a jump operation with all the information and handle it*/
int OprJ(OperationData* data_operation, char* arg, unsigned int* operation)
{
	int from_index = 0;
	unsigned int addr;
	unsigned char registr = 0, registem_address;
	if (strcmp(data_operation->name, "stop") != 0 && arg[from_index] == '$')
	{
		registr = 1;
		if (GetRegister(&from_index, arg, &registem_address) == 0) return 0;
		addr = (unsigned int)registem_address;
	}
	else
	{
		addr = 0;
	}
	*operation = MakeJOperation(addr, registr, data_operation->opcode);
	return 1;
}

/*this method get string represent an instruction line
 * in assembly after the instruction line we have the number of the registers operand's or the immediate
 * this function responsible for extracting the arguments from the line and return string with only the arguments*/
char* argSearch(OperationData* Data_opr, char* line_operation)
{
	int now_index = 0;
	lbl_skip(&now_index, line_operation);
	return line_operation + strlen(Data_opr->name) + now_index;
}

/*this method get pointer to operation node and string of  the line(without the label )
 * and it handle all the things to do with the operation such as: extract arguments, analyze arguments and encoding the information to the code segment*/
int HandleOpr(OperationData* data_operation, char* line_operation)
{
	int action_result = 1;
	int from_index = 0;
	char* arg = line_operation;
	unsigned int operation;

	if ((arg = argSearch(data_operation, line_operation)) == NULL) return 0;

	if (data_operation->type == R)
	{
		action_result = OprR(data_operation, arg, &operation);
	}
	else if (data_operation->type == I)
	{
		action_result = OprI(data_operation, arg, &operation, ConditionalBranchingChecking(line_operation));
	}
	else if (data_operation->type == J)
	{
		action_result = OprJ(data_operation, arg, &operation);
	}
	ImageCodeAdding(operation);
	return action_result;
}
/*this methode get an string represent name of the opeartion and check wheather this is brunch operation
 * the reason is that we treat these instruction in other form*/
Boolian ConditionalBranchingChecking(char* operation)
{
	int startIndex = 0;
	SpaceSkipping(&startIndex, operation);
	lbl_skip(&startIndex, operation);
	SpaceSkipping(&startIndex, operation);
	return (strncmp(operation + startIndex, "beq", 3) == 0 ||
		strncmp(operation + startIndex, "bne", 3) == 0 ||
		strncmp(operation + startIndex, "blt", 3) == 0 ||
		strncmp(operation + startIndex, "bgt", 3) == 0) ? True : False;

}
void FreeOprTable()
{
	OprNodePointer now_node = opration_table_head, temp_node;
	while (now_node != NULL)
	{
		temp_node = now_node;
		now_node = now_node->nxt;
		free(temp_node);
	}
	opration_table_head = NULL;
}
/*this method gets a line with type of instruction
 * and check wheather the instruction can has an label as arguments inside*/
Boolian OprContainSymbolChecking(OperationData* data_operation, char* line_operation)
{
	char* arg;

	if ((arg = argSearch(data_operation, line_operation)) == NULL) return 0;
	if (data_operation->type == R)
	{
		return False;
	}
	else if (data_operation->type == I)
	{
		return ConditionalBranchingChecking(line_operation);
	}
	if (alpha(arg[0]))
	{
		return True;
	}
	return False;
}

int EndOppI(char* arg, unsigned int* operation, int now_address)
{
	unsigned int addr;
	int now_index = 0, arg_index = 0;
	char* sym;
	SymDataPointer symbol_pointer;

	while (arg[arg_index] != ',') arg_index++;
	arg_index++;
	while (arg[arg_index] != ',') arg_index++;
	arg_index++;

	sym = GetLable(arg + arg_index);

	if ((symbol_pointer = SymbolCheck(sym)) == NULL || symbol_pointer->attribute == EXTERNAL)
	{
		return 0;
	}
	*operation |= (symbol_pointer->value - now_address) & 0xFFFF;
	return 1;
}

int EndOperationJ(char* arg, unsigned int* operation, int now_address)
{
	unsigned int addr;
	char* sym;
	SymDataPointer symbol_pointer;
	sym = GetLable(arg);

	if ((symbol_pointer = SymbolCheck(sym)) == NULL) return 0;

	if (symbol_pointer->attribute == EXTERNAL)
	{
		ExternalListAdding(symbol_pointer->name, now_address);
	}

	*operation |= symbol_pointer->value;
	return 1;
}

/*this methode complete all the things it should in the second operation*/
int EndSecPassOpr(OperationData* data_operation, char* line_operation, unsigned int* operation, int now_address)
{
	char* arg;

	if ((arg = argSearch(data_operation, line_operation)) == NULL) return 0;

	if (data_operation->type == R)
	{
		return 1;
	}
	else if (data_operation->type == I)
	{
		return EndOppI(arg, operation, now_address);
	}
	else if (data_operation->type == J)
	{
		return EndOperationJ(arg, operation, now_address);
	}
	return 1;
}
/*this function get a string represent name of operation and return the struct which represent the operation*/
OperationData* CheckOpr(char* sgl_type)
{
	OprNodePointer now_node = opration_table_head;

	if (sgl_type == NULL)
	{
		return NULL;
	}

	while (now_node != NULL)
	{
		if (strcmp(now_node->OperationData->name, sgl_type) == 0)
			return now_node->OperationData;
		now_node = now_node->nxt;
	}
	return NULL;
}
/*this methode create R operation
 * that is it get the instruction with all the information and creat unsigned int represent the operation which will add to the code image*/
unsigned int MakeROperation(unsigned char function, unsigned char rd, unsigned char rt, unsigned char rs, unsigned char opcode)
{
	unsigned int opr = 0;
	opr |= (function << 6);
	opr |= (rd << 11);
	opr |= (rt << 16);
	opr |= (rs << 21);
	opr |= (opcode << 26);
	return opr;
}
int GetSymAddr(char* label, unsigned int* adrr, enum OprType oprType)
{
	SymDataPointer symbol_pointer;
	char* s_symbol;



	if ((symbol_pointer = SymbolCheck(label)) == NULL)
	{
		*adrr = 0;
		return 0;
	}
	*adrr = symbol_pointer->value;
	return 1;
}
/*this methode gets information of the I operation and encode the operation to binary and store the the binary representation in unsinged char variable*/
unsigned int MakeIOperation(int immediate, unsigned char rt, unsigned char rs, unsigned char opcode)
{
	unsigned int opr = 0;
	if (immediate >= 0)
	{
		opr |= immediate;
	}
	else
	{
		opr |= (((~(immediate * -1)) + 1) & 0xFFFF);
	}
	opr |= (rt << 16);
	opr |= (rs << 21);
	opr |= (opcode << 26);
	return opr;
}

/*this method gets all the information about jump operation encode it to binary and store it in unsigned char*/
unsigned int MakeJOperation(unsigned int addr, unsigned char registr, unsigned char opcode)
{
	unsigned int opr = 0;
	opr |= addr;
	opr |= (registr << 25);
	opr |= (opcode << 26);
	return opr;
}
