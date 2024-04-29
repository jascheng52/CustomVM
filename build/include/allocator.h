#include <stdlib.h>
//the name/size for data. Then name is stored in data null terminated a
//and then following is the rest of the data
#ifndef ALLO_H
#define ALLO_H


#include <list.h>
#include <opcode.h>

//Structure for instruction, note argsSize must be 32 bit word aligned
typedef struct inst{
    OPS opType;
    size_t index;
    size_t argSizeBytes;
    char args[];
}INSTR_STRUCT;


//Data name stored first then data
typedef struct data{
    int isInt;
    size_t index;
    size_t dataNameSize;
    size_t dataSize;
    char   data[];
}DATA_STRUCT;

typedef struct label{

    size_t labelSize;
    INSTR_STRUCT *instRef;
    char label[];
}LABEL_STRUCT;


DATA_STRUCT *ALLO_mallocData(int dataNameSize, int dataSize);
size_t ALLO_dataStructSize(DATA_STRUCT *dataStruct);
LABEL_STRUCT *ALLO_mallocLabel(size_t labelSize);
size_t ALLO_labelStructSize(LABEL_STRUCT *labelStruct);
int ALLO_checkDataRep(NODE* listHead, char *dataName, size_t dataLength);
int ALLO_checkLabelRep(NODE* listHead, char *labelName, size_t labelLength);
INSTR_STRUCT *ALLO_mallocInstr(OPS opType, size_t argLength);


#endif /*ALLO.H*/



