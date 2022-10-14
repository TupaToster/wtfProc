#include "proc_head.h"

char Proc_version[3] = "02";

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
    cpu->ram       = (elem_t*) calloc (100, sizeof (elem_t));
}

void ProcDtor (Proc* cpu) {

    free (cpu->code);
    free (cpu->ram);
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

elem_t getValueArg (Proc* cpu) {

    char command = cpu->code[cpu->ip - 1];

    elem_t retVal = 0;

    if ((command & MASK_CMD) == CMD_jmp) {

        retVal = (elem_t) *(int*)(cpu->code + cpu->ip);
        cpu->ip += sizeof (int);
        return retVal;
    }

    if (command & MASK_REG) {

        retVal += cpu->regs[cpu->code[cpu->ip]];
        cpu->ip += sizeof (char);
    }
    if (command & MASK_IMM) {

        retVal += *(elem_t*)(cpu->code + cpu->ip);
        cpu->ip += sizeof (retVal);
    }

    if (command & MASK_RAM) {

        retVal = cpu->ram[(size_t) retVal];
    }
    return retVal;
}

elem_t* getPtrArg (Proc* cpu) {


    char command = cpu->code[cpu->ip - 1];

    elem_t* retVal = NULL;

    if (command & MASK_REG) {

        retVal = cpu->regs + cpu->code[cpu->ip];
        cpu->ip += sizeof (char);
    }

    if (command & MASK_IMM) {

        retVal = (elem_t*)(cpu->code + cpu->ip);

        if (command & MASK_RAM) {
            retVal = cpu->ram + (size_t) *retVal + (size_t) cpu->code[cpu->ip];
        }
        else exit(0);

        cpu->ip += sizeof (elem_t);
    }
    else if (command & MASK_RAM) {

        retVal = cpu->ram + (size_t) *retVal;
    }

    return retVal;
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

        elem_t* ptrArg  = NULL;
        elem_t  valArg = 0;

        switch ((cpu->code[cpu->ip++] & MASK_CMD)) {

            #undef DEF_CMD

            #define DEF_CMD(name, num, arg, code)                    \
                case CMD_##name:                                     \
                                                                     \
                    if      (arg == 1) valArg = getValueArg (cpu);   \
                    else if (arg == 2) ptrArg = getPtrArg (cpu);     \
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
