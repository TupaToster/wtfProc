#ifndef GENERAL_CMD

typedef double elem_t;
#define elem_t_OUT "%Lf"

#define PUSH(val) StackPush (&cpu->stk, val)
#define POP StackPop (&cpu->stk)

#define GENERAL_CMD

#define signa "CP01"

#define DEF_CMD(name, num, arg, code) ;

#else

DEF_CMD (push, 1, 1, {
    PUSH (*argument);
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
    printf (elem_t_OUT "\n", POP);
})

DEF_CMD (dump, 7, 0, {
    ProcDump (cpu);
})

DEF_CMD (hlt, 0, 0, {
    exit (0);
})

DEF_CMD (pop, 8, 1, {
    *argument = POP;
})
#endif
