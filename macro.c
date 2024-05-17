#include "macro.h"

bool expanding = false;

NAMETAB_el* NAMETAB = NULL;
DEFTAB_el* DEFTAB = NULL;
ARGTAB_el* ARGTAB = NULL;

int max_size_deftab = 0, curr_size_deftab = 0;
int max_size_nametab = 0, curr_size_nametab = 0;
int max_size_argtab = 0, curr_size_argtab = 0;

char buffer[256];
char* string = NULL;

void addNametabElem(NAMETAB_el newElem)
{
    if (curr_size_nametab == max_size_nametab)
    {
        max_size_nametab = (max_size_nametab == 0) ? 1 : curr_size_nametab * 2;
        NAMETAB = realloc(NAMETAB, max_size_nametab * sizeof(NAMETAB_el));
    }
    NAMETAB[curr_size_nametab++] = newElem;
}

void addDeftabElem(DEFTAB_el newElem)
{
    if (curr_size_deftab == max_size_deftab)
    {
        max_size_deftab = (max_size_deftab == 0) ? 1 : curr_size_deftab * 2;
        DEFTAB = realloc(DEFTAB, max_size_deftab * sizeof(DEFTAB_el));
    }
    DEFTAB[curr_size_deftab++] = newElem;
}

void addArgtabElem(ARGTAB_el newElem)
{
    if (curr_size_argtab == max_size_argtab)
    {
        max_size_argtab = (max_size_argtab == 0) ? 1 : curr_size_argtab * 2;
        ARGTAB = realloc(ARGTAB, max_size_argtab * sizeof(ARGTAB_el));
    }
    ARGTAB[curr_size_argtab++] = newElem;
}

int findInNametab(const char* _name)
{
    char* temp = NULL;
    if (_name[0] == '\t')
    {
        temp = (char*)malloc(strlen(_name));
        strcpy(temp, &_name[1]);
    }
    else
    {
        temp = (char*)malloc(strlen(_name) + 1);
        strcpy(temp, _name);
    }
    for (int i = 0; i < curr_size_nametab; i++)
    {
        if (strcmp(NAMETAB[i].name, temp) == 0)
        {
            return i;
        }
    }
    free(temp);
    return -1;
}

int findInArgtab(const char* _key)
{
    for (int i = 0; i < curr_size_argtab; i++)
    {
        if (strcmp(ARGTAB[i].key, _key) == 0)
            return i;
    }
    return -1;
}

void replaceSubstr(const char* _string, const char* old, const char* new)
{
    char* pos = strstr(_string, old);
    if (pos != NULL)
    {
        int len_old = strlen(old);
        int len_new = strlen(new);
        if (len_new != len_old)
        {
            memmove(pos + len_new, pos + len_old, strlen(pos + len_old) + 1);
        }
        memcpy(pos, new, len_new);
    }
}

void getLine(FILE* input)
{
    if (expanding)
    {
        for (int i = 0; i < curr_size_argtab; i++)
        {
            while (strstr(string, ARGTAB[i].key))
            {
                replaceSubstr(string, ARGTAB[i].key, ARGTAB[i].value);
            }
        }
    }
    else
    {
        string = fgets(buffer, sizeof(buffer), input);
    }
}

void macroProcessor(FILE* input, FILE* output)
{
    getLine(input);
    while (string != NULL)
    {
        actionLine(input, output);
        getLine(input);
    }
    free(DEFTAB);
    DEFTAB = NULL;
    free(NAMETAB);
    NAMETAB = NULL;
    free(ARGTAB);
    ARGTAB = NULL;
}

void actionLine(FILE* input, FILE* output)
{
    char* str_copy1 = (char*)malloc(strlen(string));
    strcpy(str_copy1, string);
    int index = findInNametab(strtok(str_copy1, " :,"));
    if (index != -1)
    {
        macroExpansion(index, input, output);
    }
    else if (!expanding && strstr(string, "macro"))
    {
        macroDefinition(input);
    }
    else
    {
        fprintf(output, "%s", string);
    }
}

void macroDefinition(FILE* input)
{
    char* str_copy = (char*)malloc(strlen(string) * sizeof(char));
    strcpy(str_copy, string);
    NAMETAB_el NAMETAB_elem = createNametabElem(strtok(str_copy, " :,\t"));
    addDeftabElem(createDeftabElem(string));
    int start_index_macro = curr_size_deftab - 1;
    int level = 1;
    while (level > 0)
    {
        getLine(input);
        addDeftabElem(createDeftabElem(string));
        if (strstr(string, "macro"))
        {
            level += 1;
        }
        else if (strstr(string, "endm"))
        {
            level -= 1;
        }
    }
    NAMETAB_elem.begin = start_index_macro;
    NAMETAB_elem.end = curr_size_deftab - 1;
    addNametabElem(NAMETAB_elem);
}

void macroExpansion(int index, FILE* input, FILE* output)
{
    expanding = true;
    int begin = NAMETAB[index].begin;
    int end = NAMETAB[index].end;
    char* str_copy = (char*)malloc(strlen(DEFTAB[begin].def) * sizeof(char));
    strcpy(str_copy, DEFTAB[begin].def);
    if (strstr(str_copy, "="))
    {
        char* token = (char*)malloc(2 * sizeof(char));
        char* ach;
        int id;
        char* token1 = strtok(str_copy, " :");
        token1 = strtok(NULL, " ");
        token1 = strtok(NULL, ",\n");
        while (token1 != NULL)
        {
            ach = strchr(token1, '=');
            id = ach - token1;
            token = realloc(token, (id + 1) * sizeof(char));
            memcpy(token, token1, id * sizeof(char));
            token[id] = '\0';
            if (findInArgtab(token) == -1)
            {
                addArgtabElem(createArgtabElem(token));
            }
            if (token1[strlen(token1) - 1] != '=')
            {
                id = findInArgtab(token);
                ARGTAB[id].value = (char*)malloc(strlen(ach) - 1);
                strcpy(ARGTAB[id].value, &ach[1]);
            }
            token1 = strtok(NULL, ",\n");
        }
        token1 = strtok(str_copy, " ,:\n");
        strcpy(str_copy, string);
        token1 = strtok(str_copy, " ,:\n");
        token1 = strtok(NULL, ",\n");
        char* temp = NULL;
        while (token1 != NULL)
        {
            if (token1[strlen(token1) - 1] != '=')
            {
                ach = strchr(token1, '=');
                id = ach - token1;
                token = realloc(token, (id + 1) * sizeof(char));
                memcpy(token, token1, id * sizeof(char));
                token[id] = '\0';
                temp = (char*)malloc((strlen(token) + 1) * sizeof(char));
                temp[0] = '&';
                strcpy(&temp[1], token);
                id = findInArgtab(temp);
                if (ARGTAB[id].value == NULL)
                {
                    ARGTAB[id].value = (char*)malloc(strlen(ach) - 1);
                    strcpy(ARGTAB[id].value, &ach[1]);
                }
                else
                {
                    replaceSubstr(ARGTAB[id].value, ARGTAB[id].value, &ach[1]);
                }
            }
            token1 = strtok(NULL, ",\n");
        }
        free(token);
        free(token1);
    }
    else
    {
        int id, count = 0;
        char* token = strtok(str_copy, " ,:\n");
        while (token != NULL)
        {
            if (token[0] == '&')
            {
                if (findInArgtab(token) == -1)
                {
                    addArgtabElem(createArgtabElem(token));
                }
                id = findInArgtab(token);
                count++;
            }
            token = strtok(NULL, " ,:\n");
        }
        strcpy(str_copy, string);
        token = strtok(str_copy, " ,:\n");
        token = strtok(NULL, " ,:\n");
        int j = id - count + 1;
        while (token != NULL && j <= id)
        {
            ARGTAB[j].value = (char*)malloc(strlen(token));
            strcpy(ARGTAB[j].value, token);
            token = strtok(NULL, " ,:\n");
            j++;
        }
        free(token);
    }
    for (int i = begin + 1; i < end; i++)
    {
        strcpy(string, DEFTAB[i].def);
        getLine(input);
        actionLine(input, output);
    }
    expanding = false;
}
