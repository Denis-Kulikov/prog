#include "library.h"
#include "cformat.h"

void check_tab(fragment_code* code, char* s, int* tab)
{
    if (*s == 123 && check_q(code, s) && check_comment(code, s))
        (*tab)++;

    if (*s == 125 && check_q(code, s) && check_comment(code, s))
        (*tab)--;
}

int check_q(fragment_code* code, char* s)
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
            if (check_q(code, s)) {
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
                    if (check_q(code, s))
                        comment_count--;
                break;
            case '*':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '/') 
                    if (check_q(code, s)) 
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
        if (*s == '(' && check_q(code, s) && check_comment(code, s))
            count++;
        if (*s == ')' && check_q(code, s) && check_comment(code, s))
            count--;
    }

    return (count + 1) % 2; // 1 если внутри
}

int check_init(fragment_code* code, char* s)
{
    if (!check_q(code, s) || !check_comment(code, s))
        return 0;

    char* save = s;
    fragment_code* save_code = code;

    if (!previous_symbol(&code, &s))
        return 0;
    if (*s == '=' || *s == '!')
        return 0;

    s = save;
    code = save_code;
    if (!next_symbol(&code, &s))
        return 0;
    if (*s == '=')
        return 0;
    
    s = save;
    code = save_code;
    while (next_symbol(&code, &s)) {
        if (*s != ' ' && *s != TAB) {
            if (*s != '{')
                return 0;
            break;
        }
    }

    return 1;
}

void check_op(fragment_code* code)
{
    cycle(code);
    char *s = code->symbol;
    fragment_code* save_code = code;
    char *save;
    while (1) {
        if (*s == '=') {// проверка инициации массива
            if (check_init(code, s)) {
                while (1) {
                    if (*s == ';' && check_q(code, s) && check_comment(code, s)) {
                        if (!next_symbol(&code, &s))
                            return;
                        break;
                    }
                    if (!next_symbol(&code, &s))
                        return;
                }
            }
        }
        
        if (*s == ';' && check_q(code, s) && check_comment(code, s) && check_parity(code, s)) {
            save = s;
            save_code = code;
            if (!previous_symbol(&save_code, &save)) {
                if (!next_symbol(&code, &s))
                    return;
                continue;
            }

            while (1) {
                if (*save == '\n')
                    break;
                if ((*save == ';' || *save == '{' || *save == '}') && check_parity(code, s) &&
                    check_q(save_code, save) && check_comment(save_code, save)) {
                    if (!next_symbol(&save_code, &save))
                        break;

                    past_symbol(save_code, save, '\n');
                    next_symbol(&code, &s);
                    
                    break;
                }
                if (!previous_symbol(&save_code, &save))
                    break;
            }

            // save = s;
            // save_code = code;
            // if (next_symbol(&save_code, &save)) {
            //     while (*save == ' ' || *save == TAB)
            //         if (!next_symbol(&save_code, &save))
            //             break;
            //     if (*save != '\n')
            //         past_symbol(save_code, save, '\n');
            // }
        }
        
        if (!next_symbol(&code, &s))
            break;
    }
}

void check_do_while(fragment_code** code, char** s)
{
    char* save = *s;
    fragment_code* save_code = *code;
    int brk = 0;
    while (*save != ' ' && *save != TAB && *save != '\n')
        if (!previous_symbol(&save_code, &save))
            return;
    while (*save == ' ' || *save == TAB || *save == '\n')
        if (!previous_symbol(&save_code, &save))
            return;
    do {
        if (*save == '{' && check_q(save_code, save))
            brk++;
        if (*save == '}' && check_q(save_code, save))
            brk--;
        if (!previous_symbol(&save_code, &save))
            return;
    } while (brk != 0);

    while (*save == ' ' || *save == TAB || *save == '\n')
        if (!previous_symbol(&save_code, &save))
            return;
    
    printf("\n=> ");
    printf("%c", **s);
    if (*save == 'o' && previous_symbol(&save_code, &save))
        if (*save == 'd') {
            printf("111");
            while (**s != ' ' && **s != TAB && **s != '\n') {
                printf("%c", **s);
                if (!previous_symbol(code, s))
                    return;
            }
            while (**s == ' ' || **s == TAB || **s == '\n') {
                delete_symbol(*code, *s);
                if (!previous_symbol(code, s))
                    return;      
            }
            next_symbol(code, s);
            past_symbol(*code, *s, ' ');
            for (int i = 0; i < 10; i++)
                next_symbol(code, s);
        }
}

int available_end(fragment_code* code, char* s)
{
    if (!next_symbol(&code, &s)) 
        return 0;
    else if (*s == ' ' || *s == TAB || *s == '{' ||  *s == '}' ||
        *s == ';' || *s == '(' || *s == ')' || *s == TAB || *s == '\n')
        return 1;
    
    return 0;
}

int check_shift_cycle(fragment_code* code, char* s, int i)
{
    for (int j = 0; j < i; j++)
        previous_symbol(&code, &s);

    if (*s == '\n')
        return 0;

    while (*s == ' ' || *s == TAB) {
        if (!previous_symbol(&code, &s))
            break;
        if (*s == '\n')
            return 0;
    }
    next_symbol(&code, &s);
    past_symbol(code, s, '\n');

    return 1; // если перенесли на новую строку
}