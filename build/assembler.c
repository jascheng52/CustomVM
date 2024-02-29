#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <assembler.h>
#include <allocator.h>
#include <opcode.h>
#include <registers.h>
#include <list.h>
#include <util.h>
#include <argumentParse.h>

NODE *headData;
NODE *headLabel;
NODE *headInstr;

//if LABELFLAG true, next instruction gets linked with prev labels
int LABELFLAG = 0; 

int main(int argc, char *argv[])
{
    
    if(argc == 1)
    {
        fprintf(stderr, "Pass the .jasm as input");
        return EXIT_FAILURE;
    }
    char *currentFile;
    int res;
    init();
    for(int i = 1; i < argc; i++)
    {
        
        res = assemble(argv[i]);
        if(!res)
        {
            fprintf(stderr, "Failed to assemble %s\n", argv[i]);
        }
    }

    return -1;
}

//Returns 1 on success, else return -1
int assemble(char *file)
{
    if(!checkExtension(file))
    {
        fprintf(stderr, "Invalid file %s\n", file);
        return -1;
    }
        
    FILE *openFile = fopen(file, "r");
    if(openFile == NULL)
    {
        fprintf(stderr, "Could not find file %s\n", file);
        return -1;
    }

    //Calculating File Size
    struct stat buf;
    int fd = fileno(openFile);
    if(fd < 0)
    {
        fprintf(stderr, "Error finding fd\n");
        return -1;
    }
    if(fstat(fd, &buf) < 0)
    {
        fprintf(stderr, "Failed to get file info\n");
        return -1;
    }
    size_t fileSize = buf.st_size;
    char *buffer = malloc(fileSize);
    if(buffer == NULL)
    {
        fprintf(stderr, "Failed to get space for file\n");
        return -1;
    }

    if(parseFile(openFile,buffer) < 0)
    {
        fprintf(stderr, "Error parsing: %s\n", file);
        free(buffer);
        return -1;
    }

    if(createFile(file, buffer, fileSize) < 0)
    {
        fprintf(stderr, "Failed to create new file\n");
        free(buffer);
        return -1;
    }
    free(buffer);

    return 1;  
}



//Returns 1 on sucess
int parseFile(FILE *openFile,char *buffer)
{
    char lineBuffer[MAX_LINE_LENGTH];
    memset(lineBuffer,0,MAX_LINE_LENGTH);
    char *currLine = fgets(lineBuffer,MAX_LINE_LENGTH,openFile);
    int lineNumber = 1;
    int instrNumb = 0;

    while (currLine != NULL)
    {
        if(parseLine(lineBuffer, currLine,&instrNumb) == -1)
        {
            fprintf(stderr,"Error on line: %d\n", lineNumber);
            return -1;
        }
        lineNumber++;
        currLine = fgets(lineBuffer,MAX_LINE_LENGTH,openFile);
    }   
    return 1;
}

//Extracts static data and jmp labels and instructions 1 on success
int parseLine(char *lineBuffer, char *currLine, int *instrNum)
{

    char *cursor = skipWhite(currLine); 
    while(*cursor != '\n')
    {
        switch (*cursor)
        {
            case '.':
            {
                //case for .varName, where use can define static data 
                //Getting data variable name
                char *dataStart = cursor;
                cursor++;
                if(*cursor == '\n' || *cursor == ' ' || *cursor == '\t')
                {
                    fprintf(stderr, ". indicated start of static variable no data name after declaration\n");
                    return -1;
                }
                while(*cursor != '\n')
                {
                    if(*cursor == ' ' || *cursor == '\t')
                        break;
                    cursor++;
                }

                int dataNameLength = cursor - dataStart;
                char dataName[dataNameLength + 1];
                dataName[dataNameLength] = '\0';
                memcpy(dataName,dataStart,dataNameLength);
                if(ALLO_checkDataRep(headData,dataName,dataNameLength))
                {
                    fprintf(stderr,"Repeated data varaible name %s\n", dataName);
                    return -1;
                }

                if(*cursor == '\n')
                {
                    fprintf(stderr, "No data assigned to data variable declared\n");
                    return -1;
                }
                cursor = skipWhite(cursor);
                //Getting data values
                char *dataEnd = NULL;
                char *dataValStart = cursor;
                //Signals to parse as string or int
                int NOQUOTESFLAG = 1;
                if(*cursor == '"')
                {
                    NOQUOTESFLAG = 0;
                    cursor++;
                    dataValStart = cursor;
                    int keepQuoteLoop = 1;
                    cursor++;
                    while(keepQuoteLoop)
                    {
                        if(*cursor == '\n')
                        {
                            fprintf(stderr, "Left open quotes \n");
                            return -1;
                        }
                        if(*cursor == '"')
                        {
                            char *temp = cursor + 1;
                            temp = skipWhite(temp);
                            if(*temp == '\n')
                                break;
                            cursor = temp - 1;
                        }
                        cursor++;
                    }
                    
                    dataEnd = cursor;
                }
                else
                {
                    while(*cursor != '\n')
                    {
                        if(*cursor == ' ' || *cursor == '\t')
                            break;
                        cursor++;
                    }
                        
                    dataEnd = cursor;
                }
                int dataValLength = dataEnd - dataValStart;
                DATA_STRUCT *newData = ALLO_mallocData(dataNameLength,dataValLength);
                if(newData == NULL)
                {
                    fprintf(stderr,"Failed to malloc new data\n");
                    exit(EXIT_FAILURE);
                }
                memcpy(newData->data, dataStart, dataNameLength);
                memcpy(newData->data + dataNameLength, dataValStart, dataValLength);
                //parses data as int for no quotes
                if(NOQUOTESFLAG)
                {
                    char *red = newData->data + newData->dataNameSize;
                    for(int i = 0; i< dataValLength; i++)
                    {
                        if(*red == '-')
                        {
                            red++;
                            continue;
                        }
                        *red = *red - 48;
                        red++;
                    }
                    newData->isInt = 1;
                }
                NODE *newNode = malloc(sizeof(NODE));
                if(newNode == NULL)
                {
                    fprintf(stderr,"Failed to malloc new list node\n");
                    exit(EXIT_FAILURE);
                }
                newNode->data = newData;
                newNode->type = DATA;
                if(LIST_add_node(headData,newNode) == NULL)
                {
                    fprintf(stderr, "Failed to add data to list\n");
                    exit(EXIT_FAILURE);
                }        
                // debug(newData->data,dataNameLength+dataValLength);
                if(!NOQUOTESFLAG)
                    cursor++;
                // if(*cursor != '\n')
                // {
                //     fprintf(stderr, "Detected more characters after data declarion. One declaration per line\n");
                //     return -1;
                // }
                break;
            }
            case '#':
            {
                //Case for label name
                char *labelStart = cursor;
                cursor++;
                if(*cursor == '\n' || *cursor == ' ' || *cursor == '\t')
                {
                    fprintf(stderr, "# indicated start of label no label name after declaration\n");
                    return -1;
                }
                while(*cursor != '\n')
                {
                    if(*cursor == ' ' || *cursor == '\t')
                        break;
                    cursor++;
                }
                int labelNameLength = cursor - labelStart;
                char label[labelNameLength + 1];
                label[labelNameLength] = '\0';
                memcpy(label,labelStart,labelNameLength); 
                if(ALLO_checkLabelRep(headLabel,label,labelNameLength))
                {
                    fprintf(stderr,"Repeated Label name %s\n", label);
                    return -1;
                }

                LABEL_STRUCT *newLabel = ALLO_mallocLabel(labelNameLength);
                if(newLabel == NULL)
                {
                    fprintf(stderr,"Failed to malloc new label");
                    return -1;
                }
                memcpy(newLabel->label,label, labelNameLength);
                NODE *newNode = malloc(sizeof(NODE));
                if(LIST_add_node(headLabel,newNode) == NULL)
                {
                    fprintf(stderr,"Label list is corrupted");
                    return -1;
                }
                newNode->type = LABEL;
                newNode->data = newLabel;
                LABELFLAG = 1;
                debug(newLabel->label,labelNameLength);
                break;
            }
            default:
            {
                INSTR_STRUCT *parsedIns = NULL;
                char *cursor = getInstruct(cursor, &parsedIns);
                break;
            }
        }
        cursor = skipWhite(cursor);


    }

    return 1;
}


//Parses cursor to get instruction. Returns in parsedIns INSTR_STRUCT pointer. Retuns Null on error
char *getInstruct(char *cursor, INSTR_STRUCT **parsedIns)
{
    char *start = cursor;
    while(*cursor != '\n')
    {
        if(*cursor == ' ' || *cursor == '\t')
            break;
    }
    if(*cursor == '\n')
    {
        fprintf(stderr, "Could not parse instruction\n");
        return NULL;
    }
    
    size_t parsedLength = cursor - start;
    OPS op = NA_OP;
    for(int i = 0; i < NUM_INSTR; i++)
    {
        size_t listLength = strnlen(VALID_INS[i], UPPER_INST_LENGTH);
        if(listLength == parsedLength)
        {
            if(strncmp(VALID_INS[i], start, listLength) == 0)
            {
                //enum num should match index
                op = i;
                break;
            }
        }
    }
    switch(op)
    {
        case ADDI:
        {
            
            INSTR_STRUCT *newInstr = ALLO_mallocInstr(ADDI,sizeof(INSTR_STRUCT) + 2*WORD_LENGTH);
            if(newInstr == NULL)
            {
                fprintf(stderr, "Failed to malloc instruction");
                return NULL;
            }
            NODE *newNode = malloc(sizeof(NODE));
            if(newNode == NULL)
            {
                fprintf(stderr, "Failed to malloc instruction");
                return NULL;
            }
            newNode->type = INSTR;
            newNode->data = newInstr;
            LIST_add_node(headInstr,newNode);
            char *argStart = start + parsedLength;
            cursor = findArgs(ADDI, 3,cursor, newInstr->args,newInstr->argsSizeBits);


            
            break;
        }
            break;
        default:
            break;
    }

    return NULL;
}



//Returns 1 on Success
int createFile(char *file, char *buffer, size_t size)
{
    char newFileName[strlen(file)];
    memcpy(newFileName,file,strlen(file) - 4);
    memcpy(newFileName + strlen(file) - 4,"jac", 3);
    newFileName[strlen(file) - 1] = '\0';
    FILE *newFile = fopen(newFileName, "w");
    if(newFile == NULL)
        return -1;
    if(fwrite(buffer, 1, size, newFile) < size)
        return -1;

    fclose(newFile);
    return 1;
}


//Creates Lists/other init data
void init()
{
    if((headData = malloc(sizeof(NODE))) == NULL)
    {
        fprintf(stderr, "Failed to malloc space\n");
        exit(EXIT_FAILURE);
    }
    headData->next = headData;
    headData->prev = headData;
    headData->type = NONE;

    if((headLabel = malloc(sizeof(NODE))) == NULL)
    {
        fprintf(stderr, "Failed to malloc space\n");
        exit(EXIT_FAILURE);
    }
    headLabel->next = headLabel;
    headLabel->prev = headLabel;
    headLabel->type = NONE;

    if((headInstr = malloc(sizeof(NODE))) == NULL)
    {
        fprintf(stderr, "Failed to malloc space\n");
        exit(EXIT_FAILURE);
    }
    headInstr->next = headInstr;
    headInstr->prev = headInstr;
    headInstr->type = NONE;

}

