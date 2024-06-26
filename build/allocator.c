#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <allocator.h>
#include <list.h>
#include <util.h>

//Mallocs a data struct and fills size fields.NOT DATA. Callers responsible to free. Null if fail
DATA_STRUCT *ALLO_mallocData(int dataNameSize, int dataSize)
{
    DATA_STRUCT *newData = malloc(sizeof(*newData) + (dataNameSize + dataSize));
    newData->dataNameSize = dataNameSize;
    newData->dataSize = dataSize;
    newData->isInt = 0;
    newData->index = 0;
    return newData;
}

//Returns size of datastuct by size
size_t ALLO_dataStructSize(DATA_STRUCT *dataStruct)
{
    if(dataStruct == NULL)
        return 0;
    return sizeof(DATA_STRUCT) + dataStruct->dataSize + dataStruct->dataNameSize;
} 

//Checks if data name is in list. Returns 1 on true
int ALLO_checkDataRep(NODE* listHead, char *dataName, size_t dataLength)
{
    if(listHead == NULL || dataName == NULL || dataLength == 0)
        return 0;
    
    NODE *cursor = listHead->next;
    // LIST_print(listHead);
    while(cursor != listHead)
    {
        if(cursor == NULL)
        {
            fprintf(stderr,"Data List corrupted\n");
            exit(EXIT_FAILURE);
        }
        DATA_STRUCT *data = (DATA_STRUCT *)(cursor->data);
        if(data->dataNameSize != dataLength)
            return 0;
        if(strncmp(dataName,data->data, dataLength) == 0)
            return 1;
        cursor = cursor->next;
    }
    
    return 0;
}


//Mallocs a label struct. Callers responsible to free. Null if fail
LABEL_STRUCT *ALLO_mallocLabel(size_t labelSize)
{
    LABEL_STRUCT *newLabel = malloc(sizeof(*newLabel) + labelSize);
    newLabel->labelSize = labelSize;
    newLabel->instRef = 0;
    return newLabel;
}

//Returns size of datastuct by size
size_t ALLO_labelStructSize(LABEL_STRUCT *labelStruct)
{
    if(labelStruct == NULL)
        return 0;
    return sizeof(LABEL_STRUCT) + labelStruct->labelSize;
}


int ALLO_checkLabelRep(NODE* listHead, char *labelName, size_t labelLength)
{
    if(listHead == NULL || labelName == NULL || labelLength == 0)
        return 0;
    NODE *cursor = listHead->next;
    // LIST_print(listHead);
    while(cursor != listHead)
    {
        if(cursor == NULL)
        {
            fprintf(stderr,"Label List corrupted\n");
            exit(EXIT_FAILURE);
        }
        LABEL_STRUCT *data = (LABEL_STRUCT *)(cursor->data);
        if(strncmp(labelName,data->label, labelLength) == 0)
            return 1;
        cursor = cursor->next;
    }
    return 0;
}

//Mallocs a instruction struct. Callers responsible to free. Null if fail
INSTR_STRUCT *ALLO_mallocInstr(OPS opType,size_t argLength)
{
    INSTR_STRUCT *newStruct = malloc(sizeof(*newStruct) + argLength);
    newStruct->opType = opType;
    newStruct->argSizeBytes = argLength;
    memset(newStruct->args,0,argLength);
    
    return newStruct;
}


