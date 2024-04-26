#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <argumentParse.h>
#include <opcode.h>
#include <util.h>
#include <list.h>

//tries to parse the correct args. Returns char* on end of line, null on any error
char  *findArgs(OPS opType, char *cursor, char *buffer, NODE *labelList)
{
    REG_ARG_TYPE expectedARG = REG_TYPE_MAP[opType];

    switch (expectedARG)
    {
        case NO_ARG:
        {
            return cursor;
        }

        case N:
        {
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start;

            int32_t intAdded = 0;
            int negScaler = 1;
            char *firstArg = start;
            if(*firstArg == '-')
            {
                firstArg++;
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
                if(*firstArg > 47 && *firstArg < 58)
                {
                    intAdded = intAdded * 10;
                    intAdded = intAdded + *firstArg - 48;
                    firstArg++;
                }
                else
                {
                    fprintf(stderr, "Expected Number");
                    return NULL;
                }
            }
            intAdded = intAdded * negScaler;

            //4 bytes number
            memcpy(buffer, &intAdded, 4);

            return cursor;
        }

        case R:
        {
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start;

            REGS arg1 = mapsReg(start,parsedLength);
            if(arg1 == NA_REG)
            {
                fprintf(stderr, "Expected register as first argument\n");
                return NULL;
            }
            
            unsigned short argResults = 0;
            argResults = argResults | arg1;
            //one byte register 
            memcpy(buffer,&argResults , 1);

            return cursor;
        }

        case L:
        {
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start;
            char labelName[parsedLength+1];
            memcpy(labelName,start,parsedLength);
            labelName[parsedLength] = '\0';
            void *res = findLabel(labelList,labelName,parsedLength);
            if(res == NULL)
            {
                fprintf(stderr,"Failed to find label %s declared\n", labelName);
                return NULL;
            }

            //stores the POINTER of the label struct, dereference 
            //to get label struct pointer
            memcpy(buffer,&res , sizeof(res));

            return cursor;
        }

        case R_N:
        {
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start;

            REGS arg1 = mapsReg(start,parsedLength);
            if(arg1 == NA_REG)
            {
                fprintf(stderr, "Expected register as first argument\n");
                return NULL;
            }
            cursor = skipWhite(cursor);
            if(*cursor == '\n' | *cursor == '\0')
            {
                fprintf(stderr, "Expected number as second argument\n");
                return NULL;
            }

            start = cursor;
            cursor = argAdvanceSkip(cursor);   
            parsedLength = cursor - start;
            
            int32_t intAdded = 0;
            int negScaler = 1;
            char *secondArg = start;
            if(*secondArg == '-')
            {
                secondArg++;
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
                if(*secondArg > 47 && *secondArg < 58)
                {
                    intAdded = intAdded * 10;
                    intAdded = intAdded + *secondArg - 48;
                    secondArg++;
                }
                else
                {
                    fprintf(stderr, "Expected Number");
                    return NULL;
                }
            }
            intAdded = intAdded * negScaler;
            unsigned short argResults = 0;
            argResults = argResults | arg1;

            //one byte register 4 for number
            memcpy(buffer,&argResults , 1);
            memcpy(buffer + 1, &intAdded, 4);

            return cursor;
        }

        case R_R:
        {        
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start;    

            REGS arg1 = mapsReg(start,parsedLength);
            if(arg1 == NA_REG)
            {
                fprintf(stderr, "Expected register as first argument\n");
                return NULL;
            }
            cursor = skipWhite(cursor);
            if(*cursor == '\n' || *cursor == '\0')
            {
                fprintf(stderr, "Expected register as second argument\n");
                return NULL;
            }

            start = cursor;
            cursor = argAdvanceSkip(cursor);
            parsedLength = cursor - start;

            REGS arg2 = mapsReg(start,parsedLength);
            if(arg2 == NA_REG)
            {
                fprintf(stderr, "Expected register as second argument\n");
                return NULL;
            }
            
            unsigned int argResults = 0;
            argResults = argResults | arg2;
            argResults = argResults << 4;
            argResults = argResults | arg1;
            argResults = argResults << 4;

            //only 2 bytes needed to store registers
            memcpy(buffer,&argResults , 2);
            return cursor;
        }

        case R_R_R:
        {        
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start; 

            REGS arg1 = mapsReg(start,parsedLength);
            if(arg1 == NA_REG)
            {
                fprintf(stderr, "Expected register as first argument\n");
                return NULL;
            }
            cursor = skipWhite(cursor);
            if(*cursor == '\n' || *cursor == '\0')
            {
                fprintf(stderr, "Expected register as second argument\n");
                return NULL;
            }

            start = cursor;
            cursor = argAdvanceSkip(cursor);
            parsedLength = cursor - start;

            REGS arg2 = mapsReg(start,parsedLength);
            if(arg2 == NA_REG)
            {
                fprintf(stderr, "Expected register as second argument\n");
                return NULL;
            }
            cursor = skipWhite(cursor);
            if(*cursor == '\n' || *cursor == '0')
            {
                fprintf(stderr, "Expected number as third argument\n");
                return NULL;
            }
            start = cursor;
            cursor = argAdvanceSkip(cursor); 
            parsedLength = cursor - start;
            REGS arg3 = mapsReg(start,parsedLength);
            if(arg3 == NA_REG)
            {
                fprintf(stderr, "Expected register as third argument\n");
                return NULL;
            }
            
            unsigned int argResults = 0;
            argResults = argResults | arg3;
            argResults = argResults << 8;
            argResults = argResults | arg2;
            argResults = argResults << 8;
            argResults = argResults | arg1;

            //only 3 bytes needed to store registers
            memcpy(buffer,&argResults ,3);
            return cursor;
        }

        case R_R_N:
        {
            cursor = skipWhite(cursor);
            char *start = cursor;
            cursor = argAdvanceSkip(cursor);
            size_t parsedLength = cursor - start;

            REGS arg1 = mapsReg(start,parsedLength);
            if(arg1 == NA_REG)
            {
                fprintf(stderr, "Expected register as first argument\n");
                return NULL;
            }
            cursor = skipWhite(cursor);
            if(*cursor == '\n' || *cursor == '\0')
            {
                fprintf(stderr, "Expected register as second argument\n");
                return NULL;
            }

            start = cursor;
            cursor = argAdvanceSkip(cursor);
            
            parsedLength = cursor - start;
            REGS arg2 = mapsReg(start,parsedLength);
            if(arg2 == NA_REG)
            {
                fprintf(stderr, "Expected register as second argument\n");
                return NULL;
            }
            cursor = skipWhite(cursor);
            if(*cursor == '\n' || *cursor == '0')
            {
                fprintf(stderr, "Expected register as third argument\n");
                return NULL;
            }
            start = cursor;
            cursor = argAdvanceSkip(cursor);
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
                    intAdded = intAdded + *thirdArg - 48;
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
            argResults = argResults | arg2;
            argResults = argResults << 8;
            argResults = argResults | arg1;

            //2 bytes register 4 for number
            memcpy(buffer,&argResults ,2);
            memcpy(buffer + 2, &intAdded, 4);

            return cursor;
        }

        default:
            break;
    }
    
    

    



    return NULL;
}