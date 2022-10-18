#include "deasm_head.h"

char Proc_version[3] = "02";

char* handleComLine (int argc, char* argv[]) {

    char* codeFileName = NULL;

    switch (argc) {

        case 1:

            printf ("Good usage of wtfdeasm.exe:\n"
                    "./wtfdeasm.exe fileName\n"
                    "fileName - name of file to disassemble (usually a .wtf)");
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
    cpu->funcIp    = StackCtor ();
    cpu->ram       = (elem_t*) calloc (100, sizeof (elem_t));
}

void ProcDtor (Proc* cpu) {

    free (cpu->code);
    free (cpu->ram);
    free (cpu->regs);
    cpu->codeSize = 0;
    cpu->ip       = 0;
    StackDtor (&cpu->stk);
    StackDtor (&cpu->funcIp);
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
                "file      : %.2s   \n"
                "processor : %s     \n", cpu->code + 2, Proc_version);
        exit (0);
    }

    cpu->ip = 4;
}

void printValuePtrArg (Proc* cpu, FILE* outFile) {

    char command = cpu->code[cpu->ip - 1];

    if ((command & MASK_RAM) and outFile != NULL) fprintf (outFile, "[");

    if (command & MASK_REG) {

        if (outFile != NULL) fprintf (outFile, "r%cx", 'a' + cpu->code[cpu->ip] - 1);
        cpu->ip += sizeof (char);
    }

    if ((command & MASK_REG) and (command & MASK_IMM) and outFile != NULL) fprintf (outFile, "+");

    if (command & MASK_IMM) {

        if (outFile != NULL) fprintf (outFile, elem_t_F, *(elem_t*)(cpu->code + cpu->ip));
        cpu->ip += sizeof (elem_t);
    }

    if ((command & MASK_RAM) and outFile != NULL) fprintf (outFile, "]");
}

void printIpArg (Proc* cpu, FILE* outFile) {

    for (int i = 0; i < 512; i++) {

        if (tags[i] == *(int*)(cpu->code + cpu->ip)) {

            if (outFile != NULL) fprintf (outFile, ":%d", i);
            break;
        }
    }
    cpu->ip += sizeof (int);
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

    flogprintf ("/| ip = %d, cpu->code[ip] = %02X\n", cpu->ip - 1, cpu->code[cpu->ip - 1]);

    flogprintf ("RAM: \n"
                "| ");

    for (int i = 0; i < 100; i++) {

        flogprintf (elem_t_F " | ", cpu->ram[i]);
    }

    flogprintf ("\n");
}

void ProcRunCode (Proc* cpu, FILE* outFile) {

    int tgIter = 0;

    while (cpu->ip < cpu->codeSize) {



        switch ((cpu->code[cpu->ip++] & MASK_CMD)){

            #undef DEF_CMD

            #define DEF_CMD(name, num, arg, program)                                              \
            case CMD_##name:                                                                      \
                for (int i = 0; i < 512; i++) {                                                   \
                                                                                                  \
                    if (tags[i] == cpu->ip - 1) {                                                 \
                        if (outFile != NULL) fprintf (outFile, "%d:\n", i);                       \
                        break;                                                                    \
                    }                                                                             \
                }                                                                                 \
                if (outFile != NULL) fprintf (outFile, #name " ");                                \
                if      (arg == 1 or arg == 2) printValuePtrArg   (cpu, outFile);                 \
                else if (arg == 3 and outFile != NULL) printIpArg (cpu, outFile);                 \
                else if (arg == 3 and outFile == NULL) {                                          \
                                                                                                  \
                    for (int i = 0; i < 513; i++) {                                               \
                                                                                                  \
                        if (i == 512) {                                                           \
                            tags[tgIter] = *(int*)(cpu->code + cpu->ip);                          \
                            tgIter++;                                                             \
                        }                                                                         \
                        if (tags[i] == *(int*)(cpu->code + cpu->ip)) break;                       \
                    }                                                                             \
                    cpu->ip += sizeof (int);                                                      \
                }                                                                                 \
                if (outFile != NULL) fprintf (outFile, "\n");                                     \
            break;

            #include header(cmd)

            default:
                printf ("Wrong command");
                ProcDump (cpu);
                exit(0);
            break;

        }
    }
}