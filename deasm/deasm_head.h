#pragma once
#include "D:\los_projectados\wtfProc\lib\globalization.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include header(stack)
#include header(text)

/// @brief Variable for processor version, declared in deasm_funcs.cpp
extern char Proc_version[3];

extern char* outStr;

extern int tags[];

/// @brief Macros for dumping processor
#define ProcDump(cpu)        \
    StackDump (cpu->stk);    \
    StackDump (cpu->funcIp); \
    ProcDumpInside (cpu);    \

/// @brief names of regs array elements - regs[r*x]
enum reg {

    r0x = 0, ///< Zero register that servers for wrong address
    rax = 1, ///< First reg
    rbx = 2, ///< Second reg
    rcx = 3, ///< Third reg
    rdx = 4  ///< Fourth reg
};

/// @brief Masks for bits in command byte
enum masks {

    MASK_RAM = 1<<7,      ///< Mask for ram access
    MASK_REG = 1<<6,      ///< Mask for reg access
    MASK_IMM = 1<<5,      ///< Mask for immidiate constant
    MASK_CMD = (1<<5) - 1 ///< Mask for actual command number
};

/// @brief Enumerator for command number (generated automatically)
enum CMD {

    #undef DEF_CMD
    #define DEF_CMD(name, num, arg, code) CMD_##name = num,

    #include header(cmd)
};

/// @brief Struct to store data of a running soft cpu
struct Proc {

    char*   code     = NULL; ///< String of binary code
    size_t  codeSize = 0;    ///< Size of code
    elem_t  regs[5]  = {0};  ///< Array of registers
    int     ip       = 0;    ///< Instruction pointer
    Stack   stk      = {};   ///< Stack of elem_t for work of process
    Stack   funcIp   = {};   ///< Stack of ip for function calls
    elem_t* ram      = NULL; ///< Array that represents RAM
};

/// @brief Handles command line arguments
/// @param argc Amount of args
/// @param argv Array of args
/// @return Name of file to read from
char* handleComLine (int argc, char* argv[]);

/// @brief Constructs a Proc var
/// @param cpu Ptr to var to construct
void ProcCtor (Proc* cpu);

/// @brief Destructs a Proc var
/// @param cpu Ptr to var to destruct
void ProcDtor (Proc* cpu);

/// @brief Dumps processor into logs_out
/// @param cpu Ptr to var to dump
void ProcDumpInside (Proc* cpu);

/// @brief Checks for file signature and proc version
/// @param cpu Ptr to var to dump
void checkFileSign (Proc* cpu);

/// @brief Runs code
/// @param cpu Ptr to var to run from
void ProcRunCode (Proc* cpu, FILE* outFile);

void printValuePtrArg (Proc* cpu, FILE* outFile);

void printIpArg (Proc* cpu, FILE* outFile);
