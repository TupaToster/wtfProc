#include "proc_head.h"

int main (int argc, char* argv[]) {

    char* codeFileName = handleComLine (argc, argv);

    Text codeFile = read_Text (codeFileName);

    Proc cpu = {};

    ProcCtor (&cpu);

    cpu.code     = codeFile.TextString;
    cpu.codeSize = codeFile.TextSize;

    checkFileSign (&cpu);

    ProcRunCode (&cpu);

    ProcDtor (&cpu);
    killText (&codeFile);
    free (codeFileName);
}