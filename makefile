
run: Main.o OPRSave.o SymTable.o asembly.o DataLine.o FLibrary.o CheckRow.o
	gcc -g -Wall -pedantic -ansi Main.o OPRSave.o SymTable.o asembly.o DataLine.o FLibrary.o CheckRow.o -lm -o run
Main.o: Main.c 
	gcc -c -Wall -pedantic -ansi Main.c -o Main.o
OPRSave.o: OPRSave.c OPRSave.h
	gcc -c -Wall -pedantic -ansi OPRSave.c -o OPRSave.o
SymTable.o: SymTable.c SymTable.h
	gcc -c -Wall -pedantic -ansi SymTable.c -o SymTable.o
asembly.o: asembly.c asembly.h
	gcc -c -Wall -pedantic -ansi asembly.c -o asembly.o
DataLine.o: DataLine.c DataLine.h
	gcc -c -Wall -pedantic -ansi DataLine.c -o DataLine.o
FLibrary.o: FLibrary.c FLibrary.h
	gcc -c -Wall -pedantic -ansi FLibrary.c -o FLibrary.o
CheckRow.o: CheckRow.c
	gcc -c -Wall -pedantic -ansi CheckRow.c -o CheckRow.o





