#include "asm_head.h"

int main (int argc, char* argv[]) {

    char* fileName    = NULL;
    char* outFileName = NULL;
    bool  aFlag       = 0;

    switch (argc) {

        case 1:

            printf ("Usage hint: ./wtfcomp.exe fileName [-a] [-o outFileName]\n"
                    "fileName - name of file to compile (commonly .code)\n"
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
                return 0;
            }

            outFileName = (char*) calloc (strlen (argv[3] + 1), sizeof (char));
            assert (outFileName != NULL);

            strcpy (outFileName, argv[3]);
        break;

        default:

            printf ("Wrong args");
            return 0;
        break;
    }

    if (outFileName == NULL) {

        outFileName = (char*) calloc (strlen ("a.wtf") + 1, sizeof (char));
        assert (outFileName != NULL);

        strcpy (outFileName, "a.wtf");
    }

    FILE* outFile = fopen (outFileName, "wb");
    assert (outFile != NULL);

    assert (fileName    != NULL);
    assert (outFileName != NULL);

    Text code = read_Text (fileName);

    fprintf (outFile, "%s", signa);

    for (int i = 0; i < code.stringCnt; i++) {

        char inputStr[100] = {0};

        double val = 0;

        sscanf (code.Lines[i].begin, "%s%Lf", inputStr, &val);

        #undef DEF_CMD

        #define DEF_CMD(name, num, arg, code)       \
            if (strcmp (inputStr, #name) == 0) {    \
                                                    \
                fputc (num, outFile);              \
                if (arg == 1) {                     \
                                                    \
                    writeBin (val, outFile);        \
                }                                   \
            }                                       \
            else

        #include "cmd.h"

        //else
        {
            printf ("Wrong operator at line %d", i + 1);
            return 0;
        }
    }

    killText (&code);
    fclose (outFile);
    free (fileName);
    free (outFileName);
}