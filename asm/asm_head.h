#pragma once
#include "D:\los_projectados\wtfProc\lib\globalization.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include header(flog)
#include header(cmd)
#include header(text)

#define tagCheck(...) if (outFile != NULL) { \
    __VA_ARGS__                              \
}

extern size_t ip;

extern bool errors;

enum CMD {

    #undef DEF_CMD
    #define DEF_CMD(name, num, arg, code) CMD_##name = num,

    #include header (cmd)
};

enum masks {

    MASK_RAM = 1<<7,
    MASK_REG = 1<<6,
    MASK_IMM = 1<<5,
    MASK_CMD = (1<<5) - 1,
};

#define writeBin(var, outFile) writeBinInternal (&var, outFile, sizeof (var))

void writeBinInternal (void* val, FILE* outFile, size_t sizeOfVar);

void handleArg (Text* code, int line, FILE* outFile, char cmdNum, int tags[512]);

char* handleComLine (int argc, char* argv[], bool* aFlag, char** outFileName);

void writeWtf (Text* codeFile, FILE* outFile, int tags[512]);