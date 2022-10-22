#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/// @brief Is a flag for if errors have occured
extern bool Errors;

struct Tag {

    char   name[MAX_TAG_NAME_LEN ] = {0};
    elem_t ip                      = -1;
};

void TagCtor (Tag* tag);

void TagsCtor (Tag* tags);

/// @brief Internal function for writeBin macros. Is a memcpy to file basically
/// @param val Value to write
/// @param outFile File to write to
/// @param sizeOfVar Size of variable
void writeBinInternal (void* val, FILE* outFile, size_t sizeOfVar, size_t* Ip);

/// @brief Handles argument of a command
/// @param code Text of program in asm
/// @param line Current line
/// @param outFile File to write to
/// @param cmdNum Number of command it's counting arg for
/// @param tags Array of tags for jumps
void handleArg (Text* code, int line, FILE* outFile, char cmdNum, Tag tags[512], size_t* Ip);

/// @brief Handles command line arguments
/// @param argc Amount of arguments
/// @param argv Array of arguments
/// @param aFlag Wrote by function. Indicates if no check for up to date (unused currently)
/// @param outFileName Wrote by function. Name of file to write to
/// @return Name of file to read from
char* handleComLine (int argc, char* argv[], char** outFileName);

/// @brief Writes into a .wtf binary file
/// @param codeFile Struct with source code
/// @param outFile File to write to
/// @param tags Array of tags for jumps
void writeWtf (Text* codeFile, FILE* outFile, Tag tags[512], size_t* Ip);
