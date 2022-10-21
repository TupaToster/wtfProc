#include "asm_head.h"

bool Errors = 0;

int main (int argc, char* argv[]) {

    char* outFileName       =  NULL;
    char* fileName = handleComLine (argc, argv, &outFileName);
    assert (fileName != NULL);


    FILE* outFile = fopen (outFileName, "wb");
    assert (outFile  != NULL);

    Text codeFile = read_Text (fileName);

    size_t Ip = 0;
    Tag   tags[TAGS_SIZE]   = {};
    TagsCtor (tags);

    do {
        writeWtf (&codeFile, NULL, tags, &Ip);
        if (Errors) break;

        writeWtf (&codeFile, outFile, tags, &Ip);
        if (Errors) break;

    } while(false);

    killText (&codeFile);
    fclose (outFile);
    free (fileName);
    free (outFileName);
}
