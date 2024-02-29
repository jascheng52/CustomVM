#include <stdlib.h>
#include <registers.h>
#include <allocator.h>
#include <list.h>
#ifndef UTIL_H
#define UTIL_H

char *skipWhite(char *textLine);
char *argAdvanceSkip(char *cursor);
void debug(void *buffer, size_t num);
int checkExtension(char *file);
REGS mapsReg(char *regStr, size_t length);
LABEL_STRUCT *findLabel(NODE *head, char *labelStr, size_t length);
DATA_STRUCT *findData(NODE *head, char *dataStr, size_t length);
int checkNumStr(char *str, size_t length);
size_t wordAlign(size_t size);



#endif /*UTIL_H*/


