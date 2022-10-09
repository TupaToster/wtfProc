#include "asm_head.h"

void writeBin (elem_t val, FILE* outFile) {

    assert (outFile != NULL);

    for (int i = 0; i < sizeof (elem_t); i++) {

        fputc (*(((char*)&val) + i), outFile);
    }
}

void handleArg (Text* code, int line, FILE* outFile, char cmdNum) {

    char arg[100] = {0};

    char temp = *code->Lines[line].end;
    *code->Lines[line].end = '\0';


    sscanf (code->Lines[line].begin, "%s%s", arg, arg);

    if (arg[0] == 'r' and arg[2] == 'x') {

        fputc (cmdNum | MASK_REG, outFile);

        fputc ((char) arg[1] - 'a' + 1, outFile);
    }
    else if (cmdNum != CMD_pop) {

        fputc (cmdNum | MASK_IMM, outFile);

        writeBin (strtod (arg, NULL), outFile);
    }
    else {

        printf ("Wrong args");
        exit (0);
    }


    *code->Lines[line].end = temp;
}