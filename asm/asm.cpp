#include "asm_head.h"

int main (int argc, char* argv[]) {

    char* fileName    = NULL;
    char* outFileName = NULL;
    bool  aFlag       = 0;

    fileName = handleComLine (argc, argv, &aFlag, &outFileName);

    FILE* outFile = fopen (outFileName, "wb");

    assert (outFile  != NULL);
    assert (fileName != NULL);

    Text codeFile = read_Text (fileName);

    writeWtf (&codeFile, outFile);

    killText (&codeFile);
    fclose (outFile);
    free (fileName);
    free (outFileName);
}