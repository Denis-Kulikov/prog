#include "library.h"

void check_tab(fragment_code* code, char* s, int* tab)
{
    if (*s == 123 && chec_q(code, s) && check_comment(code, s))
        (*tab)++;

    if (*s == 125 && chec_q(code, s) && check_comment(code, s))
        (*tab)--;
}

int chec_q(fragment_code* code, char* s)
{
    int count = 0;

    if (*(s + 1) == '\'' && *(s - 1) == '\'')
        return 0;

    if (!previous_symbol(&code, &s))
        return 1;

    while (1) { 
        if (*s == '\"') {
            if (!previous_symbol(&code, &s)) {
                count++;
                break;
            }
            if (*s != '\\')
                count++;
        }
        
        if (!previous_symbol(&code, &s))
            break;
    }

    return (count + 1) % 2; // 1 если НЕ внутри
}

int check_str(fragment_code* code, char* s) 
{
    while (*s != '\n') {
        if (*s == '/')
            if (chec_q(code, s)) {
                if (!previous_symbol(&code, &s))
                    return 1;

                if (*s == '/')
                    return 0;
            }
        
        if (!previous_symbol(&code, &s))
            break;
    }
    return 1;
}

int check_comment(fragment_code* code, char* s)
{
    int comment_count = 0;
    if (!check_str(code, s))
        return 0;

    if (!previous_symbol(&code, &s))
        return 1;

    while (1) { 
        switch (*s) {
            case '/':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '*')
                    if (chec_q(code, s))
                        comment_count--;
                break;
            case '*':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '/') 
                    if (chec_q(code, s)) 
                        comment_count++;
                break;
        }

        if (!previous_symbol(&code, &s))
            break;
    }

    return (comment_count + 1) % 2;
}

int check_parity(fragment_code* code, char* s)
{
    int count = 0;

    while (previous_symbol(&code, &s)) { 
        if (*s == '(' && chec_q(code, s) && check_comment(code, s))
            count++;
        if (*s == ')' && chec_q(code, s) && check_comment(code, s))
            count--;
    }

    return (count + 1) % 2; // 1 если внутри
}

int check_init(fragment_code* code, char* s)
{
    char* helper = s;

    if (!previous_symbol(&code, &helper))
        return 0;
    if (*helper == '=' || *helper == '!')
        return 0;

    helper = s;
    if (!next_symbol(&code, &helper))
        return 0;
    if (*helper == '=')
        return 0;
    
    if (!chec_q(code, s) || !check_comment(code, s))
        return 0;

    return 1;
}

void check_op(fragment_code* code)
{
    char *s = code->symbol;
    fragment_code* back_c = code;
    char *back_s;
    while (1) {
        if (*s == '=') {// проверка инициации массива
            if (check_init(code, s)) {
                while (1) {
                    if (*s == ';' && chec_q(code, s) && check_comment(code, s)) {
                        if (!next_symbol(&code, &s))
                            return;
                        break;
                    }
                    if (!next_symbol(&code, &s))
                        return;
                }
            }
        }
        
        if (*s == ';' && chec_q(code, s) && check_comment(code, s)) {
            back_s = s;
            if (!previous_symbol(&back_c, &back_s) || !check_parity(code, s)) {
                if (!next_symbol(&code, &s))
                    return;
                continue;
            }

            while (1) {
                if (*back_s == '\n')
                    break;
                if ((*back_s == ';' || *back_s == '{' || *back_s == '}') &&
                    chec_q(back_c, back_s) && check_comment(back_c, back_s)) {
                    next_symbol(&back_c, &back_s);
                    past_symbol(back_c, back_s, '\n');
                    next_symbol(&code, &s);
                    break;
                }
                if (!previous_symbol(&back_c, &back_s))
                    break;
            }

        }
        
        if (!next_symbol(&code, &s))
            break;
    }
}