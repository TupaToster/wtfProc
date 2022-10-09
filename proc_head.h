#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "text.h"

#define ProcDump(cpu)\
    StackDump (cpu.stk);\
    ProcDumpInside (&cpu);\

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

    #include "cmd.h"
};


struct Proc {

    char*  code    = NULL;
    elem_t regs[5] = {0};
    int    ip      = 0;
    Stack  stk     = {};
};

void ProcCtor (Proc* cpu);

void ProcDtor (Proc* cpu);

elem_t* handleArg (Proc* cpu);

void ProcDumpInside (Proc* cpu);
