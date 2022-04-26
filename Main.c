/*MAMAN-14 made by :Omri Drori and Dmitri Golovkin */

#include "asembly.h"
#include "FLibrary.h"
#include "OPRSave.h"

/*this is project is an assemble project we made which takes as an input series of files in assembly language and produce
 *  binary and hexadecimal version of the code*/

 /*here is the main function which takes the strings passed as command line arguments and represent names of files in assembly langauge after that it calls the initialize functions
  * and call the translation function*/
void main(int argc, char* argv[])
{
	FILE* FilePointer;
	char* fPath;
	int i = 1;
	OprTableBuilding();/*here we buld a data structre conatins all the instructions and information about them which we are going to use in project*/
/*here we scan file by file first check if it is really an assembly file by checking the extention and after that we pass the file to the assemble function*/
	while (i < argc) {
		fPath = argv[i];/*the item in the i'th place is the string with the name of the i'th file*/
		if (IsValidExtension(fPath, "as") == 1)/*now before going to process the files let's check that is really an assembly file*/
		{
			if ((FilePointer = fopen(fPath, "r")) != NULL)/*open the first file passed as argument*/
			{
				printf("**the file: %s  is in process now\n", fPath);/*we are going to create another files from this file with the same name but with another extension so here we remove the extension saves the pure name*/

				DelExtensionFromFile(fPath);/*we are going to create another files from this file with the same name but with another extension so here we remove the extension saves the pure name*/

				Assemble(FilePointer, fPath);/*and now to the main part of the program here we really take the files and translate them to a binary*/


				fclose(FilePointer);
			}
			else
			{
				printf("Could not find the file %s\n", fPath);
			}
		}
		else
		{
			printf("The file extension of '%s' isnt valid.\n", argv[i]);
		}
		i++;
	}
	FreeOprTable();
}
