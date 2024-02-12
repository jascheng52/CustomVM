#ifndef OP_H
#define OP_H

typedef enum{
    ADDI,
    ADD,
    MULT_I,
    MULT,
    AND,
    OR,
    XOR,
    NOT,
    SL,
    SR,
    GRT,
    LST,
    EQU,
    LDB,
    STRB,
    LDW,
    STRW,
    MV,
    RET,
    NOP
}OPS;

typedef struct instruction
{
    char op[2]; 
}INSTR;

#endif /*OP_H*/

