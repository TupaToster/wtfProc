#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "D:\los_projectados\wtfProc\lib\globalization.h"
#include header(flog)
#include header(cmd)
#include header(text)

#define MAX_TAG_NAME_LEN 100

/// @brief Macros to check for no writing run of writeWtf
#define tagCheck(...) if (outFile != NULL) { \
    __VA_ARGS__                              \
}

/// @brief Counts ip of current instruction in cpu->code
extern size_t ip;

/// @brief Is a flag for if errors have occured
extern bool errors;

struct Tag {

    char  name[MAX_TAG_NAME_LEN ] = {0};
    int   ip                      = -1;
};

void TagCtor (Tag* tag);

/// @brief Macros for writeBinInternal
#define writeBin(var, outFile) writeBinInternal (&var, outFile, sizeof (var))

/// @brief Internal function for writeBin macros. Is a memcpy to file basically
/// @param val Value to write
/// @param outFile File to write to
/// @param sizeOfVar Size of variable
void writeBinInternal (void* val, FILE* outFile, size_t sizeOfVar);

/// @brief Handles argument of a command
/// @param code Text of program in asm
/// @param line Current line
/// @param outFile File to write to
/// @param cmdNum Number of command it's counting arg for
/// @param tags Array of tags for jumps
void handleArg (Text* code, int line, FILE* outFile, char cmdNum, Tag tags[512]);

/// @brief Handles command line arguments
/// @param argc Amount of arguments
/// @param argv Array of arguments
/// @param aFlag Wrote by function. Indicates if no check for up to date (unused currently)
/// @param outFileName Wrote by function. Name of file to write to
/// @return Name of file to read from
char* handleComLine (int argc, char* argv[], bool* aFlag, char** outFileName);

/// @brief Writes into a .wtf binary file
/// @param codeFile Struct with source code
/// @param outFile File to write to
/// @param tags Array of tags for jumps
void writeWtf (Text* codeFile, FILE* outFile, Tag tags[512]);