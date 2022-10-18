#include "deasm_head.h"

char* outStr  = NULL;
int   tags[512] = {0};

int main (int argc, char* argv[]) {

    char* codeFileName = handleComLine (argc, argv);

    Text codeFile = read_Text (codeFileName);

    memset (tags, -1, 512 * sizeof (int));

    Proc cpu = {};
    ProcCtor (&cpu);

    cpu.code     = codeFile.TextString;
    cpu.codeSize = codeFile.TextSize;

    FILE* outFile = NULL;

    checkFileSign (&cpu);

    ProcRunCode (&cpu, outFile);

    codeFileName = (char*) realloc (codeFileName, strlen(codeFileName) + strlen("_deasm.code") + 1);
    assert (codeFileName != NULL);

    codeFileName = strcat (codeFileName, "_deasm.code");
    assert (codeFileName != NULL);

    outFile = fopen (codeFileName , "w");

    flog (codeFileName);

    cpu.ip = 4;

    ProcRunCode (&cpu, outFile);

    ProcDtor (&cpu);
    killText (&codeFile);
    free (codeFileName);
}