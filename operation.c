int previous_symbol(fragment_code** code, char** s)
{
    (*s)--;
    if ((*s) == (*code)->symbol - 1) {
        (*code) = (*code)->previous_code; 
        if (*code == NULL)
            return 0;
        (*s) = (*code)->symbol;
        while (**s)
            (*s)++;
        (*s)--;
    }

    return 1;
}

int next_symbol(fragment_code** code, char** s)
{
    (*s)++;
    
    if (!**s) {
        *code = (*code)->next_code; 
        if (*code == NULL)
            return 0;
        (*s) = (*code)->symbol;
    }

    return 1;
}

int scmp_command(char* str1, char* str2)
{
    while (*str2) {
        if (*str1 != *str2)
            return 1;
        str1++;    
        str2++;    
    }
    return 0;
}

void delete_symbol(fragment_code* code, char* pos)
{
    char *cur = pos, *next = cur + 1;

    while (*cur) {
        *cur = *next;
        cur++;
        next++;
    }
}

void past_symbol(fragment_code* code, char* pos, char s)
{
    char *cur = pos, *next;
    int i = 0;

    while (*cur) {
        cur++;
        i++;
    }
    next = cur + 1;

    for (; i >= 0; i--) {
        *next = *cur;
        cur--;
        next--;
    }

    *pos = s;
}