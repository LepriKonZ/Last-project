#include "asembly.h"
#include "OPRSave.h"
#include "FLibrary.h"


unsigned char* code_img;
unsigned char* data_img;
int now_line_index;
int IC;
int DC;
/*get current IC*/
int getIC() {
	return IC;
}
/*get current DC*/
int getDC() {
	return DC;
}

/*this method initialize all the properties of the memory image of the program*/
void ImagesMakeToNull()
{
	code_img = NULL;
	data_img = NULL;
	IC = 100, DC = 0;

}
/*free the data images*/ 
void FreeImg()
{
	if (data_img != NULL)
		free(data_img);

	if (code_img != NULL)
		free(code_img);

}

/*this methode get the memory image and return from it the adress of the symbol*/
unsigned int GetInfoFromMem(int from_index, unsigned char* mem_img)
{
	unsigned int mem_val = 0;
	int i = from_index + 3;

	while (i >= from_index) {
		mem_val *= 256;
		mem_val += mem_img[i] % 256;
		i--;
	}
	return mem_val;
}

/*this function is implementation of the first pass assemble
 * it gets a pointer to the file and scan the file by read each line from the file and analyze the line and pass it to
 * functions which process the line and translate it */
int Frun(FILE* fp)
{
	char now_line[MAX_LINE_SIZE];/*maximum size of the line*/
	char* no_space_in_line;
	enum TypeGuildanceLine dsl_type;
	OperationData* data_operation;
	char* operation_name;
	Boolian isBuildFailed = False, lable_line;
	
	ImagesMakeToNull();
	now_line_index = 1;
	/*here in the loop we scan line by line from the file and process*/
	while (fgets(now_line, MAX_LINE_SIZE, fp) != NULL) {
			lable_line = False;
			no_space_in_line = GetString(now_line);
			/*check if the current line is an empty line or comment line if yes - skip on it*/
			if (IsEmpty(now_line)) {
				continue;
			}

			if (IsComment(now_line)) {
				continue;
			}
			/*check if there is label defined in the current line if yes turn on the  flag*/
			if (IsLabel(no_space_in_line))
				lable_line = True;
			
			/*check if this line is an guidance line if yes treat it accordinaly*/
			if ((dsl_type = FindRowDataStore(no_space_in_line)) != UNDEFINED_TYPE_GUIDANCE_LINE)
			{
				if (lable_line)
					SymAdding(GetLable(no_space_in_line), DC, DATA, now_line_index);

				if (!RowGuildence(dsl_type, no_space_in_line))
				{
					isBuildFailed = True;
					printf("\tLine %d: error, unvalid parameters.\n", now_line_index);
				}
				continue;
			}
			/*if this is an entery line we will continue because this will treat in the secend pass */
			if (IsEntry(no_space_in_line)) {/*here we check weather this is an entery line which get different treatment in the second pass*/
				continue;
			}
			if (IsExtern(no_space_in_line))/*check if this is extrnal line */
			{
				if (!ExtHandle(no_space_in_line, now_line_index))
				{
					isBuildFailed = True;
					printf("\tLine %d: error, unvalid lable.\n", now_line_index);
				}
				continue;
			}
			/*if we got here we will know that this is an instruction line so we will treat it accordingly */
			if (lable_line)
				SymAdding(GetLable(now_line), getIC(), CODE, now_line_index);/*if there is label definition in the line we will add the symbol with code attribute*/

			if ((operation_name = GetNameOfOperation(now_line)) == NULL)
			{
				isBuildFailed = True;
				printf("\tLine %d: error, opr name is'nt valid!\n", now_line_index);
				continue;
			}
			if ((data_operation = CheckOpr(operation_name)) == NULL)/*after we have the operation name we look for the node in the operation linked list which is point to the struct represent the operation*/
			{
				isBuildFailed = True;
				printf("\tLine %d: error, the opr \"%s\" is'nt exist!\n", now_line_index, operation_name);
			}
			else if (!HandleOpr(data_operation, no_space_in_line))
			{
				isBuildFailed = True;
				printf("\tLine %d: error, check the opr arguments.\n", now_line_index, now_line);
			}
		
		now_line_index++;
		free(no_space_in_line);
	}

	if (isBuildFailed == 0) {
		return 1;
	}
	
	return 0;	
}
/*this methode implement the seconde pass assembly it pass on look for symbols which didnt encoded and encode them*/
int Srun(FILE* fp, char* fName, unsigned char* mem_img)
{
	int ic = 100;
	OperationData* OperationData;
	unsigned int now_operation;
	char* no_space_line;
	char* name_operation;
	Boolian sec_pass_err = False;
	char now_line[MAX_LINE_SIZE];
	now_line_index = 1;
	while(fgets(now_line, MAX_LINE_SIZE, fp) != NULL)
	{
		no_space_line = GetString(now_line);
		if (IsEmpty(now_line) || IsComment(now_line)) continue;
		if (FindRowDataStore(no_space_line) != UNDEFINED_TYPE_GUIDANCE_LINE || IsExtern(now_line))
		{
			continue;
		}
		else if (IsEntry(no_space_line))
		{
			if (!EndHandle(no_space_line, now_line_index))
			{
				sec_pass_err = True;
				printf("\tLine %d: error, unvalid lable.\n", now_line_index);
			}
			continue;
		}
		name_operation = GetNameOfOperation(now_line);
		OperationData = CheckOpr(name_operation);

		if (OprContainSymbolChecking(OperationData, no_space_line))
		{
			now_operation = GetInfoFromMem(ic, mem_img);
			if (EndSecPassOpr(OperationData, no_space_line, &now_operation, ic) == 0)
			{
				sec_pass_err = True;
				printf("\tLine %d: error - '%s', the lable isnt exist.\n", now_line_index, now_line);
			}
			else
			{
				ImageMemoryAdding(now_operation, mem_img, ic);
			}
		}
		ic += 4;
		now_line_index++;
		free(no_space_line);
	}
	if (sec_pass_err == 0) {
		return 1;
	}

	return 0;
}

void Free()
{
	SymTabFree();
	ExtrnListFree();
	FreeImg();
}

unsigned char* MakingMemoryImage(int icf, int dcf)
{
	int mem_len = icf + dcf, now_index, num;
	unsigned char* mem_img = (char*)malloc(sizeof(char) * mem_len);
	unsigned char mem;
	now_index = 100;

	while (now_index < icf) {
		num = code_img[now_index];
		mem_img[now_index] = code_img[now_index];
		now_index++;
	}
	now_index = 0;
	while (now_index < dcf) {
		mem_img[now_index + icf] = data_img[now_index];
		now_index++;
	}
	return mem_img;
}

void Assemble(FILE* fp, char* fName)
{
	int ICF;/*these are the the counter on the data and instructions tabels */
	int DCF;
	unsigned char* memory_img;

	if (Frun(fp) == 0)
	{
		Free();
		return;
	}

	ICF = getIC();
	DCF = getDC();
	ICaddingToSymbols(ICF);
	fseek(fp, 0, SEEK_SET);
	memory_img = MakingMemoryImage(ICF, DCF);

	if (Srun(fp, fName, memory_img) == 1)
	{
		WriteToObjectFile(fName, memory_img, ICF, DCF);
		WriteToEntryFile(fName, GetTableSymbolHead());
		WriteToExternalFile(fName, GetListExternalHead());
	}

	free(memory_img);
	Free();
}





/*this methode gets a number and size to increase the dc (the amount in memory we allocated) and adds the number the data image and also increase dc*/
void ImageDataAdding(int number, int s)
{
	int i = 0;
	int index_shift = 0;
	while (i < s) {
		data_img = (unsigned char*)realloc(data_img, (DC + 1) * sizeof(unsigned char));
		data_img[getDC()] = (unsigned char)((number >> index_shift) & 0xFF);
		DC++;
		index_shift += 8;
		i++;
	}
}

/*this methode get an unsigned int represent instruction in binary representation and adds it to the code image*/
void ImageCodeAdding(unsigned int operation)
{
	int i = 0;
	int shift_indx = 0;

	while (i < 4) {
		code_img = (char*)realloc(code_img, (IC + 1) * sizeof(unsigned char));
		code_img[getIC()] = (unsigned char)((operation >> shift_indx) & 0xFF);
		IC++;
		shift_indx += 8;
		i++;
	}
}

void ImageMemoryAdding(unsigned int data, unsigned char* mem_img, int from_index)
{
	int i = from_index;

	while (i < from_index + 4) {
		mem_img[i] = data % 256;
		data /= 256;
		i++;
	}
}


