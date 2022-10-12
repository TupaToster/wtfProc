#include "proc_head.h"

char Proc_version[3] = "01";

char* handleComLine (int argc, char* argv[]) {

    char* codeFileName = NULL;

    switch (argc) {

        case 1:

            printf ("Good usage of wtfproc.exe:\n"
                    "./wtfproc.exe fileName\n"
                    "fileName - name of file to run (usually a .wtf)");
            exit (0);
        break;

        case 2:

            codeFileName = (char*) calloc (strlen (argv[1]) + 1, sizeof (char));

            assert (codeFileName != NULL);

            strcpy (codeFileName, argv[1]);
        break;

        default:

            printf ("Wrong arg");
            exit(0);
        break;
    }

    return codeFileName;
}

void ProcCtor (Proc* cpu) {

    cpu->code      = NULL;
    cpu->regs[rax] = 0;
    cpu->regs[rbx] = 0;
    cpu->regs[rcx] = 0;
    cpu->regs[rdx] = 0;
    cpu->regs[r0x] = 0;
    cpu->code      = 0;
    cpu->ip        = 0;
    cpu->stk       = StackCtor ();
}

void ProcDtor (Proc* cpu) {

    free (cpu->code);
    free (cpu->regs);
    cpu->codeSize = 0;
    cpu->ip       = 0;
    StackDtor (&cpu->stk);
}

void checkFileSign (Proc* cpu) {

    if (!(cpu->code[0] == 'C'
    and   cpu->code[1] == 'P')) {

        printf ("Wrong file signature");
        exit (0);
    }

    if (!(cpu->code[2] == Proc_version[0]
    and   cpu->code[3] == Proc_version[1])) {

        printf ("Wrong file version \n"
                "file      : %.*s   \n"
                "processor : %s     \n");
        exit (0);
    }


    cpu->ip = 4;
}

elem_t* handleArg (Proc* cpu) {

    cpu->regs[r0x] = 0;
    int command = cpu->ip - 1;

    if ((cpu->code[command] & MASK_CMD) == CMD_pop) {

        if (cpu->code[command] & MASK_REG) {

            cpu->ip++;
            return cpu->regs + cpu->code[cpu->ip - 1];
        }
    }
    else if ((cpu->code[command] & MASK_CMD) == CMD_push) {
        if (cpu->code[command] & MASK_REG) {

            cpu->regs[r0x] += cpu->regs[cpu->code[cpu->ip]];
            cpu->ip += sizeof (char);
        }
        if (cpu->code[command] & MASK_IMM) {

            cpu->regs[r0x] += *(elem_t*)(cpu->code + cpu->ip);
            cpu->ip += sizeof (elem_t);
        }

        return cpu->regs + r0x;
    }
}

void ProcDumpInside (Proc* cpu) {

    for (int i = 0; i < 16; i++) {

        flogprintf ("%02X ", i);
    }

    flogprintf ("\n");

    for (int i = 0; i < 16; i++) {

        flogprintf ("%02X ", (unsigned char)cpu->code[cpu->ip - 1 - (cpu->ip - 1) % 16 + i]);
    }

    flogprintf ("\n");

    for (int i = 0; i < (cpu->ip - 1) % 16;i++) {
        flogprintf ("   ");
    }

    flogprintf ("/| ip = %d, cpu->code[ip] = %02X\n"
                "End of cpu dump\n", cpu->ip - 1, cpu->code[cpu->ip - 1]);
}

void ProcRunCode (Proc* cpu) {

    while (cpu->ip < cpu->codeSize) {

        elem_t* argument = NULL;

        switch ((cpu->code[cpu->ip++] & MASK_CMD)) {

            #undef DEF_CMD

            #define DEF_CMD(name, num, arg, code)                 \
                case CMD_##name:                                  \
                                                                  \
                    argument = NULL;                              \
                    if (arg == 1) argument = handleArg (cpu);    \
                    code                                          \
                break;

            #include header(cmd)

            default:
                printf ("Wrong command");
                ProcDump (cpu);
                exit (0);
            break;
        }
    }
}
