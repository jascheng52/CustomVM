#include <stdio.h>
#include <allocator.h>
#include <opcode.h>
#include <registers.h>

#ifndef ASSEM_H
#define ASSEM_H

#define EXTEN_CMP 10
#define MAX_LINE_LENGTH 1024
#define UPPER_INST_LENGTH 15

int assemble(char *file);
int checkExtension(char *file);
int createFile(char *file, char *buffer, size_t size);
int parseFile(FILE *openFile, char *buffer);
int parseLine(char *lineBuffer, char *currLine, int *instrNum);
int getNextToken(char *lineBuffer);
char *getInstruct(char *cursor, INSTR_STRUCT **parsedIns);

void init();

void debug(void *buffer, size_t num);

#endif /*ASSEM.H*/