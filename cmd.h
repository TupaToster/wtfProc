#ifndef GENERAL_CMD

#define CPU cpu

typedef double elem_t;
#define elem_t_OUT "%Lf"

#define PUSH(val) StackPush (&CPU.stk, val)
#define POP StackPop (&CPU.stk)
#define DEF_CMD(name, num, arg, code) ;

#define GENERAL_CMD

#define signa "CP01"

#else

DEF_CMD (push, 1, 1, {

})

DEF_CMD (add, 2, 0, {

})

DEF_CMD (sub, 3, 0, {

})

DEF_CMD (mult, 4, 0, {

})

DEF_CMD (div, 5, 0, {

})

DEF_CMD (out, 6, 0, {

})

DEF_CMD (dump, 7, 0, {

})

DEF_CMD (hlt, 0, 0, {

})
#endif
