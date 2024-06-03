#ifndef INSTRUCTION


#define INSTRUCTION
#include <registers.h>
#include <stdint.h>


void INS_addInteger(REGS desReg,REGS argReg, int32_t integer );
void INS_add(REGS desReg, REGS arg0Reg, REGS arg1Reg);
void INS_multInteger(REGS arg, int32_t integer);
void INS_mult(REGS arg0, REGS arg1);
void INS_or(REGS desReg, REGS arg0, REGS arg1);
void INS_and(REGS desReg, REGS arg0, REGS arg1);
void INS_xor(REGS desReg, REGS arg0, REGS arg1);
void INS_not(REGS desReg, REGS arg);
void INS_shiftLeft(REGS desReg, REGS arg, uint32_t shift);
void INS_shiftRight(REGS desReg, REGS arg, uint32_t shift);
void INS_greaterThan(REGS desReg, REGS arg0, REGS arg1);
void INS_lessThan(REGS desReg, REGS arg0, REGS arg1);
void INS_equal(REGS desReg, REGS arg0, REGS arg1);



#endif /*instruction.h*/