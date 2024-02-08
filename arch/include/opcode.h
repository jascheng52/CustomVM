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

