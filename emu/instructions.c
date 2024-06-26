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
    (gen_instruct) INS_or,
    (gen_instruct) INS_and,
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
    (gen_instruct) INS_ret,
    (gen_instruct) INS_syscall,
    (gen_instruct) INS_nop
};

//Updates register in place


void INS_addInteger(REGS desReg,REGS arg0, uint32_t integer )
{
    glob_reg[desReg] = glob_reg[arg0] + (int32_t)integer;
}

void INS_add(REGS desReg, REGS arg0, REGS arg1)
{
    glob_reg[desReg] = glob_reg[arg0] + glob_reg[arg1];
}

//Stores upper 32 bits in hi and lower in lo
void INS_multInteger(REGS arg0, uint32_t integer)
{
    int res = glob_reg[arg0] * (int32_t)integer;
    char *resAdd = (char *)&res;
    memcpy(&glob_reg[LO],resAdd,2);
    memcpy(&glob_reg[HI],resAdd + 2,2);
}

void INS_mult(REGS arg0, REGS arg1)
{
    int res = glob_reg[arg0] * glob_reg[arg1];
    char *resAdd = (char *)&res;
    memcpy(&glob_reg[LO],resAdd,2);
    memcpy(&glob_reg[HI],resAdd + 2,2);
}

void INS_or(REGS desReg, REGS arg0, REGS arg1)
{
    glob_reg[desReg] = glob_reg[arg0] || glob_reg[arg1];
}

void INS_and(REGS desReg, REGS arg0, REGS arg1)
{
    glob_reg[desReg] = glob_reg[arg0] && glob_reg[arg1];
}

void INS_xor(REGS desReg, REGS arg0, REGS arg1)
{
    glob_reg[desReg] = glob_reg[arg0] ^ glob_reg[arg1];
}

void INS_not(REGS desReg, REGS arg0)
{
    glob_reg[desReg] = ~ glob_reg[arg0];
}

void INS_shiftLeft(REGS desReg, REGS arg0, uint32_t shift)
{
    glob_reg[desReg] = glob_reg[arg0] << shift;
}

void INS_shiftRight(REGS desReg, REGS arg0, uint32_t shift)
{
    glob_reg[desReg] = glob_reg[arg0] >> shift;
}

void INS_greaterThan(REGS desReg, REGS arg0, REGS arg1)
{
    if(glob_reg[arg0] > glob_reg[arg1])
    {
        glob_reg[desReg] = 1;
        return;
    }
    glob_reg[desReg] = 0;
}

void INS_lessThan(REGS desReg, REGS arg0, REGS arg1)
{
    if(glob_reg[arg0] < glob_reg[arg1])
    {
        glob_reg[desReg] = 1;
        return;
    }
    glob_reg[desReg] = 0;
}

void INS_equal(REGS desReg, REGS arg0, REGS arg1)
{
    if(glob_reg[arg0] == glob_reg[arg1])
    {
        glob_reg[desReg] = 1;
        return;
    }
    glob_reg[desReg] = 0;
}

void INS_loadByte(REGS desReg, REGS arg0)
{
    uint32_t address = glob_reg[arg0];
    if(address > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", address);
        exit(EXIT_FAILURE);
    }
    glob_reg[desReg] = (uint32_t)PROCESS_STACK[address];
}

void INS_storeByte(REGS desReg, REGS arg0)
{
    uint32_t val =  glob_reg[arg0];
    uint32_t address = glob_reg[desReg];

    if(address > MAX_STACK_SIZE)
    {
        fprintf(stderr,"Address out of bound: %d\n", address);
        exit(EXIT_FAILURE);
    }
    PROCESS_STACK[address] = (char)(val & 0x0000000ff);
}


void INS_loadWord(REGS desReg, REGS arg0)
{
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
    glob_reg[desReg] = glob_reg[arg0];
}

void INS_jump(uint32_t index)
{
    glob_reg[IP] = PROCESS_STACK[index];
}

void INS_ret()
{
    glob_reg[IP] = glob_reg[RT];
}

//implement after detrmining syscalls
void INS_syscall(uint32_t number)
{
    switch (number)
    {
        case 0:
        {
            char *addStart = &PROCESS_STACK[glob_reg[ARG0]];
            printf("%s",addStart);
        }
            break;
    
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
            return start;
        }

        case R_R:
        {
            REGS desReg = *start|0;
            start++;
            REGS arg0 = *start|0;
            start++;
            ((two_reg)func_table[opCode]) (desReg, arg0);
            return start;  
        }

        case D_R:
        {
            REGS desReg = *start|0;
            start++;
            uint32_t dataOffset = *(uint32_t *)start;
            ((reg_num)func_table[opCode]) (dataOffset,desReg);
            start = start + sizeof(uint32_t);
            return start;
        }

        case R: //unused for now
        case L:
        case N:
        {
            
            uint32_t offset = *(uint32_t *)start;
            ((number)func_table[opCode])(offset);
            start = start + sizeof(uint32_t);
            return start;
        }

        case NO_ARG:
        {
            ((no_args)func_table[opCode])();
            return start;
        }
        
        default:
            break;
    }


}