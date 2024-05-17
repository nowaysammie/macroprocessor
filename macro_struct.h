#ifndef MACRO_STRUCT_H_
#define MACRO_STRUCT_H_

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char* name;
    int begin;
    int end;
} NAMETAB_el;

typedef struct
{
    char* key;
    char* value;
} ARGTAB_el;

typedef struct
{
    char* def;
} DEFTAB_el;

NAMETAB_el createNametabElem(const char* _name);
ARGTAB_el createArgtabElem(const char* _key);
DEFTAB_el createDeftabElem(const char* _def);

#endif
