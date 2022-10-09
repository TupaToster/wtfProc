#include "proc_head.h"

void ProcCtor (Proc* cpu) {

    cpu->code = NULL;
    cpu->regs[rax] = 0;
    cpu->regs[rbx] = 0;
    cpu->regs[rcx] = 0;
    cpu->regs[rdx] = 0;
    cpu->regs[r0x] = 0;
    cpu->ip = 0;
    cpu->stk = StackCtor ();
}

void ProcDtor (Proc* cpu) {

    free (cpu->code);
    free (cpu->regs);
    cpu->ip = 0;
    StackDtor (&cpu->stk);
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