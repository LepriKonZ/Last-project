#pragma once
#include "DataLine.h"
#include "SymTable.h"

void WriteToObjectFile(char *fname, unsigned char *data, int icf, int dcf);

void WriteToExternalFile(char *fname, ExternalNodePointer externalNode);

void WriteToEntryFile(char* fname, SymNodePointer symbolTableHead);

Boolian IsValidExtension(char* fname, char* ExtWeNeed);

char* DelExtensionFromFile(char* fname);
