#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <argumentParse.h>
#include <opcode.h>
#include <util.h>

//tries to parse the correct args. Returns char* on end of line, null on any error
char  *findArgs(OPS opType, size_t expNum, char *cursor, char *buffer, size_t bufferBitLength)
{
    cursor = skipWhite(cursor);
    char *start = cursor;
    cursor = argAdvanceSkip(cursor);
    if (cursor == NULL)
        return NULL;
    

    size_t parsedLength = cursor - start;
    switch (opType)
    {
    case ADDI:
        
        REGS arg1 = mapsReg(start,parsedLength);
        if(arg1 == NA_REG)
        {
            fprintf(stderr, "Expected register as first argument\n");
            return NULL;
        }
        cursor = skipWhite(cursor);
        if(*cursor == '\n')
        {
            fprintf(stderr, "Expected register as second argument\n");
            return NULL;
        }

        start = cursor;
        cursor = argAdvanceSkip(cursor);
        if(cursor == NULL) 
            return NULL;
        
        parsedLength = cursor - start;
        REGS arg2 = mapsReg(start,parsedLength);
        if(arg2 == NA_REG)
        {
            fprintf(stderr, "Expected register as second argument\n");
            return NULL;
        }
        cursor = skipWhite(cursor);
        if(*cursor == '\n')
        {
            fprintf(stderr, "Expected register as second argument\n");
            return NULL;
        }
        start = cursor;
        cursor = argAdvanceSkip(cursor);
        if(cursor == NULL) 
            return NULL;
        parsedLength = cursor - start;
        int32_t intAdded = 0;
        int negScaler = 1;
        char *thirdArg = start;
        if(*thirdArg == '-')
        {
            thirdArg++;
            negScaler = -1;
            parsedLength--;
            if(parsedLength == 0)
            {
                fprintf(stderr, "Expected Number");
                return NULL;
            }
        }
        for(int i = 0; i < parsedLength; i++)
        {
            if(*thirdArg > 47 && *thirdArg < 58)
            {
                intAdded = intAdded * 10;
                intAdded = *thirdArg - 48;
                thirdArg++;
            }
            else
            {
                fprintf(stderr, "Expected Number");
                return NULL;
            }
        }
        intAdded = intAdded * negScaler;
        unsigned short argResults = 0;
        argResults = argResults || arg1;
        argResults = argResults << 4;
        argResults = argResults || arg2;

        memcpy(buffer,&argResults ,sizeof(unsigned short));
        memcpy(buffer + 2, &intAdded, 4);

        for(int i = 0; i < 3; i++)
        {
            unsigned char c = ((char *)(buffer))[i];
            fprintf(stderr,"%0.2x", c);
            fflush(stderr);
        }
        fprintf(stderr,"\n");
        return cursor;
        break;
    
    default:
        break;
    }
    
    

    



    return NULL;
}