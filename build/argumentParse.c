#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <argumentParse.h>
#include <opcode.h>
#include <util.h>

//tries to parse the correct args. Returns char* on end of line, null on any error
char  *findArgs(OPS opType, size_t expNum, char *cursor, char *buffer, size_t bufferBitLength)
{
    char *start = cursor;
    cursor = argAdvanceSkip(cursor);
    if (cursor == NULL)
        return NULL;
    

    size_t parsedLength = cursor - start;
    switch (opType)
    {
    case ADDI:
        
        REGS arg1 = mapsReg(cursor,parsedLength);
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
        REGS arg2 = mapsReg(cursor,parsedLength);
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
        int intAdded = 0;
        int isNeg = 0;
        char *thirdArg = start;
        for(int i = 0; i < parsedLength; i++)
        {
            if(*thirdArg == '-')
            {
                thirdArg++;
                continue;
            }
            intAdded = intAdded * 10;
            intAdded = *thirdArg - 48;
            thirdArg++;
        }
        //figure out how to pack args
        short regArgs = arg1;
        regArgs = regArgs << 4;
        regArgs = regArgs + arg2;


        break;
    
    default:
        break;
    }
    
    

    



    return NULL;
}