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

            codeFileName = (char*) calloc (strlen (argv[1]) + 1, sizeof (char));
            assert (codeFileName != NULL);

            strcpy (codeFileName, argv[1]);
        break;

        default:

            printf ("Wrong args");
            return 0;
        break;
    }

    Text codeFile = read_Text (codeFileName);

    Proc cpu = {};

    ProcCtor (&cpu);

    cpu.code = codeFile.TextString;


    if (!(cpu.code[0] == 'C'
    and   cpu.code[1] == 'P')) {

        printf ("Wrong file signature");
        return 0;
    }

    cpu.ip = 4;

    while (cpu.ip < codeFile.TextSize) {

        elem_t* argument = NULL;

        switch ((cpu.code[cpu.ip++] & MASK_CMD)) {

            #undef DEF_CMD

            #define DEF_CMD(name, num, arg, code)                 \
                case CMD_##name:                     \
                                                                  \
                    argument = NULL;                              \
                    if (arg == 1) argument = handleArg (&cpu);    \
                    code                                          \
                break;

            #include "cmd.h"

            default:
                printf ("Wrong command");
                ProcDump (cpu);
                return 0;
            break;
        }
    }

    ProcDtor (&cpu);
    killText (&codeFile);
    free (codeFileName);
}