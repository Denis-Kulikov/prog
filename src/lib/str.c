#include <stddef.h>

int sspn(char* str)
{
    char error[] = {'\\', '/', ':', '*', '?', '<', '>', '|', '\0'}, *e_pos;

    while (*str) {
        e_pos = error;
        while (*e_pos) {
            if (*str == *e_pos)
                return -1;
            e_pos++;
        }
        str++;
    }

    return 0;
}

char* scpy(char** str, char** str_copy)
{    
    char* save = *str_copy;
    
    while (**str) {
        **str_copy = **str; 
        (*str_copy)++;
        (*str)++;
    }

    return save;
}

int slen(char* str)
{
    if (str == NULL)
        return -1;

    int length = 0;
    for (;*str; str++)
        length++;
        
    return length;
}

int scmp(char* str1, char* str2)
{
    while (*str1 || *str2) {
        if ((*str1 ^ *str2) || (*str1 != *str2))
            return 1;
        str1++;    
        str2++;    
    }
    return 0;
}