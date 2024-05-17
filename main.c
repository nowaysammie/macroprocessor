#include "macro.h"

int main()
{
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    if (!input || !output)
        return -1;
    macroProcessor(input, output);
    fclose(output);
    fclose(input);
    return 0;
}
