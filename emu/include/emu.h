#include <stdint.h>

#ifndef EMULATE
#define EMULATE

#include <registers.h>

#define MAX_STACK_SIZE 65535 //max represented by 16 bits
#define MAX_FILE_LENGTH 256
#define NUM_REGISTERS 16



int checkExtension(char *file);

#endif /*emu.h*/