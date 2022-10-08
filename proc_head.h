#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#pragma once
#include "stack.h"
#include "text.h"

struct Proc {

    char* code = NULL;
    elem_t regs[5] = {0};
    int ip = 0;
    Stack stk = {};
};

