#include "asm_head.h"

void handleArg (Text* code, int line, FILE* outFile, char cmdNum, Tag tags[TAGS_SIZE]) {

    assert (code != NULL);
    assert (tags != NULL);

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

                tagCheck (fputc (cmdNum, outFile);)
                ip += sizeof (char);

                tagCheck (fputc (arg1[it1 + 1] - 'a' + 1, outFile);)
                ip+=sizeof (char);
            }
            else if (arg1[it1] >= '0' and arg1[it1] <= '9' or arg1[it1] == '-')  {

                cmdNum |= MASK_IMM;

                tagCheck (fputc (cmdNum, outFile);)
                ip += sizeof (char);


                double value = 0;
                sscanf (arg1 + it1, elem_t_F, &value);
                fwrite (&value, 1, sizeof (double), outFile);
            }
            else if (arg1[it1] == ':') {

                tagCheck (fputc (cmdNum, outFile);)
                ip += sizeof (char);

                arg1[strlen (arg1) - 1] = '\0';
                for (int i = 0; i <= TAGS_SIZE; i++) {

                    if (i == TAGS_SIZE) {

                        unsigned int t = -1;
                        tagCheck (fwrite (&t, 1, sizeof (unsigned int), outFile);)
                        ip += sizeof (unsigned int);
                    }
                    else if (strcmp (tags[i].name, arg1 + it1 + 1) == 0) {

                        tagCheck (fwrite (&tags[i].ip, 1, sizeof (unsigned int), outFile);)
                        ip += sizeof (unsigned int);
                        break;
                    }
                }
            }
            else {

                printf ("Wrong arg at line %d : %s", line + 1, *code->Lines[line].begin);
                errors = 1;
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

                tagCheck (fputc (cmdNum, outFile);)
                ip += sizeof (char);

                tagCheck (fputc (arg1[it1 + 1] - 'a' + 1, outFile);)
                ip += sizeof (char);

                double value = 0;
                sscanf (arg2 + it2, elem_t_F, &value);
                fwrite (&value, 1, sizeof (double), outFile);
            }
            else if (arg1[it1] >= '0' and arg1[it1] <= '9' or arg1[it1] == '-') {

                if (arg2[0] != '+') {

                    printf ("Wrong arg at line %d : %s", line + 1, *code->Lines[line].begin);
                    exit (0);
                }

                tagCheck (fputc (cmdNum, outFile);)
                ip += sizeof (char);

                tagCheck (fputc (arg2[it2] - 'a' + 1, outFile);)
                ip += sizeof (char);

                double value = 0;
                sscanf (arg1 + it1, elem_t_F, &value);
                fwrite (&value, 1, sizeof (double), outFile);
            }
        break;

        default:

            printf ("Wrong operator in line %d : %s\n", line + 1, *code->Lines[line].begin);
            errors = 1;
        break;
    }

    *code->Lines[line].end = temp;

}

char* handleComLine (int argc, char* argv[], bool* aFlag, char** outFileName) {

    assert (argv != NULL);
    assert (aFlag != NULL);
    assert (outFileName != NULL);

    char* fileName = NULL;

    switch (argc) {

        case 1:

            printf ("Usage hint: ./wtfcomp.exe fileName [-a] [-o outFileName]\n"
                    "fileName - name of file to compile (commonly .codeFile)\n"
                    "-o - optional key to set name of output file .wtf\n"
                    "outFileName - name of output file with -o (set to a.wtf by default)\n");
            exit(0);
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

            printf ("Wrong command line args");
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

void writeWtf (Text* codeFile, FILE* outFile, Tag tags[TAGS_SIZE]) {

    assert (codeFile != NULL);
    assert (tags != NULL);

    tagCheck (fprintf (outFile, "%s", signa);)

    ip += 4 * sizeof (char);

    for (int i = 0; i < codeFile->stringCnt; i++) {

        char inputStr[100] = {0};

        sscanf (codeFile->Lines[i].begin, "%s", inputStr);

        if (inputStr[0] == '#') continue;

        #undef DEF_CMD

        #define DEF_CMD(name, num, arg, code)                         \
            if (strcmp (inputStr, #name) == 0) {                      \
                                                                      \
                if (arg == 0){                                        \
                                                                      \
                    tagCheck(fputc (num, outFile);)                   \
                    ip += sizeof (char);                              \
                }                                                     \
                else if (arg != 0) {                                  \
                                                                      \
                    handleArg (codeFile, i, outFile, num, tags);      \
                }                                                     \
            }                                                         \
            else

        #include header(cmd)

        #undef DEF_CMD

        if (inputStr[strlen (inputStr) - 1] == ':') {

            for (int i = 0; i <= TAGS_SIZE; i++) {

                if (i == TAGS_SIZE) {

                    printf ("Exceeded tag array at line %d : %s\n", i + 1, inputStr);
                    errors = 1;
                }
                else if (tags[i].name[0] == '\0') {

                    // printf ("%s\n", inputStr);
                    strcpy (tags[i].name, inputStr);
                    // printf ("%s\n", tags[i].name);
                    tags[i].name[strlen (inputStr) - 1] = '\0';
                    tags[i].ip = ip;
                    break;
                }
            }
        }
        else {

            printf ("Wrong operator at line %d : %s\n", i + 1, inputStr);
            errors = 1;
        }
    }
}

void TagCtor (Tag* tag) {

    tag->ip = -1;
    for (int i = 0; i < MAX_TAG_NAME_LEN; i++) {

        tag->name[i] = '\0';
    }
}
