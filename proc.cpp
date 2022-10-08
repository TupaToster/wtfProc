#include "proc_head.h"

int main (int argc, char* argv[]) {

    char* codeFileName = NULL;

    switch (argc) {

        case 1:

            printf ("Good usage of wtfproc.exe:\n"
                    "./wtfproc.exe fileName\n"
                    "fileName - name of file to run (usually a .wtf)");
            return 0;
        break;

        case 2:

            codeFileName = (char*) calloc (strlen (argv[2]) + 1, sizeof (char));
            assert (codeFileName != NULL);

            strcpy (codeFileName, argv[2]);

        default:

            printf ("Wrong args");
            return 0;
        break;
    }

    Text codeFile = read_Text (codeFileName);

    char* code = codeFile.TextString;

    size_t codeLen = codeFile.TextSize;
}