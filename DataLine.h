#pragma once
#define	MAX_DW 2147483647
#define	MIN_DW -2147483648
#define	MAX_DH 32767
#define	MIN_DH -32768
#define	MAX_DB 127
#define	MIN_DW -128
#define	MIN_IN -2147483648
#define	MAX_IN 2147483647
#define MAX_LINE_SIZE 80
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum Boolian
{
	False = 0,
	True = 1
} Boolian;
enum TypeGuildanceLine
{
	UNDEFINED_TYPE_GUIDANCE_LINE, DB, DH, DW, ASCIZ
};

int NumStore(enum TypeGuildanceLine dslType, char* row, int now_index);
int AsciiGuidance(char* row, int now_index);
int ValidityDataChecker(enum TypeGuildanceLine dataType, int data);
int RowGuildence(enum TypeGuildanceLine sglType, char* row);
enum TypeGuildanceLine FindRowDataStore(char* row);
