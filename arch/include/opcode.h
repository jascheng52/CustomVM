#ifndef OP_H
#define OP_H

#define NUM_INSTR 23
#define NUM_REG 16
#define WORD_LENGTH 32
//Enum of valid instructions, NA means none
//When adding or removing instructions update the string array as well
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
    JMP,
    CALL,
    RET,
    SYSCALL,
    NOP,
    NA_OP
}OPS;

//Expected arg type. R=Register L=Label N=Number
typedef enum{
    NO_ARG,
    R,
    L,
    N,
    R_N,
    R_R,
    R_L,
    R_R_N,
    R_R_R
}REG_ARG_TYPE;

//enum numval compare with string
//NA should never be used for the most part
static const char *const VALID_INS[] = {
    [NA_OP] = "",
	[ADDI] = "addi",
	[ADD] = "add",
	[MULT_I] = "multi",
	[MULT] = "mult",
    [AND] = "and",
    [OR] = "or",
    [XOR] = "xor",
    [NOT] = "not",
    [SL] = "sl",
    [SR] = "sr",
    [GRT] = "grt",
    [LST] = "lst",
    [EQU] = "equ",
    [LDB] = "ldb",
    [STRB] = "strb",
    [LDW] = "ldw",
    [STRW] = "strw",
    [MV] = "mv",
    [JMP] = "jmp",
    [CALL] = "call",
    [RET] = "ret",
    [SYSCALL] = "syscall",
    [NOP] = "NOP"
};

static const REG_ARG_TYPE const REG_TYPE_MAP[] = {
    [NA_OP] = NO_ARG,
	[ADDI] = R_R_N,
	[ADD] = R_R_R,
	[MULT_I] = R_N,
	[MULT] = R_R,
    [AND] = R_R_R,
    [OR] = R_R_R,
    [XOR] = R_R_R,
    [NOT] = R,
    [SL] = R_N,
    [SR] = R_N,
    [GRT] = R_R_R,
    [LST] = R_R_R,
    [EQU] = R_R_R,
    [LDB] = R_R,
    [STRB] = R_R,
    [LDW] = R_R,
    [STRW] = R_R,
    [MV] = R_R,
    [JMP] = R_L,
    [CALL] = L,
    [RET] = NO_ARG,
    [SYSCALL] = N,
    [NOP] = NO_ARG    

};


#endif /*OP_H*/

