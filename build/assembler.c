#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assembler.h>


int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        fprintf(stderr, "Pass the .jasm as input");
        return EXIT_FAILURE;
    }

    char *currentFile;
    int res;
    for(int i = 1; i < argc; i++)
    {
        res = assemble(argv[i]);
        if(res == 0)
        {
            fprintf(stderr, "Failed to assemble %s\n", argv[i]);
        }
    }

    return 0;
}

//Returns 1 on success, else return 0
int assemble(char *file)
{
    if(checkExtension(file) == 0)
    {
        fprintf(stderr, "Invalid file %s\n", file);
        return 0;
    }
        
    FILE *openFile = fopen(file, "r");
    if(openFile == NULL)
    {
        fprintf(stderr, "Could not find file %s\n", file);
        return 0;
    }
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
        return 0;
    if(length-index != 5)
        return 0;
    if(!strncmp(&file[index],".jasm", EXTEN_CMP))
        return 1;

    
    return 0;
}