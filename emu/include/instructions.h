#ifndef INSTRUCTION

#define INSTRUCTION
#include <registers.h>
#include <stdint.h>
#include <opcode.h>

char *INS_executeNext(char* start);
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
void INS_storeByte(REGS desReg, REGS arg0);
void INS_loadWord(REGS desReg, REGS arg0);
void INS_loadAddress(uint32_t index,REGS arg0);
void INS_storeWord(REGS desReg, REGS arg0);
void INS_move(REGS desReg, REGS arg0);
void INS_jump(uint32_t index);
void INS_ret();
void INS_syscall(uint32_t number);
void INS_nop();

typedef void (*three_reg) (REGS desReg ,REGS arg0, REGS arg1);
typedef void (*two_reg_num) (REGS desReg ,REGS arg0, uint32_t integer);
typedef void (*two_reg) (REGS arg0 ,REGS arg1);
typedef void (*reg_num) (REGS desReg, uint32_t integer);
typedef void (*num_reg) (uint32_t index, REGS arg0);
typedef void (*number) (uint32_t index);
typedef void (*no_args) ();

#endif /*instruction.h*/