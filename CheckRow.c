#include "SymTable.h"



/*this methode get a line from the file and check wheather this is an empty line*/
Boolian IsEmpty(char* row)
{
	int i = 0; /*the index of starting*/
	SpaceSkipping(&i, row);/*skip all the spaces untill end and insert to the index variable the first characther which is not white*/
	return row[i] == '\n';/*if the first character which is not whithe is the end of the line this means all the line was empty so return yes the line is empty line*/
}
/*this function gets a string of the current line and check whether this is a comment line
 * in the assembly language ';' this character in the first place of the string represent comment so check whether there is ; character */
Boolian IsComment(char* row)
{
	int i = 0; /*the index */

	SpaceSkipping(&i, row);/*if there is line like this:"     ;add" this is still comment line so we skip over the spaces in the begging right to the first character which is not white*/

	return row[i] == ';';/*check whether the character in the index which is first not white whether this is comment line*/
}
/*check if it a charactor*/
Boolian alpha(char c)
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
	{
		return True;
	}
	return False;
}
/*check if it is number between 0-9 return 1 if it is to know if it is a number*/
Boolian IsDigit(char c)
{
	if (c >= '0' && c <= '9')
	{
		return 1;
	}
	return 0;
}
/*this function gets a string represent the line and check weather this line is a line which define a label
 * in the assembly language symbol defined in the beginning and have : character after them so we look for : character
 * also in the assembly rules label is not allowed to have numbers or other characters but letters so we also check for
 * validity of the label and if this label is not valid we return false*/
Boolian IsLabel(char* row)
{
	int i = 0;
	if (alpha(row[0]))
	{
		while (IsDigit(row[i]) || alpha(row[i]))/*here we assume the function called with * the line without spaces so we shouldnt call this function again*/
		{
			i++;
		}
		if (row[i] == ':') return 1;
	}
	return 0;
}
/*this methode gets a line and check whether this line is an entery line which is symbol internal defintion
 * and we will use this information when we will write to the different files*/
Boolian IsEntry(char* row)
{
	if (!strncmp(row, ".entry", 6))
	{
		return 1;
	}
	return 0;
}
/*this function gets a line and check if this is an extern line
 * we will use this information after the seconde pass when we will write to the files*/
Boolian IsExtern(char* row)
{
	if (!strncmp(row, ".extern", 7))
	{
		return 1;
	}
	return 0;
}


/*this methode takes as input the numbers which stated in guidance line as characters in string and return as int*/
int GetStringsNublers(char* sgl_type, int* the_current_index, char end_flag, int* res)
{
	int idx = *the_current_index;
	int the_current_parameter = 0;
	int fac = 1;
	int i, j;
	if (sgl_type[idx] == '-' || sgl_type[idx] == '+')
	{
		if (sgl_type[idx] == '-') {
			fac = -1;
		}
		else {
			fac = 1;
		}
		
		idx++;
	}
	
	while (sgl_type[idx] >= '0' && sgl_type[idx] <= '9')
	{
		the_current_parameter *= 10;
		the_current_parameter += sgl_type[idx] - '0';
		idx++;
	}
	the_current_parameter *= fac;
	if (sgl_type[idx] != end_flag && sgl_type[idx] != '\0' && sgl_type[idx] != '\n')
	{
		return 0;
	}
	
	*res = the_current_parameter;
	*the_current_index = idx;
	return 1;
}

/*this methode gets a line from the file and look for the label (i.e before the : charachter) and return label in string*/
char* GetLable(char* row)
{
	char* label = (char*)calloc(LABLE_SIZE, sizeof(char));
	int the_current_index;

	for (the_current_index = 0; alpha(row[the_current_index]) == True || IsDigit(row[the_current_index]); the_current_index++) {/*labels should contain only digits or letters*/
		/*if we got to the end of line before the end of label this is not valid*/
		if (row[the_current_index] == '\n' || row[the_current_index] == '\0') {
			return NULL;
		}
		if ( row[the_current_index] == '\0') {
			return NULL;
		}
		label[the_current_index] = row[the_current_index];
	}
	label[the_current_index] = '\0';
	return label;
}

char* GetString(char* sgl_type)
{
	char* string_without_spaces = (char*)calloc(MAX_LINE_SIZE, sizeof(char));
	strcpy(string_without_spaces, sgl_type);
	int current_index = 0;
	int index_to_write = 0;
	while (string_without_spaces[current_index] != '\n' && string_without_spaces[current_index] != '\0')
	{
		if (string_without_spaces[current_index] != '\t' && string_without_spaces[current_index] != ' ')
		{
			string_without_spaces[index_to_write] = string_without_spaces[current_index];
			index_to_write++;
		}
		current_index++;
	}
	string_without_spaces[index_to_write] = '\0';
	return string_without_spaces;
}

/*this function gets an int pointer index and the string represent the line
 * and run on the lind and find the index of the first character which is not space
 * and put this index in the index pointer
 * the motivation to use pointer of index is that we want to use it in all the program not just in this function like a local variable*/
void SpaceSkipping(int* i, char* row)
{
	int current_index = *i;
	current_index = *i;/*i is index*/
	while ((row[current_index] == '\t' || row[current_index] == ' ') && row[current_index] != '\n' && row[current_index] != '\0')
	{
		current_index++;
	}
	*i = current_index;
}
/*this method gets string represent line from the file which we will assume that the line is an instruction line
 * and this method extract the name of the instruction defined in the line*/
char* GetNameOfOperation(char* row)
{
	int i;
	char* operand_name = (char*)calloc(5, sizeof(char));
	int the_start_index = 0;
	
	

	SpaceSkipping(&the_start_index, row);/*skip all the spaces in the begging until the label if there is label*/
	lbl_skip(&the_start_index, row);/*if there is label - skip it , and if there is not label dont do nothing*/
	SpaceSkipping(&the_start_index, row);/*skip from the label until the first word - by the rules this is should be the instruction*/

	for (i = 0; row[the_start_index] != ' ' && row[the_start_index] != '\n' && row[the_start_index] != '\0'; the_start_index++, i++) {/*here we come when we are in the starting of the instruction*/
		if (row[the_start_index] < 'a' || row[the_start_index] > 'z') return NULL;
		operand_name[i] = row[the_start_index];/*so we copy character one after another untill we get the the end of the word*/
	}
	operand_name[the_start_index] = '\0';
	return operand_name;
}

/*this methode get a string represent entery/external line .
 * these lines defined labels.
 * here we use this methode to extract the label which defined, put it in a string and return the string
 * big emphasize!!: this methode assumes that start index is the place after the entery\extern word
 * so for example if we have .externwNumber the start index is 7 and not from the beginning of the line*/
char* GetEntryLableOrExternLable(int where_to_start, char* row)
{
	char* label = (char*)calloc(LABLE_SIZE, sizeof(char));
	int the_current_place = where_to_start;
	int i = 0;

	if (!alpha(row[where_to_start]))
	{
		return NULL;
	}

	while (row[the_current_place] != '\n' && row[the_current_place] != '\0') {/*extren\entry lines has only definition of label and nothing more so we run until the end of the line*/
		if (!alpha(row[the_current_place]) && !IsDigit(row[the_current_place])) return NULL;
		label[i] = row[the_current_place];
		the_current_place++;
		i++;
	}

	label[i] = '\0';
	return label;
}
/*this method gets an index pointer and string represent line which has definition of label in it
 * and it look for the and of the label name i.e after the : character , and put in the variable index the index of the character after the : character*/
void lbl_skip(int* i, char* row)
{
	int the_current_index = *i;/*start looking from the current index usually is the start of the line*/
	if (!IsLabel(row)) {/*if there is no definition of label we return without change the value in the index pointer- meaning after label is in the place without change*/
		return;
	}
	while (row[the_current_index] != ':' && row[the_current_index] != '\n' && row[the_current_index] != '\0')/*run as long as it is not ':' and increase counter- here we come only if there is label in the line so it is promised to stop*/
	{
		the_current_index++;
	}
	the_current_index++;
	*i = the_current_index;/*change the value in the index pointer*/
}
