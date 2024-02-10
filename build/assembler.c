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

NODE *headData;
NODE *headLabel;

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

    if(!parseFile(openFile,buffer))
    {
        fprintf(stderr, "Error parsing file\n");
        free(buffer);
        return -1;
    }

    if(!createFile(file, buffer, fileSize))
    {
        fprintf(stderr, "Failed to create new file\n");
        free(buffer);
        return -1;
    }
    free(buffer);

    return 1;
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

//Returns 1 on sucess
int parseFile(FILE *openFile,char *buffer)
{
    char lineBuffer[MAX_LINE_LENGTH];
    memset(lineBuffer,0,MAX_LINE_LENGTH);
    char *currLine = fgets(lineBuffer,MAX_LINE_LENGTH,openFile);
    int lineNumber = 0;
    int instrNumb = 0;

    while (currLine != NULL)
    {
        if(parseLine(lineBuffer, currLine,&instrNumb) == 0)
        {
            fprintf(stderr,"Error on line: %d\n", lineNumber);
            return -1;
        }
        lineNumber++;
        currLine = fgets(lineBuffer,MAX_LINE_LENGTH,openFile);
    }   
    return 1;
}

int parseLine(char *lineBuffer, char *currLine, int *instrNum)
{

    char *tokenStart = skipWhite(currLine);
    char c = *tokenStart;
    //case for .varName, where use can define static data
    if(c == '.')
    {
        //Getting data variable name
        char *cursor = tokenStart + 1;
        if(*cursor == '\n' || *cursor == ' ' || *cursor == '\t')
        {
            fprintf(stderr, ". indicated start of static variable no data name after declaration\n");
            return -1;
        }
        while(*cursor != '\t' || *cursor != '\n')
        {
            if(*cursor == ' ')
                break;
            cursor++;
        }

        int dataNameLength = cursor - tokenStart;
        if(*cursor == '\n')
        {
            fprintf(stderr, "No data assigned to data variable declared\n");
            return -1;
        }
        cursor = skipWhite(cursor);
        //Getting data values
        char *dataEnd = NULL;
        char *dataValStart = cursor;
        if(*cursor == '"')
        {
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
                cursor++;
            dataEnd = cursor;
        }
        int dataValLength = dataEnd - dataValStart ;
        
        DATA_STRUCT *newData = mallocData(dataNameLength,dataValLength);
        memcpy(newData->data, tokenStart, dataNameLength);
        memcpy(newData->data + dataNameLength, dataValStart, dataValLength);
        // debug(newData->data,dataNameLength+dataValLength);

        return 1;
    }

    return 1;
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

//Skips white space not include \n
char *skipWhite(char *textLine)
{
    char *cursor = textLine;
    while(*cursor == ' ' || *cursor == '\t' || *cursor == '\r')
    {
        cursor++;
    }
    return cursor;
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
        headLabel->next = headData;
        headLabel->prev = headData;
        headLabel->type = NONE;

}

void debug(void *buffer, size_t num)
{
        char temp[num +1];
        memcpy(temp,buffer,num);
        temp[num] = '\0';
        printf("%p:%s\n",temp,temp);
}