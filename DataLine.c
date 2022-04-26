#pragma once
#include "DataLine.h"

/*this function gets a line in string and check Whether  this is guidance line if yes return enum which represent the
 * kind of guidance line*/
enum TypeGuildanceLine FindRowDataStore(char* row)
{
	int i = 0;
	lbl_skip(&i, row);
	if (strncmp(row + i, ".db", 3) == 0)
	{
		return DB;
	}
	if (strncmp(row + i, ".dw", 3) == 0)
	{
		return DW;
	}
	if (strncmp(row + i, ".dh", 3) == 0)
	{
		return DH;
	}
	if (strncmp(row + i, ".asciz", 6) == 0)
	{
		return ASCIZ;
	}
	return UNDEFINED_TYPE_GUIDANCE_LINE;
}
/*this methode get a guidance type and return the size it takes in memory image so we will know how much to increase dc
 * the data image pointer */
int GetDataStorageSize(enum TypeGuildanceLine sgl_type)
{
	if (sgl_type == DB)
	{
		return 1;
	}
	else if (sgl_type == DH)
	{
		return 2;
	}
	else if (sgl_type == DW)
	{
		return 4;
	}
	return 0;
}

/*this methode takes as input guidance line and its type (db,dw,dh,sh...)  and treat it accordingly also it get the index after
 * the guidance word (for example after db)*/
int NumStore(enum TypeGuildanceLine dslType, char* row, int from_index)
{
	int fact;
	int now_param;
	int i;
	for (i = from_index; row[i] != '\n' && row[i] != '\0';)
	{
		if (GetStringsNublers(row, &i, ',', &now_param) == 0 || !ValidityDataChecker(dslType, now_param))/*if everything is ok we will store the data in the data image*/
		{
			return 0;
		}
		ImageDataAdding(now_param, GetDataStorageSize(dslType));
		if (row[i] == ',')/*if character after the number which we currently stored is a , character we will infer that there is another number to be stored*/
		{
			i++;
		}
	}
	return 1;
}
/*this method gets a line which is asciz guidanc and it stores the string in the data image as series of characters
 * that is it call the function which adding to the data image with each character in the string*/
int AsciiGuidance(char* row, int from_index)
{
	int i = from_index; /*we assumed that the start index is the index which put as in place after the "asciz" word i.e 5 items from the begining*/
	if (row[from_index] != '\"')/*as long as the current character is not " character it's mean that we are not in the start of the string*/
	{
		return 0;
	}

	for (i = i++; row[i] != '\"'; i++)/*after we got to the first " character we know that this is the starting of the string so we start to write to the data image characther after another*/
	{
		if (row[i] == '\0' || row[i] == '\n') return 0;
		ImageDataAdding(row[i], 1);
	}
	ImageDataAdding('\0', 1);/*as with strings we add end of string character which is \0*/
	return 1;
}
/*here we come if and only if this is guidance line
 * and if so this methode takes as input the type of guidance line , and the string rperesent the line
 * and extract the data to be stored from the line and pass it to methodes which translt it*/
int RowGuildence(enum TypeGuildanceLine dslType, char* row)
{
	int now_param;
	int from_index = 0;
	int fact = 1;

	lbl_skip(&from_index, row);
	if (dslType == DB || dslType == DH ||dslType == DW )/*if the type of the guidance is not asciz we will call the methode handle num storage*/
	{
		from_index += 3;
		return NumStore(dslType, row, from_index);
	}
	from_index += 6;/*increase the index in 3 because it is the length of the guidance word(for example db) + space*/
	return AsciiGuidance(row, from_index);/*calling the according methode which treat it*/
}
/*this methode gets a guidance line type and int variable which stores a number to store defined in the guidance line and check
 * wheather this number is valid to store in the memory perspective
 * for example if we have the number 131,071 which in binary is 111111111 (9 times 1) it takes more than a 1 bite to store it
 * so if we have for example the guidance line .db 131,071 we will return that is not valid*/
int ValidityDataChecker(enum TypeGuildanceLine dataType, int data)
{
	if (dataType == DB)
	{
		return (data >= MIN_DW) && (data <= MAX_DB);
	}
	else if (dataType == DH)
	{
		return (data >= MIN_DH) && (data <= MAX_DH);
	}
	else if (dataType == DW)
	{
		return (data >= MIN_IN) && (data <= MAX_IN);
	}
	return 0;
}
