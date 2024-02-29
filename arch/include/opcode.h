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


#endif /*OP_H*/

