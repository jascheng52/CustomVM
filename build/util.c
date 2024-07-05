#include <stdlib.h>
#include <string.h>
#include <assembler.h>
#include <registers.h>
#include <list.h>
#include <allocator.h>

//Skips white space not include \n
char *skipWhite(char *textLine)
{
    char *cursor = textLine;
    while(*cursor == ' ' || *cursor == '\t')
    {
        cursor++;
    }
    return cursor;
}

char *argAdvanceSkip(char *cursor)
{
    while(*cursor != '\n')
    {
        if(*cursor == ' ' || *cursor == '\t' || *cursor == '\0')
           break;
        cursor++;
    }

    return cursor;
}

void debug(void *buffer, size_t num)
{
        char temp[num +1];
        memcpy(temp,buffer,num);
        temp[num] = '\0';
        printf("%p:%s\n",buffer,temp);
}

int checkExtension(char *file)
{
    int length, index;
    char c = '\0';
    length = strlen(file);
    index = length;

    while(index > 0)
    {
        if(file[index] == '.')
        {
            c = file[index];
            break;
        }
        index--;
    }
    if(c == '\0')
        return -1;
    if(length-index != 5)
        return -1;
    if(!strncmp(&file[index],".jasm", EXTEN_CMP))
        return 1;

    return -1;
}

//Print num bytes from buffer as hex
void printBytesFromBuffer(void *buffer, size_t numBytes)
{
    for(int i = 0; i < numBytes; i++)
    {
        unsigned char c = ((char *)(buffer))[i];
        fprintf(stderr,"%0.2x", c);
        fflush(stderr);
    }
    fprintf(stderr, "\n");
}

//Checks if str is all numbers. 1 if all numbers
int checkNumStr(char *str, size_t length)
{
    for(int i = 0; i < length; i++)
    {
        if(*str < '0' || *str > '9')
            return 0;
    }
    return 1;
}

//Returns register enum based on regStr. NA is no match found
REGS mapsReg(char *regStr, size_t length)
{
    for(int i = 0; i < NUM_REG; i++)
    {
        size_t listLength = strnlen(VALID_REG[i], UPPER_INST_LENGTH);
        if(listLength == length)
        {
            // printf("parsed string: %.*s -- valid: %s\n", listLength,regStr, VALID_REG[i]);
            if(strncmp(VALID_REG[i], regStr, listLength) == 0)
                return i;
        }
    }
    return NA_REG;
}

//Returns label struct in list that matches label. NULL none found
LABEL_STRUCT *findLabel(NODE *head, char *labelStr, size_t length)
{
    NODE *currNode = head->next;
    char compareString[length + 1];
    memcpy(compareString + 1, labelStr, length);
    compareString[0] = '#';
    while(currNode != head)
    {
        LABEL_STRUCT *labelStruct = (LABEL_STRUCT *)currNode->data;
        if(length + 1 == labelStruct->labelSize)
        {
            char res[labelStruct->labelSize];
            memcpy(res,labelStruct->label,labelStruct->labelSize);
            if(strncmp(compareString,res,length + 1) == 0)
                return labelStruct;

        }
        currNode = currNode->next;
    }
    return NULL;

}

//Returns data struct in list that matches data name. NULL none found
DATA_STRUCT *findData(NODE *head, char *dataStr, size_t length)
{
    NODE *currNode = head->next;
    char compareString[length + 1];
    memcpy(compareString + 1, dataStr, length);
    while(currNode != head)
    {
        if(currNode->type == BLOCK)
            compareString[0] = '~';
        else
            compareString[0] = '.';

        DATA_STRUCT *dataStruct = (DATA_STRUCT *)currNode->data;
        if(length + 1 == dataStruct->dataNameSize)
        {
            char res[dataStruct->dataNameSize];
            memcpy(res,dataStruct->data,dataStruct->dataNameSize);
            if(strncmp(compareString, res,length + 1) == 0)
                return dataStruct;
        }
        currNode = currNode->next;
    }
    return NULL;

}

//aligns size to 32 bit
size_t wordAlign(size_t size)
{
    size_t offset = size % WORD_LENGTH;
    return size + WORD_LENGTH - offset;
}
