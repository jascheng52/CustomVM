#include <stdlib.h>
#include <stdio.h>
#include <instructions.h>
#include <stdint.h>
#include <emu.h>
#include <string.h>
typedef void (*gen_instruct) (void);

gen_instruct func_table[] = {
    (gen_instruct) INS_addInteger,
    (gen_instruct) INS_add,
    (gen_instruct) INS_multInteger,
    (gen_instruct) INS_mult,
    (gen_instruct) INS_and,
    (gen_instruct) INS_or,
    (gen_instruct) INS_xor,
    (gen_instruct) INS_not,
    (gen_instruct) INS_shiftLeft,
    (gen_instruct) INS_shiftRight,
    (gen_instruct) INS_greaterThan,
    (gen_instruct) INS_lessThan,
    (gen_instruct) INS_equal,
    (gen_instruct) INS_loadByte,
    (gen_instruct) INS_storeByte,
    (gen_instruct) INS_loadWord,
    (gen_instruct) INS_loadAddress,
    (gen_instruct) INS_storeWord,
    (gen_instruct) INS_move,
    (gen_instruct) INS_jump,
    (gen_instruct) INS_syscall,
    (gen_instruct) INS_nop
};

//Updates register in place


void INS_addInteger(REGS desReg,REGS arg0, uint32_t integer )
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0] + (int32_t)integer;
}

void INS_add(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0] + glob_reg[arg1];
}

//Stores upper 32 bits in hi and lower in lo
void INS_multInteger(REGS arg0, uint32_t integer)
{
    unsigned long res = (unsigned long)glob_reg[arg0] * (unsigned long)integer;
    char *resAdd = (char *)&res;
    memcpy(&glob_reg[LO],resAdd,4);
    memcpy(&glob_reg[HI],resAdd + 4,4);
}

void INS_mult(REGS arg0, REGS arg1)
{
    long res = glob_reg[arg0] * glob_reg[arg1];
    char *resAdd = (char *)&res;
    memcpy(&glob_reg[LO],resAdd,4);
    memcpy(&glob_reg[HI],resAdd + 4,4);
}

void INS_or(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0] | glob_reg[arg1];
}

void INS_and(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0] & glob_reg[arg1];
}

void INS_xor(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0] ^ glob_reg[arg1];
}

void INS_not(REGS desReg, REGS arg0)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = ~ glob_reg[arg0];
}

void INS_shiftLeft(REGS desReg, REGS arg0, uint32_t shift)
{
    glob_reg[desReg] = glob_reg[arg0] << shift;
}

void INS_shiftRight(REGS desReg, REGS arg0, uint32_t shift)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0] >> shift;
}

void INS_greaterThan(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    if(glob_reg[arg0] > glob_reg[arg1])
    {
        glob_reg[desReg] = 1;
        return;
    }
    glob_reg[desReg] = 0;
}

void INS_lessThan(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    if(glob_reg[arg0] < glob_reg[arg1])
    {
        glob_reg[desReg] = 1;
        return;
    }
    glob_reg[desReg] = 0;
}

void INS_equal(REGS desReg, REGS arg0, REGS arg1)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    if(glob_reg[arg0] == glob_reg[arg1])
    {
        glob_reg[desReg] = 1;
        return;
    }
    glob_reg[desReg] = 0;
}

void INS_loadByte(REGS desReg, REGS arg0)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    uint32_t address = glob_reg[arg0];
    if(address > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", address);
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = (uint32_t)PROCESS_STACK[address] & 0x000f;
}

void INS_storeByte(REGS desReg, REGS arg0)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    uint32_t val =  glob_reg[arg0];
    uint32_t address = glob_reg[desReg];

    if(address > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", address);
        exit(EXIT_FAILURE);
    }
    PROCESS_STACK[address] = (char)(val & 0x000f);
}


void INS_loadWord(REGS desReg, REGS arg0)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    uint32_t address = glob_reg[arg0];
    if(address > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", address);
        exit(EXIT_FAILURE);
    }
    //casting as number then deref
    glob_reg[desReg] = *(uint32_t *)(&PROCESS_STACK[address]);

}

//index is pointer in process stack of data location
void INS_loadAddress(uint32_t index,REGS arg0)
{
    if(index > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", index);
        exit(EXIT_FAILURE);
    }
    glob_reg[arg0] = index;
}

void INS_storeWord(REGS desReg, REGS arg0)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    uint32_t val = glob_reg[arg0];
    uint32_t address = glob_reg[desReg];

    if(address > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", address);
        exit(EXIT_FAILURE);
    }

    PROCESS_STACK[address] = val;
}

void INS_move(REGS desReg, REGS arg0)
{
    if(desReg == ZERO_NUM)
    {
        fprintf(stderr,"Tried to modify Zero Register. Cannot be modified");
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = glob_reg[arg0];
}

void INS_jump(uint32_t index)
{
    glob_reg[IP] = index;
}


//implement after detrmining syscalls
void INS_syscall(uint32_t number)
{
    switch (number)
    {
        case 0: //print address unsigned
        {
            char *addStart = &PROCESS_STACK[glob_reg[ARG0]];
            uint32_t num = *(uint32_t *)addStart;
            printf("%u\n",num);
            break;
        }
        case 1: //print addresssigned
        {
            char *addStart = &PROCESS_STACK[glob_reg[ARG0]];
            uint32_t num = *(uint32_t *)addStart;
            printf("%d\n",num);
            break;
        }
        case 2: //print address string
        {
            char *addStart = &PROCESS_STACK[glob_reg[ARG0]];
            printf("%s\n",addStart);
            break;
        }

        case 3: //print register(a0)
        {
            uint32_t num = glob_reg[ARG0];
            printf("%d\n",num);
            break;
        }

        default:
            printf("Invalid Syscall Number\n");
            break;
    }
    return;
}

void INS_nop()
{
    return;
}


//returns next start of instruction
char *INS_executeNext(char* start)
{
   
    OPS opCode = (OPS)(*((uint32_t *)start) & 0x00ff); 
    start = start + 2;
    REG_ARG_TYPE type = REG_TYPE_MAP[opCode];
    switch (type)
    {
        case R_R_R:
        {
            REGS desReg = *start|0;
            start++;
            REGS arg0 = *start|0;
            start++;
            REGS arg1 = *start|0;
            start++;
            ((three_reg)func_table[opCode]) (desReg, arg0, arg1);
            glob_reg[IP] = start - PROCESS_STACK;
            return start;
            break;
        }

        case R_R_N:
        {
            REGS desReg = *start|0;
            start++;
            REGS arg0 = *start|0;
            start++;
            uint32_t number = *(uint32_t *) start;
            start = start + sizeof(uint32_t);
            ((two_reg_num)func_table[opCode]) (desReg, arg0, number);
            glob_reg[IP] = start - PROCESS_STACK;

            return start;
        }

        case R_R:
        {
            REGS desReg = *start|0;
            start++;
            REGS arg0 = *start|0;
            start++;
            ((two_reg)func_table[opCode]) (desReg, arg0);
            glob_reg[IP] = start - PROCESS_STACK;
            return start;  
        }

        case R_N:
        {
            REGS desReg = *start|0;
            start++;
            uint32_t number = *(uint32_t *) start;
            start = start + sizeof(uint32_t);
            ((two_reg)func_table[opCode]) (desReg, number);
            glob_reg[IP] = start - PROCESS_STACK;
            return start;  
        }

        case D_R:
        {
            REGS desReg = *start|0;
            start++;
            uint32_t dataOffset = *(uint32_t *)start;
            ((reg_num)func_table[opCode]) (dataOffset,desReg);
            start = start + sizeof(uint32_t);
            glob_reg[IP] = start - PROCESS_STACK;
            return start;
        }

        case R: //unused for now
        case N:
        {
            
            uint32_t offset = *(uint32_t *)start;
            ((number)func_table[opCode])(offset);
            start = start + sizeof(uint32_t);
            glob_reg[IP] = start - PROCESS_STACK;

            return start;
        }
        case L:
        {
            uint32_t offset = *(uint32_t *)start;
            ((number)func_table[opCode])(offset);//modifes glob_IP
            return PROCESS_STACK + glob_reg[IP];
        }

        case NO_ARG:
        {
            ((no_args)func_table[opCode])();
            glob_reg[IP] = start - PROCESS_STACK;
            return start;
        }
        
        default:
            break;
    }


}