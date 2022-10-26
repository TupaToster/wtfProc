#include "asm_head.h"

void printTag (Tag tags[], char* name, char* outStr, size_t* Ip, size_t commandIp, char cmdNum) {

    if (outStr != NULL) outStr[*Ip] = cmdNum;
    *Ip += sizeof (char);

    name[strlen (name) - 1] = '\0';

    for (int i = 0; i <= TAGS_SIZE; i++) {

        if (i == TAGS_SIZE) {

            elem_t t = -1;
            if (outStr != NULL) *(elem_t*)(outStr + *Ip) = t;
            *Ip += sizeof (elem_t);
        }
        else if (strcmp (tags[i].name, name) == 0) {

            if (outStr != NULL) *(elem_t*)(outStr + *Ip) = tags[i].ip;
            *Ip += sizeof (elem_t);
            break;
        }
    }
}

void addTag (Tag tags[], const char* name, size_t* Ip){

    for (int i = 0; i <= TAGS_SIZE; i++) {

        if (i == TAGS_SIZE) {

            printf ("Exceeded tag array at line : %s\n", name);
            Errors = 1;
        }
        else if (tags[i].name[0] == '\0') {

            strcpy (tags[i].name, name);
            tags[i].name[strlen (name) - 1] = '\0';
            tags[i].ip = (elem_t) *Ip;
            break;
        }
    }
}

void handleArg (Text* code, int line, char* outStr, char cmdNum, Tag tags[TAGS_SIZE], size_t* Ip) {

    assert (code != NULL);
    assert (tags != NULL);

    char arg1[100] = "";
    char arg2[100] = "";

    int it1 = 1;
    int it2 = 1;

    size_t commandIp = *Ip;
    *Ip += sizeof (char);

    elem_t value = 0;

    char temp = *code->Lines[line].end;
    *code->Lines[line].end = '\0';

    if (sscanf (code->Lines[line].begin, "%s%[^+\n\0]%[^\n\0 ]", arg1, arg1, arg2) < 2) {

        printf ("Wrong operator in line %d : %s\n", line + 1, *code->Lines[line].begin);
        Errors = 1;
        goto Return;
    }

    if (arg1[it1] == ':') {

        printTag (tags, arg1 + it1 + 1, outStr, Ip, commandIp, cmdNum);
        goto Return;
    }

    if (arg1[it1] == '[') {

        cmdNum |= MASK_RAM;
        it1++;
    }

    if (arg1[it1] == 'r' and arg1[it1 + 2] == 'x')
        printReg (Ip, commandIp, outStr, &cmdNum, arg1 + it1);

    if (sscanf (arg1 + it1, elem_t_F, &value) == 1)
        printImm (Ip, commandIp, outStr, &cmdNum, value);

    if (arg2[it2] == 'r' and arg2[it2 + 2] == 'x') {

        if (cmdNum & MASK_REG) {

            printf ("Wrong args at line %d : %s \n", line + 1, code->Lines[line].begin);
            Errors++;
            goto Return;
        }

        printReg (Ip, commandIp, outStr, &cmdNum, arg2 + it2);
    }

    if (sscanf (arg2 + it2, elem_t_F, &value) == 1) {

        if (cmdNum & MASK_IMM) {

            printf ("Wrong args at line %d : %s \n", line + 1, code->Lines[line].begin);
            Errors++;
            goto Return;
        }

        printImm (Ip, commandIp, outStr, &cmdNum, value);
    }

    Return:

    *code->Lines[line].end = temp;
}

char* handleComLine (int argc, char* argv[], char** outStrName) {

    assert (argv != NULL);
    assert (outStrName != NULL);

    char* fileName = NULL;

    switch (argc) {

        case 1:

            printf ("Usage hint: ./wtfcomp.exe fileName [-a] [-o outStrName]\n"
                    "fileName - name of file to compile (commonly .codeFile)\n"
                    "-o - optional key to set name of output file .wtf\n"
                    "outStrName - name of output file with -o (set to a.wtf by default)\n");
            return NULL;
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
                return NULL;
            }

            *outStrName = (char*) calloc (strlen (argv[3] + 1), sizeof (char));
            assert (*outStrName != NULL);

            strcpy (*outStrName, argv[3]);
        break;

        default:

            printf ("Wrong command line args");
            return NULL;
        break;
    }

    if (*outStrName == NULL) {

        *outStrName = (char*) calloc (strlen ("a.wtf") + 1, sizeof (char));
        assert (*outStrName != NULL);

        strcpy (*outStrName, "a.wtf");
    }
    return fileName;
}

void writeWtf (Text* codeFile, char* outStr, Tag tags[TAGS_SIZE], size_t* Ip) {

    assert (codeFile != NULL);
    assert (tags != NULL);

    if (outStr != NULL) memcpy (outStr + *Ip, signa, 4);

    *Ip = 4 * sizeof (char);

    for (int i = 0; i < codeFile->stringCnt; i++) {

        char inputStr[100] = "";

        sscanf (codeFile->Lines[i].begin, "%s", inputStr);

        if (inputStr[0] == '#') continue;

        #undef DEF_CMD

        #define DEF_CMD(name, num, arg, code)                         \
            if (strcmp (inputStr, #name) == 0) {                      \
                                                                      \
                if (arg == 0){                                        \
                                                                      \
                    if (outStr != NULL) outStr[*Ip] = num;            \
                    *Ip += sizeof (char);                             \
                }                                                     \
                else if (arg != 0) {                                  \
                                                                      \
                    handleArg (codeFile, i, outStr, num, tags, Ip);   \
                }                                                     \
            }                                                         \
            else

        #include header(cmd)

        #undef DEF_CMD

        if (inputStr[strlen (inputStr) - 1] == ':') {

            addTag (tags, inputStr, Ip);
        }
        else {

            printf ("Wrong operator at line %d : %s\n", i + 1, inputStr);
            Errors = 1;
        }
    }
}

void TagCtor (Tag* tag) {

    tag->ip = -1;
    for (int i = 0; i < MAX_TAG_NAME_LEN; i++) {

        tag->name[i] = '\0';
    }
}

void TagsCtor (Tag* tags) {

    for (int i = 0; i < TAGS_SIZE; i++) {

        TagCtor (tags + i);
    }
}

void printReg (size_t* Ip, size_t commandIp, char* outStr, char* cmdNum, char* arg) {

    *cmdNum |= MASK_REG;

    if (outStr != NULL) outStr[commandIp] = *cmdNum;

    if (outStr != NULL) outStr[*Ip] = arg[1] - 'a' + 1;
    *Ip += sizeof (char);
}

void printImm (size_t* Ip, size_t commandIp, char* outStr, char* cmdNum, elem_t value) {

    *cmdNum |= MASK_IMM;

    if (outStr != NULL) outStr[commandIp] = *cmdNum;

    if (outStr != NULL) *(elem_t*)(outStr + *Ip) = value;
    *Ip += sizeof (elem_t);
}