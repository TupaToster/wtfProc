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

/// @brief Is a flag for if errors have occured
extern bool Errors;

/// @brief Structure for tag's name and ip
struct Tag {

    char   name[MAX_TAG_NAME_LEN] = ""; ///< Nameof tag
    elem_t ip                     = -1; ///< Ip of tag
};

/// @brief Tag constructor
/// @param tag Ptr to var to construct
void TagCtor (Tag* tag);

/// @brief Tag array constructor
/// @param tags Ptr to var to construct
void TagsCtor (Tag* tags);

/// @brief Internal function for writeBin macros. Is a memcpy to file basically
/// @param val Value to write
/// @param outStr String to write to
/// @param sizeOfVar Size of variable
void writeBinInternal (void* val, char* outStr, size_t sizeOfVar, size_t* Ip);

/// @brief Handles argument of a command
/// @param code Text of program in asm
/// @param line Current line
/// @param outStr String to write to
/// @param cmdNum Number of command it's counting arg for
/// @param tags Array of tags for jumps
void handleArg (Text* code, int line, char* outStr, char cmdNum, Tag tags[512], size_t* Ip);

/// @brief Handles command line arguments
/// @param argc Amount of arguments
/// @param argv Array of arguments
/// @param aFlag Wrote by function. Indicates if no check for up to date (unused currently)
/// @param outStrName Wrote by function. Name of file to write to
/// @return Name of file to read from
char* handleComLine (int argc, char* argv[], char** outStrName);

/// @brief Writes into a .wtf binary file
/// @param codeFile Struct with source code
/// @param outStr String to write to
/// @param tags Array of tags for jumps
void writeWtf (Text* codeFile, char* outStr, Tag tags[512], size_t* Ip);

/// @brief Prints a register into binary
/// @param Ip Ip of argument
/// @param commandIp Ip of command
/// @param outStr String to write to
/// @param cmdNum Number of command
/// @param arg Argument string
void printReg (size_t* Ip, size_t commandIp, char* outStr, char* cmdNum, char* arg);

/// @brief Prints an immidiate constant into binary
/// @param Ip Ip of argument
/// @param commandIp Ip of command
/// @param outStr String to write to
/// @param cmdNum Number of command
/// @param value Argument value
void printImm (size_t* Ip, size_t commandIp, char* outStr, char* cmdNum, elem_t value);
