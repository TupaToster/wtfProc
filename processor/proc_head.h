#pragma once
#include "D:\los_projectados\wtfProc\lib\globalization.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include header(stack)
#include header(text)

extern char Proc_version[3];

#define ProcDump(cpu)\
    StackDump (cpu->stk);\
    ProcDumpInside (cpu);\

enum reg {

    r0x = 0,
    rax = 1,
    rbx = 2,
    rcx = 3,
    rdx = 4
};

enum masks {

    MASK_RAM = 1<<7,
    MASK_REG = 1<<6,
    MASK_IMM = 1<<5,
    MASK_CMD = (1<<5) - 1
};

enum CMD {

    #undef DEF_CMD
    #define DEF_CMD(name, num, arg, code) CMD_##name = num,

    #include header(cmd)
};

struct Proc {

    char*  code     = NULL;
    size_t codeSize = 0;
    elem_t regs[5]  = {0};
    int    ip       = 0;
    Stack  stk      = {};
};

char* handleComLine (int argc, char* argv[]);

void ProcCtor (Proc* cpu);

void ProcDtor (Proc* cpu);

elem_t* handleArg (Proc* cpu);

void ProcDumpInside (Proc* cpu);

void checkFileSign (Proc* cpu);

void ProcRunCode (Proc* cpu);
