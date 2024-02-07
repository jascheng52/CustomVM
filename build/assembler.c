#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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



    //Calculating File Size
    struct stat buf;
    int fd = fileno(openFile);
    if(fd < 0)
    {
        fprintf(stderr, "Error finding fd\n");
        return 0;
    }
    if(fstat(fd, &buf) < 0)
    {
        fprintf(stderr, "Failed to get file info\n");
        return 0;
    }
    size_t fileSize = buf.st_size;
    char buffer[fileSize];
    if(createFile(file, buffer, fileSize) == 0)
    {
        fprintf(stderr, "Failed to create new file\n");
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

//Returns 1 on Success
int createFile(char *file, char *buffer, size_t size)
{
    char newFileName[strlen(file)];
    memcpy(newFileName,file,strlen(file) - 4);
    memcpy(newFileName + strlen(file) - 4,"jac", 3);
    newFileName[strlen(file) - 1] = '\0';
    FILE *newFile = fopen(newFileName, "w");
    if(newFile == NULL)
        return 0;
    if(fwrite(buffer, 1, size, newFile) < size)
        return 0;

    fclose(newFile);
    return 1;
}