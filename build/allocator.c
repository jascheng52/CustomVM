#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <allocator.h>

//Mallocs a data struct and fills size fields.NOT DATA. Callers responsible to free. Null of fail
DATA_STRUCT *mallocData(int dataNameSize, int dataSize)
{
    DATA_STRUCT *newData = malloc(sizeof(DATA_STRUCT) + sizeof(char)*(dataNameSize + dataSize));
    newData->dataNameSize = dataNameSize;
    newData->dataSize = dataSize;
    return newData;
}

//Returns size of datastuct by size
size_t dataStructSize(DATA_STRUCT *dataStruct)
{
    if(dataStruct == NULL)
        return 0;
    return sizeof(DATA_STRUCT) + dataStruct->dataSize + dataStruct->dataNameSize;
}

//Mallocs a label struct. Callers responsible to free. Null of fail
LABEL_STRUCT *mallocLabel(size_t labelSize)
{
    LABEL_STRUCT *newLabel = malloc(sizeof(LABEL_STRUCT) + sizeof(char)*(labelSize));
    return newLabel;
}

//Returns size of datastuct by size
size_t labelStructSize(LABEL_STRUCT *labelStruct)
{
    if(labelStruct == NULL)
        return 0;
    return sizeof(LABEL_STRUCT) + labelStruct->labelSize;
}





