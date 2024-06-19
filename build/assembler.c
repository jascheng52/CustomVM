#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#include <assembler.h>
#include <allocator.h>
#include <registers.h>
#include <list.h>
#include <util.h>
#include <opcode.h>
#include <argumentParse.h>

NODE *headData;
NODE *headLabel;
NODE *headInstr;

// if LABELFLAG true, next instruction gets linked with prev labels
int LABELFLAG = 0;

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        fprintf(stderr, "Pass the .jasm as input\n");
        return EXIT_FAILURE;
    }
    char *currentFile;
    int res;
    init();
    for (int i = 1; i < argc; i++)
    {

        res = assemble(argv[i]);
        if (!res)
        {
            fprintf(stderr, "Failed to assemble %s\n", argv[i]);
        }
    }

    return -1;
}

// Returns 1 on success, else return -1
int assemble(char *file)
{
    if (!checkExtension(file))
    {
        fprintf(stderr, "Invalid file %s\n", file);
        return -1;
    }

    FILE *openFile = fopen(file, "r");
    if (openFile == NULL)
    {
        fprintf(stderr, "Could not find file %s\n", file);
        return -1;
    }

    // Calculating File Size
    struct stat buf;
    int fd = fileno(openFile);
    if (fd < 0)
    {
        fprintf(stderr, "Error finding fd\n");
        return -1;
    }
    if (fstat(fd, &buf) < 0)
    {
        fprintf(stderr, "Failed to get file info\n");
        return -1;
    }
    size_t fileSize = buf.st_size;
    char *buffer = malloc(fileSize);
    if (buffer == NULL)
    {
        fprintf(stderr, "Failed to get space for file\n");
        return -1;
    }

    if (parseFile(openFile, buffer) < 0)
    {
        fprintf(stderr, "Error parsing: %s\n", file);
        free(buffer);
        return -1;
    }

    if (createFile(file, buffer, fileSize) < 0)
    {
        fprintf(stderr, "Failed to create new file\n");
        free(buffer);
        return -1;
    }
    free(buffer);

    return 1;
}

// Returns 1 on sucess
int parseFile(FILE *openFile, char *buffer)
{
    char lineBuffer[MAX_LINE_LENGTH];
    memset(lineBuffer, 0, MAX_LINE_LENGTH);
    char *currLine = fgets(lineBuffer, MAX_LINE_LENGTH, openFile);
    int lineNumber = 1;
    int instrNumb = 0;

    while (currLine != NULL)
    {
        if (parseLine(lineBuffer, currLine, &instrNumb) == -1)
        {
            fprintf(stderr, "Error on line: %d\n", lineNumber);
            return -1;
        }
        lineNumber++;
        currLine = fgets(lineBuffer, MAX_LINE_LENGTH, openFile);
    }
    return 1;
}

// Extracts static data and jmp labels and instructions 1 on success
int parseLine(char *lineBuffer, char *currLine, int *instrNum)
{

    char *cursor = skipWhite(currLine);
    // fprintf(stdout, "Cursor position , %p\n", cursor);
    while (*cursor != '\n')
    {
        if (*cursor == '\0')
            return 1;
        switch (*cursor)
        {
        case '.':
        {
            // case for .varName, where use can define static data
            // Getting data variable name
            char *dataStart = cursor;
            cursor++;
            if (*cursor == '\n' || *cursor == ' ' || *cursor == '\t' || *cursor == '\0')
            {
                fprintf(stderr, ". indicated start of static variable no data name after declaration\n");
                return -1;
            }
            while (*cursor != '\n')
            {
                if (*cursor == ' ' || *cursor == '\t' || *cursor == '\0')
                    break;
                cursor++;
            }

            int dataNameLength = cursor - dataStart;
            char dataName[dataNameLength + 1];
            dataName[dataNameLength] = '\0';
            memcpy(dataName, dataStart, dataNameLength);
            if (ALLO_checkDataRep(headData, dataName, dataNameLength))
            {
                fprintf(stderr, "Repeated data varaible name %s\n", dataName);
                return -1;
            }

            if (*cursor == '\n' || *cursor == '\0')
            {
                fprintf(stderr, "No data assigned to data variable declared\n");
                return -1;
            }
            cursor = skipWhite(cursor);
            // Getting data values
            char *dataEnd = NULL;
            char *dataValStart = cursor;
            size_t dataValLength = 0;
            int32_t total = 0;

            // Signals to parse as string or int
            int NOQUOTESFLAG = 1;
            if (*cursor == '"')
            {
                NOQUOTESFLAG = 0;
                cursor++;
                dataValStart = cursor;
                while (1)
                {
                    if (*cursor == '\n' || *cursor == '\0')
                    {
                        fprintf(stderr, "Left open quotes \n");
                        return -1;
                    }
                    if (*cursor == '"')
                    {
                        char *temp = cursor + 1;
                        temp = skipWhite(temp);
                        if (*temp == '\n' || *cursor == '\0')
                            break;
                        cursor = temp - 1;
                    }
                    cursor++;
                }

                dataEnd = cursor;
                dataValLength = dataEnd - dataValStart;
            }
            else
            {
                int neg = 1;
                if(*cursor == '-')
                {
                    neg = -1;
                    cursor++;
                }
                while (*cursor != '\n')
                {
                    if (*cursor == ' ' || *cursor == '\t' || *cursor == '\0')
                        break;
                    total = total * 10;
                    total = total + *cursor - 48;
                    cursor++;
                }
                dataEnd = cursor;
                size_t min = 0;
                if(neg == -1)
                    min = 1;
                if(dataEnd - dataValStart == min)
                {
                    fprintf(stderr,"Missing data value declaration\n");
                    return -1;
                }
                dataValLength = 4;
                dataValStart =  (void *)&total;
            }
            
            DATA_STRUCT *newData = ALLO_mallocData(dataNameLength, dataValLength);
            if (newData == NULL)
            {
                fprintf(stderr, "Failed to malloc new data\n");
                exit(EXIT_FAILURE);
            }
            
            if (NOQUOTESFLAG)
                newData->isInt = 1;
        
            memcpy(newData->data, dataStart, dataNameLength);
            memcpy(newData->data + dataNameLength, dataValStart, dataValLength);

            NODE *newNode = malloc(sizeof(NODE));
            if (newNode == NULL)
            {
                fprintf(stderr, "Failed to malloc new list node\n");
                exit(EXIT_FAILURE);
            }
            newNode->data = newData;
            newNode->type = DATA;
            if (LIST_add_node(headData, newNode) == NULL)
            {
                fprintf(stderr, "Failed to add data to list\n");
                exit(EXIT_FAILURE);
            }
            // debug(newData->data,dataNameLength+dataValLength);
            if (!NOQUOTESFLAG)
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
            // Case for label name
            char *labelStart = cursor;
            cursor++;
            if (*cursor == '\n' || *cursor == ' ' || *cursor == '\t' || *cursor == '\0')
            {
                fprintf(stderr, "# indicated start of label no label name after declaration\n");
                return -1;
            }
            while (*cursor != '\n')
            {
                if (*cursor == ' ' || *cursor == '\t' || *cursor == '\0')
                    break;
                cursor++;
            }
            int labelNameLength = cursor - labelStart;
            char label[labelNameLength + 1];
            label[labelNameLength] = '\0';
            memcpy(label, labelStart, labelNameLength);
            if (ALLO_checkLabelRep(headLabel, label, labelNameLength))
            {
                fprintf(stderr, "Repeated Label name %s\n", label);
                return -1;
            }

            LABEL_STRUCT *newLabel = ALLO_mallocLabel(labelNameLength);
            // fprintf(stderr,"New Label at %p\n", newLabel);
            if (newLabel == NULL)
            {
                fprintf(stderr, "Failed to malloc new label");
                return -1;
            }
            memcpy(newLabel->label, label, labelNameLength);
            NODE *newNode = malloc(sizeof(NODE));
            if (LIST_add_node(headLabel, newNode) == NULL)
            {
                fprintf(stderr, "Label list is corrupted");
                return -1;
            }
            newNode->type = LABEL;
            newNode->data = newLabel;
            LABELFLAG = 1;
            //debug(newLabel->label, labelNameLength);
            break;
        }
        default:
        {
            // fprintf(stdout, "Cursor position , %p\n", cursor);
            INSTR_STRUCT *parsedIns = NULL;
            cursor = getInstruct(cursor, &parsedIns);
            if (cursor == NULL)
                return -1;

            //Binding label to instruction
            if(LABELFLAG)
            {
                LABEL_STRUCT *lastLabel = headLabel->prev->data;
                lastLabel->instRef = parsedIns;
                LABELFLAG = 0;
            }

            break;
        }
        }
        cursor = skipWhite(cursor);
    }

    return 1;
}

// Parses cursor to get instruction. Returns in parsedIns INSTR_STRUCT pointer. Retuns Null on error
char *getInstruct(char *cursor, INSTR_STRUCT **parsedIns)
{
    char *start = cursor;
    while (*cursor != '\n')
    {
        if (*cursor == ' ' || *cursor == '\t' || *cursor == '\0')
            break;
        cursor++;
    }
    // if (*cursor == '\n' || *cursor == '\0')
    // {
    //     fprintf(stderr, "Could not parse instruction\n");
    //     return NULL;
    // }

    size_t parsedLength = cursor - start;
    OPS op = NA_OP;
    for (int i = 0; i < NUM_INSTR; i++)
    {
        size_t listLength = strnlen(VALID_INS[i], UPPER_INST_LENGTH);
        if (listLength == parsedLength)
        {
            if (strncmp(VALID_INS[i], start, listLength) == 0)
            {
                // enum num should match index
                op = i;
                break;
            }
        }
    }
    if(op == NA_OP)
    {
        char tokenString[parsedLength + 1];
        memcpy(tokenString,start,parsedLength);
        tokenString[parsedLength] = '\0';
        fprintf(stderr, "Unrecognized instruction %s\n", start);
        return NULL;
    }
    
    char *argStart = start + parsedLength;
    size_t argSizeBytes = ARG_OP_SIZE[op];

    INSTR_STRUCT *newInstr = ALLO_mallocInstr(op, argSizeBytes);
    if (newInstr == NULL)
    {
        fprintf(stderr, "Failed to malloc instruction");
        return NULL;
    }

    NODE *newNode = malloc(sizeof(*newNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Failed to malloc instruction");
        return NULL;
    }
    newNode->type = INSTR;
    newNode->data = newInstr;
    LIST_add_node(headInstr, newNode);

    cursor = findArgs(op, cursor, newInstr->args, headLabel,headData);

    printBytesFromBuffer(newInstr->args,argSizeBytes);
    // int z = *(int *)(newInstr->args + 2);
    // printf("%d\n",z);
    if (cursor == NULL)
        return NULL;

    *parsedIns = newInstr;

    return cursor;
}

// Returns 1 on Success
int createFile(char *file, char *buffer, size_t size)
{
    char newFileName[strlen(file)];
    memcpy(newFileName, file, strlen(file) - 4);
    memcpy(newFileName + strlen(file) - 4, "jac", 3);
    newFileName[strlen(file) - 1] = '\0';
    
    FILE *newFile = fopen(newFileName, "wb");
    if (newFile == NULL)
        return -1;
    size_t instructionStart = 0;
    fwrite(&instructionStart,1,sizeof(size_t), newFile);

    size_t offset = sizeof(instructionStart);
    offset = offset + writeData(offset,newFile);
    instructionStart = offset;
    fprintf(stderr,"Written Bytes Data: %d\n", offset);

    //Updating instruction start
    fseek(newFile,0,SEEK_SET);
    fwrite(&instructionStart,1,sizeof(size_t), newFile);
    fseek(newFile,instructionStart,SEEK_SET);

    size_t finalSize = writeIns(offset,newFile);
    fprintf(stderr,"Written Bytes Instructions: %d\n", finalSize-offset);


    fclose(newFile);
    return 1;
}

// Creates Lists/other init data
void init()
{
    if ((headData = malloc(sizeof(NODE))) == NULL)
    {
        fprintf(stderr, "Failed to malloc space\n");
        exit(EXIT_FAILURE);
    }
    headData->next = headData;
    headData->prev = headData;
    headData->type = NONE;
    
    if ((headLabel = malloc(sizeof(NODE))) == NULL)
    {
        fprintf(stderr, "Failed to malloc space\n");
        exit(EXIT_FAILURE);
    }
    headLabel->next = headLabel;
    headLabel->prev = headLabel;
    headLabel->type = NONE;

    if ((headInstr = malloc(sizeof(NODE))) == NULL)
    {
        fprintf(stderr, "Failed to malloc space\n");
        exit(EXIT_FAILURE);
    }
    headInstr->next = headInstr;
    headInstr->prev = headInstr;
    headInstr->type = NONE;
}

// Return number of bytes written
size_t writeData(size_t offset, FILE *file)
{

    size_t newOffset = offset;
    NODE *cursor = headData->next;

    while(cursor != headData)
    {
        DATA_STRUCT *res = (DATA_STRUCT *)cursor->data;
        res->index = newOffset;
        char *dataValStart = res->data + res->dataNameSize;
        size_t numWrite = 0;
        char dataWrite[res->dataSize + 1];
        memcpy(dataWrite, dataValStart,res->dataSize);
        dataWrite[res->dataSize] = '\0';
        size_t toCopy = res->dataSize + 1;
        if(res->isInt)
        {
            toCopy--;
        }
    
        numWrite = fwrite(dataWrite,1,toCopy,file);
        if(numWrite == 0)
        {
            fprintf(stderr, "Failed to write to file\n");
            exit(EXIT_FAILURE);
        }
        newOffset = newOffset + numWrite;
        cursor = cursor->next;
    }

    return newOffset;
}
size_t writeIns(size_t offset, FILE *file)
{
    size_t newOffset = offset;

    NODE *cursor = headInstr->next;

    while(cursor != headInstr)
    {
        INSTR_STRUCT *instruct = (INSTR_STRUCT *) cursor->data;
        instruct->index = offset;

        OPS op = instruct->opType;

        newOffset = newOffset + fwrite(&op,1,2,file); 
        REG_ARG_TYPE argType = REG_TYPE_MAP[op];
        char *argStart = instruct->args;

        switch(argType)
        {
            //Limit 32 bits for fiel references.
            case L:
            {
                LABEL_STRUCT *label = *((LABEL_STRUCT **)argStart);
                INSTR_STRUCT *ref = label->instRef;
                newOffset = newOffset + fwrite(&ref->index,1,sizeof(u_int32_t),file);
                break;
            }
            case D_R:
            {
                DATA_STRUCT *data = *((DATA_STRUCT **) (argStart + 2));
                newOffset = newOffset + fwrite(argStart, 1, 2, file);
                newOffset = newOffset + fwrite(&data->index, 1, sizeof(u_int32_t), file);
                break;
            }

            default:
            {
                newOffset = newOffset + fwrite(argStart,1,instruct->argSizeBytes,file);
                break;
            }
        }

        cursor = cursor->next;
    }
    
    return newOffset;
}