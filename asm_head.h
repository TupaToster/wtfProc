#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "flog.h"
#include "cmd.h"
#include "text.h"

enum CMD {

    #undef DEF_CMD
    #define DEF_CMD(name, num, arg, code) CMD_##name = num,

    #include "cmd.h"
};

enum masks {

    MASK_RAM = 1<<7,
    MASK_REG = 1<<6,
    MASK_IMM = 1<<5,
    MASK_CMD = (1<<5) - 1
};

void writeBin (elem_t val, FILE* outFile);

void handleArg (Text* code, int line, FILE* outFile, char cmdNum);