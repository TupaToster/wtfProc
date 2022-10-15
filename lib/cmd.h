/*! \mainpage Index page

    \section Intorduction

    This is a soft-cpu project with it's own asm language and hand written function to work with text and a stack to store data during it's work.

    \section ASM

    Language consists of commands: push, pop, add, sub, mult, div, out. dump, hlt, pop, jmp, ja, jae, jb, jbe, je, jne, call, ret, video, in. More info on those commands can
    be seen in cmd.h, i.e. how they work and what they do.
*/

#ifndef GENERAL_CMD

typedef double elem_t; ///< Base element of cpu
#define elem_t_F "%Lf" ///< Printf format string for elem_t

#define PUSH(val) StackPush (&cpu->stk, val)
#define POP StackPop (&cpu->stk)

#define signa "CP02" ///<signature of a fine file

#define DEF_CMD(name, num, arg, code) ;


#define GENERAL_CMD
#else ///< Template for auto code gen starts here

DEF_CMD (push, 1, 1, {

    PUSH (valArg);
})

DEF_CMD (add, 2, 0, {

    PUSH (POP + POP);
})

DEF_CMD (sub, 3, 0, {

    elem_t temp = POP;
    PUSH (POP - temp);
})

DEF_CMD (mult, 4, 0, {

    PUSH (POP * POP);
})

DEF_CMD (div, 5, 0, {

    elem_t temp = POP;
    PUSH (POP / temp);
})

DEF_CMD (out, 6, 0, {

    printf (elem_t_F "\n", POP);
})

DEF_CMD (dump, 7, 0, {

    ProcDump (cpu);
})

DEF_CMD (hlt, 0, 0, {

    exit (0);
})

DEF_CMD (pop, 8, 2, {

    *ptrArg = POP;
})

DEF_CMD (jmp, 9, 3, {

    cpu->ip = ipArg;
})

DEF_CMD (jb, 10, 3, {

    if (POP > POP) {

        cpu->ip = ipArg;
    }
})

DEF_CMD (jbe, 11, 3, {

    if (POP >= POP) {

        cpu->ip = ipArg;
    }
})

DEF_CMD (ja, 12, 3, {

    if (POP < POP) {

        cpu->ip = ipArg;
    }
})

DEF_CMD (jae, 13, 3, {

    if (POP <= POP) {

        cpu->ip = ipArg;
    }
})

DEF_CMD (je, 14, 3, {

    if (POP == POP) {

        cpu->ip = ipArg;
    }
})

DEF_CMD (jne, 15, 3, {

    if (POP != POP) {

        cpu->ip = ipArg;
    }
})

DEF_CMD (call, 16, 3, {

    StackPush (&cpu->funcIp, cpu->ip);
    cpu->ip = ipArg;
})

DEF_CMD (ret, 17, 0, {

    cpu->ip = (int)StackPop (&cpu->funcIp);
})

DEF_CMD (video, 18, 0, {

    for (int i = 0; i < 10; i++) {

        for (int j = 0; j < 10; j++) {

            if (cpu->ram[10*i + j] == 0) printf (".");
            else printf ("*");
        }
        printf ("\n");
    }
})

DEF_CMD (in, 19, 0, {

    printf ("Insert number to push: ");
    elem_t val = 0;
    scanf (elem_t_F, &val);
    PUSH (val);
})

DEF_CMD (sqrt, 20, 0, {

    PUSH (sqrt (POP));
})
#endif