#include "asm_head.h"

void writeBinInternal (void* val, FILE* outFile, size_t sizeOfVar) {

    assert (outFile != NULL);

    for (int i = 0; i < sizeOfVar; i++) {

        fputc (*(((char*)val) + i), outFile);
        ip += sizeof (char);
    }
}

void handleArg (Text* code, int line, FILE* outFile, char cmdNum, int tags[512]) {

    char arg1[100] = {0};
    char arg2[100] = {0};

    int it1 = 1,
        it2 = 1;

    char temp = *code->Lines[line].end;

    *code->Lines[line].end = '\0';

    switch (sscanf (code->Lines[line].begin, "%s%[^+\n\0]%[^\n\0 ]", arg1, arg1, arg2)) {

        case 2:

            if (arg1[it1] == '[') {

                cmdNum |= MASK_RAM;
                it1++;
            }

            if(arg1[it1] == 'r' and arg1[it1 + 2] == 'x') {

                cmdNum |= MASK_REG;

                fputc (cmdNum, outFile);
                ip += sizeof (char);

                fputc (arg1[it1 + 1] - 'a' + 1, outFile);
                ip+=sizeof (char);
            }
            else if (arg1[it1] >= '0' and arg1[it1] <= '9' or arg1[it1] == '-')  {

                cmdNum |= MASK_IMM;

                fputc (cmdNum, outFile);
                ip += sizeof (char);


                double value = 0;
                sscanf (arg1 + it1, elem_t_F, &value);
                writeBin (value, outFile);
            }
            else if (arg1[it1] == ':') {

                fputc (cmdNum, outFile);
                ip += sizeof (char);
                int index = strtol (arg1 + it1 + 1, NULL, 10);

                writeBin (tags[index], outFile);
            }
            else {

                printf ("Wrong arg at line %d : %s", line + 1, *code->Lines[line].begin);
                exit (0);
            }
        break;

        case 3:

            if (arg1[it1] == '[') {

                cmdNum |= MASK_RAM;
                it1++;
            }

            cmdNum |= MASK_REG;
            cmdNum |= MASK_IMM;

            if (arg1[it1] == 'r' and arg1[it1 + 2] == 'x') {


                if (arg2[0] != '+') {

                    printf ("Wrong arg at line %d : %s", line + 1, *code->Lines[line].begin);
                    exit (0);
                }

                fputc (cmdNum, outFile);
                ip += sizeof (char);

                fputc (arg1[it1 + 1] - 'a' + 1, outFile);
                ip += sizeof (char);

                double value = 0;
                sscanf (arg2 + it2, elem_t_F, &value);
                writeBin (value, outFile);
            }
            else if (arg1[it1] >= '0' and arg1[it1] <= '9' or arg1[it1] == '-') {

                if (arg2[0] != '+') {

                    printf ("Wrong arg at line %d : %s", line + 1, *code->Lines[line].begin);
                    exit (0);
                }

                fputc (cmdNum, outFile);
                ip += sizeof (char);

                fputc (arg2[it2] - 'a' + 1, outFile);
                ip += sizeof (char);

                double value = 0;
                sscanf (arg1 + it1, elem_t_F, &value);
                writeBin (value, outFile);
            }
        break;

        default:

            printf ("Wrong operator in line %d : %s", line + 1, *code->Lines[line].begin);
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

void writeWtf (Text* codeFile, FILE* outFile, int tags[512]) {

    fprintf (outFile, "%s", signa);

    ip += 4 * sizeof (char);

    for (int i = 0; i < codeFile->stringCnt; i++) {

        char inputStr[100] = {0};

        sscanf (codeFile->Lines[i].begin, "%s", inputStr);

        #undef DEF_CMD

        #define DEF_CMD(name, num, arg, code)                         \
            if (strcmp (inputStr, #name) == 0) {                      \
                                                                      \
                if (arg == 0){                                        \
                                                                      \
                    fputc (num, outFile);                             \
                    ip += sizeof (char);                              \
                }                                                     \
                else if (arg != 0) {                                  \
                                                                      \
                    handleArg (codeFile, i, outFile, num, tags);      \
                }                                                     \
            }                                                         \
            else

        #include header(cmd)

        if (inputStr[strlen (inputStr) - 1] == ':') {

            int tagNum = 0;
            sscanf (codeFile->Lines[i].begin, "%d", &tagNum);
            tags[tagNum] = ip;
        }
        else {

            printf ("Wrong operator at line %d : %s", i + 1, inputStr);
            exit (0);
        }
    }
}
