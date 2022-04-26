#include "FLibrary.h"

char* AddExtension(char* fName, char* ext)
{
	char* new_file_name;
	char* fname_length, length_of_extension;
	fname_length = strlen(fName);
	length_of_extension = strlen(ext);
	new_file_name = calloc(fname_length + length_of_extension + 1, sizeof(char));
	strcpy(new_file_name, fName);
	strncat(new_file_name, ext, length_of_extension);
	return new_file_name;
}

void WriteToObjectFile(char* fname, unsigned char* data, int icf, int dcf)
{
	FILE* fs;
	char* new_file_name;
	int opr_index;
	int data_size = icf + dcf, data_index;
	new_file_name = AddExtension(fname, ".ob");
	fs = fopen(new_file_name, "w");
	fprintf(fs, " %7d %7d \n", icf - 100, dcf);

	data_index = 100;
	while (data_index < data_size)
	{
		fprintf(fs, "%04d", data_index);
		for (opr_index = 0; opr_index < 4 && data_index < data_size; opr_index++)
		{
			fprintf(fs, " %02X", data[data_index++]);
		}
		fprintf(fs, "\n");
	}
	fclose(fs);
}
void WriteToExternalFile(char* fname, ExternalNodePointer ext_node)
{
	FILE* fs;
	char* new_file_name;
	new_file_name = AddExtension(fname, ".ext");
	if (ext_node == NULL) return;
	fs = fopen(new_file_name, "w");
	while (ext_node != NULL)
	{
		fprintf(fs, "%s %04d\n", ext_node->data->symbol, ext_node->data->address);
		ext_node = (*ext_node).nxt;
	}
	fclose(fs);
}

void WriteToEntryFile(char* fname, SymNodePointer sym_table_head)
{
	FILE* fs;
	char* new_file_name;
	new_file_name = AddExtension(fname, ".ent");
	if (Entrycounter() == 0) return;
	fs = fopen(new_file_name, "w");
	SymNodePointer now_node = sym_table_head;

	while (now_node != NULL)
	{
		if (now_node->symbolData->isEntry)
		{
			fprintf(fs, "%s %04d\n", now_node->symbolData->name, now_node->symbolData->value);
		}
		now_node = (*now_node).nxt;
	}
	fclose(fs);
}
/*this function gets as an input string represent a name of a file and check wheather this is really an assembly file by checking the extention
 * an assembly file should end with ".as" extenstion so we pass */
Boolian IsValidExtension(char* fname, char* the_extension_we_need)
{
	char* ext = strrchr(fname, '.');
	if (!ext || strcmp(ext + 1, the_extension_we_need) != 0)
	{
		return False;
	}
	return True;
}
/*we are going to build ob file extern file and entery file each one has a different extention so we remove the current extention and
 * leave a pure name of the file for later purpuses*/
char* DelExtensionFromFile(char* fname)
{
	char* where_extension;
	where_extension=strrchr(fname, '.');/*find where is the extention begin - as the rules in operation systems extention begain with an . character*/
	*where_extension = '\0';/*replace the dot character with end of string character and as consequance the rest of the string (the extention part) will be removed*/
}
