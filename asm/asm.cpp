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

    char* outStr = NULL;

    do {
        writeWtf (&codeFile, outStr, tags, &Ip);
        if (Errors) break;

        outStr = (char*) calloc (Ip, sizeof (char));
        assert (outStr != NULL);

        Ip = 0;

        writeWtf (&codeFile, outStr, tags, &Ip);
        if (Errors) break;

        fwrite (outStr, Ip, sizeof (char), outFile);
    } while(false);

    killText (&codeFile);
    fclose (outFile);
    free (outStr);
    free (fileName);
    free (outFileName);
}
