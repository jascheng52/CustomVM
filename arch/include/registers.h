#ifndef REG_H
#define REG_H

typedef enum{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    ARG0,
    ARG1,
    RT,
    FP,
    SP,
    LO,
    HI,
    IP,
    ZERO_NUM,
    NA_REG
}REGS;

//String to enum mapping. NA_REG is no match
static const char *const VALID_REG[] = {
    [ZERO] = "$s0",
    [ONE] = "$s1",
    [TWO] = "$s2",
    [THREE] = "$s3",
    [FOUR] = "$s4",
    [FIVE] = "$s5",
    [SIX] = "$s6",
    [SEVEN] = "$s7",
    [ARG0] = "$a0",
    [ARG1] = "$a1",
    [RT] = "$rt",
    [FP] = "$fp",
    [SP] = "$sp",
    [LO] = "$lo",
    [HI] = "$hi",
    [IP] = "$ip",
    [ZERO_NUM] = "$0", //Regist value is constant 0
    [NA_REG] = ""};

#endif /*REG_H*/