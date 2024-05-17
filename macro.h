#ifndef MACRO_H_
#define MACRO_H_

#include <stdbool.h>
#include "macro_struct.h"

void addNametabElem(NAMETAB_el newElem);
void addDeftabElem(DEFTAB_el newElem);
void addArgtabElem(ARGTAB_el newElem);

int findInNametab(const char* _name);
int findInArgtab(const char* _key);
void replaceSubstr(const char* _string, const char* old, const char* new);

void getLine(FILE* input);
void macroProcessor(FILE* input, FILE* output);
void actionLine(FILE* input, FILE* output);
void macroDefinition(FILE* input);
void macroExpansion(int index, FILE* input, FILE* output);

#endif
