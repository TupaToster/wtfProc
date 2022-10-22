#include "proc_head.h"

char* handleComLine (int argc, char* argv[]) {

    assert (argv != NULL);

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

    assert (cpu != NULL);

    cpu->code      = NULL;
    cpu->regs[rax] = 0;
    cpu->regs[rbx] = 0;
    cpu->regs[rcx] = 0;
    cpu->regs[rdx] = 0;
    cpu->regs[r0x] = 0;
    cpu->code      = 0;
    cpu->ip        = 0;
    cpu->stk       = StackCtor ();
    cpu->funcIp    = StackCtor ();
    cpu->ram       = (elem_t*) calloc (RAM_SIZE, sizeof (elem_t));
}

void ProcDtor (Proc* cpu) {

    assert (cpu != NULL);

    free (cpu->code);
    free (cpu->ram);
    free (cpu->regs);
    cpu->codeSize = 0;
    cpu->ip       = 0;
    StackDtor (&cpu->stk);
    StackDtor (&cpu->funcIp);
}

void checkFileSign (Proc* cpu) {

    assert (cpu != NULL);
    assert (cpu->code != NULL);

    if (!(cpu->code[0] == signa[0]
    and   cpu->code[1] == signa[1])) {

        printf ("Wrong file signature");
        exit (0);
    }

    if (!(cpu->code[2] == signa[2]
    and   cpu->code[3] == signa[3])) {

        printf ("Wrong file version \n"
                "file      : %.2s   \n"
                "processor : %s     \n", cpu->code + 2, signa + 2);
        exit (0);
    }

    cpu->ip = 4;
}

void ProcDumpInside (Proc* cpu) {

    assert (cpu       != NULL);
    assert (cpu->code != NULL);
    assert (cpu->ram  != NULL);
    assert (cpu->regs != NULL);

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

    flogprintf ("/| ip = %d, cpu->code[ip] = %02X\n", cpu->ip - 1, cpu->code[cpu->ip - 1]);

    flogprintf ("RAM: \n"
                "| ");

    for (int i = 0; i < RAM_SIZE; i++) {

        flogprintf (elem_t_F " | ", cpu->ram[i]);
    }

    flogprintf ("\n");
}

bool cmp (elem_t a, elem_t b) {
    return fabs (a - b) < EPS;
}

elem_t* getArg (Proc* cpu, int argType) {

    assert (cpu       != NULL);
    assert (cpu->code != NULL);
    assert (cpu->ram  != NULL);
    assert (cpu->regs != NULL);

    char command = cpu->code[cpu->ip - 1];

    elem_t* retVal   = &cpu->regs[r0x];
    elem_t  immConst = 0;

    *retVal = 0;

    if (command & MASK_REG) {

        if (argType != PTR_ARG) *retVal += cpu->regs[cpu->code[cpu->ip]];
        else retVal = cpu->regs + cpu->code[cpu->ip];
        cpu->ip += sizeof (char);
    }

    if (command & MASK_IMM or argType == 3) {

        *retVal += *(elem_t*)(cpu->code + cpu->ip);
        immConst = *(elem_t*)(cpu->code + cpu->ip);
        cpu->ip +=  sizeof (elem_t);
        flog (*retVal);
    }

    if (command & MASK_RAM) {

        size_t ind = (size_t) *retVal;
        *retVal -= immConst;
        retVal = cpu->ram + ind;
    }

    return retVal;
}

void ProcRunCode (Proc* cpu) {

    assert (cpu       != NULL);
    assert (cpu->code != NULL);
    assert (cpu->ram  != NULL);
    assert (cpu->regs != NULL);

    while (cpu->ip < cpu->codeSize) {

        elem_t* cmdArg = NULL;

        switch ((cpu->code[cpu->ip++] & MASK_CMD)) {

            #undef DEF_CMD

            #define DEF_CMD(name, num, arg, code)                    \
                case CMD_##name:                                     \
                                                                     \
                    cmdArg = getArg (cpu, arg);                      \
                    assert (cmdArg != NULL);                         \
                    code                                             \
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
