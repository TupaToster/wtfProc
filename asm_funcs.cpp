#include "asm_head.h"

void writeBin (elem_t val, FILE* outFile) {

    assert (outFile != NULL);

    for (int i = 0; i < sizeof (elem_t); i++) {

        fputc (*(((char*)&val) + i), outFile);
    }
}

void handleArg (Text* code, int line, FILE* outFile, char cmdNum) {

    char arg1[100] = {0};
    char arg2[100] = {0};

    char temp = *code->Lines[line].end;

    *code->Lines[line].end = '\0';

    switch (sscanf (code->Lines[line].begin, "%s%[^+\n\0]%[^\n\0 ]", arg1, arg1, arg2)) {

        case 2:

            if (arg1[1] == 'r' and arg1[3] == 'x') {

                fputc (cmdNum | MASK_REG, outFile);

                fputc (arg1[2] - 'a' + (char)1, outFile);
            }
            else if (cmdNum != CMD_pop) {

                fputc (cmdNum | MASK_IMM, outFile);

                writeBin (strtod (arg1 + 1, NULL), outFile);
            }
            else {

                printf ("Wrong arg on line %d", line);
                exit (0);
            }
        break;

        case 3:

            if (arg1[1] == 'r' and arg1[3] == 'x' and cmdNum != CMD_pop) {

                fputc (cmdNum | MASK_IMM | MASK_REG, outFile);

                fputc (arg1[2] - 'a' + (char)1, outFile);

                writeBin (strtod (arg2 + 1, NULL), outFile);
            }
            else if (arg2[1] == 'r' and arg2[3] == 'x' and cmdNum != CMD_pop) {

                fputc (cmdNum | MASK_IMM | MASK_REG, outFile);

                fputc (arg2[2] - 'a' + (char)1, outFile);

                writeBin (strtod (arg1 + 1, NULL), outFile);
            }
            else {

                printf ("Wrong arg on line %d", line);
                exit (0);
            }
        break;

        default:

            printf ("Wrong arg on line %d", line);
            exit (0);
        break;
    }

    *code->Lines[line].end = temp;
}