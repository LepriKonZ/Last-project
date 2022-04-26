#pragma once
#include "DataLine.h"

void ImageDataAdding(int number, int s);

void ImageCodeAdding(unsigned int operation);

void ImageMemoryAdding(unsigned int data, unsigned char* mem_img, int from_index);

int Frun(FILE* fp);

int Srun(FILE* fp, char* fname, unsigned char* mem_img);

void Assemble(FILE* fp, char* fName);
