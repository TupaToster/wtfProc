#include "asm_head.h"

void writeBin (elem_t val, FILE* outFile) {

    assert (outFile != NULL);

    for (int i = 0; i < sizeof (elem_t); i++) {

        fputc (*(((char*)&val) + i), outFile);
    }
}