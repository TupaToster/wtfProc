#include "asm_head.h"

size_t ip = 0;

bool errors = 0;

int main (int argc, char* argv[]) {

    char* fileName    =  NULL;
    char* outFileName =  NULL;
    bool  aFlag       =  0;
    int   tags[TAGS_SIZE]   = {0};

    for (int i = 0; i < TAGS_SIZE; i++) tags[i] = -1;

    fileName = handleComLine (argc, argv, &aFlag, &outFileName);
    assert (fileName != NULL);


    FILE* outFile = fopen (outFileName, "wb");
    assert (outFile  != NULL);

    Text codeFile = read_Text (fileName);

    writeWtf (&codeFile, NULL, tags);

    if (errors == 1) return -1;

    ip = 0;

    writeWtf (&codeFile, outFile, tags);

    killText (&codeFile);
    fclose (outFile);
    free (fileName);
    free (outFileName);
}