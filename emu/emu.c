
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <instructions.h>

#include <emu.h>


char PROCESS_STACK[MAX_STACK_SIZE];

 
uint32_t glob_reg[16];

int main(int argc, char **argv)
{

    if (argc == 1)
    {
        fprintf(stderr,"Missing program file\n");
        exit(EXIT_FAILURE);
    }
    if (!checkExtension(argv[1]))
    {
        fprintf(stderr, "Invalid File. Extension is .jac\n");
        exit(EXIT_FAILURE);
    }
    
    
    int progFile = open(argv[1],O_RDONLY);

    if (progFile < 0)
    {
        fprintf(stderr,"Failed to open file\n");
        close(progFile);
        exit(EXIT_FAILURE);
    }

    struct stat buf;
    if (fstat(progFile,&buf) < 0)
    {
        fprintf(stderr,"Failed to open file\n");
        close(progFile);
        exit(EXIT_FAILURE);
    }
    size_t progSize = buf.st_size;
    
    size_t numWritten = 0;
    while(numWritten < progSize)
    {
        int num = read(progFile, PROCESS_STACK, progSize);
        if(num < 0)
        {
            fprintf(stderr, "Error writing instructions: %d\n", errno);
            close(progFile);
            exit(EXIT_FAILURE);
        }
        numWritten = numWritten + num;
    }
    PROCESS_STACK[numWritten] = 0xff;
    PROCESS_STACK[numWritten + 1] = 0xff;
    numWritten = numWritten + 2;

    close(progFile);

    run();

    return 0;
}

void run()
{
    //Setting pointer of sp to the top of stack
    //Grows down
    glob_reg[SP] = MAX_STACK_SIZE;
    char *start = PROCESS_STACK;
    size_t offset = *(size_t *)start;
    start = start + offset;
    glob_reg[IP] = start - PROCESS_STACK;
    while(*(uint16_t *)(glob_reg[IP] + PROCESS_STACK) != 0xffff)
    {
        start = INS_executeNext(start);
    }
    return;

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
    if(!strncmp(&file[index],".jac", MAX_FILE_LENGTH))
        return 1;

    return 0;
}