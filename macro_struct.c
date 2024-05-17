#include "macro_struct.h"

NAMETAB_el createNametabElem(const char* _name)
{
    NAMETAB_el newEl;
    newEl.name = strdup(_name);
    newEl.begin = 0;
    newEl.end = 0;
    return newEl;
}

ARGTAB_el createArgtabElem(const char* _key)
{
    ARGTAB_el newEl;
    newEl.key = strdup(_key);
    newEl.value = NULL;
    return newEl;
}

DEFTAB_el createDeftabElem(const char* _def)
{
    DEFTAB_el newEl;
    newEl.def = strdup(_def);
    return newEl;
}
