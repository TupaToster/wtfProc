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

char* handleComLine (int argc, char* argv[], bool* aFlag, char** outFileName) {

    char* fileName = NULL;

    switch (argc) {

        case 1:

            printf ("Usage hint: ./wtfcomp.exe fileName [-a] [-o outFileName]\n"
                    "fileName - name of file to compile (commonly .codeFile)\n"
                    "-o - optional key to set name of output file .wtf\n"
                    "outFileName - name of output file with -o (set to a.wtf by default)\n");
            return 0;
        break;
        case 2:

            fileName = (char*) calloc (strlen (argv[1] + 1), sizeof (char));
            assert (fileName != NULL);

            strcpy (fileName, argv[1]);
        break;

        case 4:

            fileName = (char*) calloc (strlen (argv[1] + 1), sizeof (char));
            assert (fileName != NULL);

            strcpy (fileName, argv[1]);

            if (strcmp (argv[2], "-o")) {

                printf ("Wrong args");
                exit (0);
            }

            *outFileName = (char*) calloc (strlen (argv[3] + 1), sizeof (char));
            assert (*outFileName != NULL);

            strcpy (*outFileName, argv[3]);
        break;

        default:

            printf ("Wrong args");
            exit (0);
        break;
    }

    if (*outFileName == NULL) {

        *outFileName = (char*) calloc (strlen ("a.wtf") + 1, sizeof (char));
        assert (*outFileName != NULL);

        strcpy (*outFileName, "a.wtf");
    }

    return fileName;
}

void writeWtf (Text* codeFile, FILE* outFile) {

    fprintf (outFile, "%s", signa);

    for (int i = 0; i < codeFile->stringCnt; i++) {

        char inputStr[100] = {0};

        sscanf (codeFile->Lines[i].begin, "%s", inputStr);

        #undef DEF_CMD

        #define DEF_CMD(name, num, arg, code)                         \
            if (strcmp (inputStr, #name) == 0) {                      \
                                                                      \
                if (arg == 0) fputc (num, outFile);                   \
                else if (arg == 1) {                                  \
                                                                      \
                    handleArg (codeFile, i, outFile, num);           \
                }                                                     \
            }                                                         \
            else

        #include header(cmd)

        //else
        {
            printf ("Wrong operator at line %d", i + 1);
            exit (0);
        }
    }
}
