#pragma once
#include "D:\los_projectados\wtfProc\lib\globalization.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include header(stack)
#include header(text)

/// @brief Variable for processor version, declared in proc_funcs.cpp
extern char Proc_version[3];

#define EPS 0.00001

/// @brief Macros for dumping processor
#define ProcDump(cpu)        \
    StackDump (cpu->stk);    \
    StackDump (cpu->funcIp); \
    ProcDumpInside (cpu);    \


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
void ProcRunCode (Proc* cpu);

/// @brief Gets a elem_t arg as a value
/// @param cpu Ptr to cpu to read from
/// @return elem_t value
elem_t getValueArg (Proc* cpu);

/// @brief Gets a elem_t* arg
/// @param cpu Ptr to cpu to read from
/// @return elem_t* value
elem_t* getPtrArg (Proc* cpu);

/// @brief Gets an ip
/// @param cpu Ptr to cpu to read from
/// @return int value
int getIpArg (Proc* cpu);

bool cmp (elem_t a, elem_t b);