#ifndef INSTRUCTION


#define INSTRUCTION
#include <registers.h>
#include <stdint.h>
#include <opcode.h>

void INS_addInteger(REGS desReg,REGS arg0, uint32_t integer );
void INS_add(REGS desReg, REGS arg0, REGS arg1);
void INS_multInteger(REGS arg0, uint32_t integer);
void INS_mult(REGS arg0, REGS arg1);
void INS_or(REGS desReg, REGS arg0, REGS arg1);
void INS_and(REGS desReg, REGS arg0, REGS arg1);
void INS_xor(REGS desReg, REGS arg0, REGS arg1);
void INS_not(REGS desReg, REGS arg0);
void INS_shiftLeft(REGS desReg, REGS arg0, uint32_t shift);
void INS_shiftRight(REGS desReg, REGS arg0, uint32_t shift);
void INS_greaterThan(REGS desReg, REGS arg0, REGS arg1);
void INS_lessThan(REGS desReg, REGS arg0, REGS arg1);
void INS_equal(REGS desReg, REGS arg0, REGS arg1);
void INS_loadByte(REGS desReg, REGS arg0);
void INS_loadWord(REGS desReg, REGS arg0);
void INS_loadAddress(size_t index,REGS arg0);
void INS_storeWord(REGS desReg, REGS arg0);
void INS_move(REGS desReg, REGS arg0);
void INS_jump(size_t index);
void INS_ret();
void INS_syscall(size_t number);
void INS_nop();

typedef void (*three_reg) (REGS desReg ,REGS arg0, REGS arg1);
typedef void (*two_reg_num) (REGS desReg ,REGS arg0, uint32_t integer);
typedef void (*two_reg) (REGS arg0 ,REGS arg1);
typedef void (*reg_num) (REGS desReg, uint32_t integer);
typedef void (*num_reg) (size_t index, REGS arg0);
typedef void (*number) (size_t index);
typedef void (*no_args) ();

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
    (gen_instruct) INS_loadWord,
    (gen_instruct) INS_loadAddress,
    (gen_instruct) INS_storeWord,
    (gen_instruct) INS_move,
    (gen_instruct) INS_jump,
    (gen_instruct) INS_ret,
    (gen_instruct) INS_syscall,
    (gen_instruct) INS_nop
};






#endif /*instruction.h*/