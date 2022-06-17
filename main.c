#include "library.h"
#include "cformat.h"

#include "operation.c"
#include "check.c"
#include "code.c"

#define AVAILABLE_SYMBOL(i) command[i] == ' ' || command[i] == TAB || command[i] == '{' || \
            command[i] == '}' || command[i] == ';' || command[i] == '(' || \
            command[i] == ')' || command[i] == TAB || command[i] == '\n'

void chack_new_line(fragment_code* code)
{
    char *s;
    char *back_s;
    int count; 
    while (code != NULL) {
        s = &(code->symbol[0]);
        while (*s != '\0') {
            if (*s == ';') {
                back_s = s - 1;
                count = 0;
                while (*back_s != '\n' && *back_s != NEW_LINE && *back_s != '(') {
                    if (*back_s == ';' || *back_s == '{' || *back_s == '}' || *back_s == ')') {
                        count++;
                        break;
                    }
                    back_s--;
                }
                if (count) {
                    int count_s  = 0;
                    back_s = s - 1;
                    while (*back_s != ';' && *back_s != '{' && *back_s != '}' && *back_s != ')') {
                        back_s--;
                        if (*back_s != ' ' || *back_s != TAB)
                            count_s++;
                    }
                    if (!check_parity(back_s + 1, code) && (count_s != 0)) {
                        past_symbol(code, back_s + 1, '\n');
                        s++;
                    }
                }
            }
            s++;
        }
        code = code->next_code;
    }
}

void clean_tab(fragment_code* code)
{
    char* s = code->symbol;
    while (1) {
        if (*s == '\n') {
            while (*s == '\n') // нужно ли это?
                if (!next_symbol(&code, &s))
                    return;
            while (*s == ' ' || *s == TAB) {
                delete_symbol(code, s);
                if (!(*s) && (!next_symbol(&code, &s)))
                    return;
            }
        } else
            if (!next_symbol(&code, &s))
                break;
    }
}

void check_op(fragment_code* code)
{
    char *s;
    char *back_s;
    int count; 
    while (code != NULL) {
        s = &(code->symbol[0]);
        while (*s != '\0') {
            if (*s == ';') {
                back_s = s - 1;
                count = 0;
                while (*back_s != '\n' && *back_s != NEW_LINE && *back_s != '(') {
                    if (*back_s == ';' || *back_s == '{' || *back_s == '}' || *back_s == ')') {
                        count++;
                        break;
                    }
                    back_s--;
                }
                if (count) {
                    int count_s  = 0;
                    back_s = s - 1;
                    while (*back_s != ';' && *back_s != '{' && *back_s != '}' && *back_s != ')') {
                        back_s--;
                        if (*back_s != ' ' || *back_s != TAB)
                            count_s++;
                    }
                    if (!check_parity(back_s + 1, code) && (count_s != 0)) {
                        past_symbol(code, back_s + 1, '\n');
                        s++;
                    }
                }
            }
            s++;
        }
        code = code->next_code;
    }
}

int chec_q(fragment_code* code, char* s)
{
    // char *start_symbol = get_start_symbol(code);
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
    if (!check_str(code, s))
        return 0;

    if (!previous_symbol(&code, &s))
        return 1;
/*    while (1) { 
        switch (*s) {
            case '/':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '*')
                    if (chec_q(code, s))
                        return 1;
                break;
            case '*':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '/') {
                    if (chec_q(code, s)) 
                        return 1;
                    else 
                        return 0;
                }
                if (!previous_symbol(&code, &s))
                    break;
        }

        if (!previous_symbol(&code, &s))
            break;
    }
*/
    return 1;
}

int check_init(fragment_code* code, char* s)
{
    char *start_symbol = get_start_symbol(code);
    int count = 0;
    if (*s == '{') {
        previous_symbol(&code, &s);
        if (s == start_symbol)
            return 0;
        while (*s == ' ') {
            previous_symbol(&code, &s);
            if (s == start_symbol)
                return 0;
        }
        if (*s == '=')
            return 0;
    } else {
        count++;
        while (count) {
            previous_symbol(&code, &s);

            if (*s == '{')
                count--;
            if (*s == '{')
                count++;
            
            if (s == start_symbol)
                return 0;
        }

        while (*s == ' ') {
            previous_symbol(&code, &s);
            if (s == start_symbol - 1)
                return 1;
        }
        if (*s == '=')
            return 0;
    }
    
    return 1;
}

void check_tab(fragment_code* code, char* s, int* tab)
{
    // if (*s == 123 && chec_q(code, s) && check_comment(code, s) && check_init(code, s))
    if (*s == 123 && chec_q(code, s) && check_comment(code, s)) {
        (*tab)++;
    }

    // if (*s == 125 && chec_q(code, s) && check_comment(code, s) && check_init(code, s))
    if (*s == 125 && chec_q(code, s) && check_comment(code, s)) {
        (*tab)--;
    }
}

void add_tab(fragment_code* code)
{
    char *s = code->symbol;
    int tab = 0, i;

        while (1) {
            check_tab(code, s, &tab);
            if (*s == '\n') {
                while (*s == '\n')
                    if (!next_symbol(&code, &s))
                        return;

                if (*s == '}' && chec_q(code, s)) { // ###
                    for (i = 0; i < tab - 1; i++) 
                        past_symbol(code, s, TAB);
                    while (*s == TAB)
                        if (!next_symbol(&code, &s))
                            return;
                } else {
                    for (i = 0; i < tab; i++) 
                        past_symbol(code, s, TAB);
                    
                    while (*s == TAB)
                        if (!next_symbol(&code, &s))
                            return;
                }              
            } else
                if (!next_symbol(&code, &s))
                    return;
        }
}

void add_tab_after_command(fragment_code** _code, char** _s)
{
    fragment_code* code = *_code;
    char* s = *_s;
    int brk = 0;
    if (chec_q(code, s)) {
        while (*s == ' ' || *s == TAB) 
            next_symbol(&code, &s);
    
        do {
            if (*s == '(')
                brk++;
            if (*s == ')')
                brk--;
            next_symbol(&code, &s);
        } while (brk != 0);
        while (*s != '\n') {
            if (*s == '{' || *s == ';')
                return;
            next_symbol(&code, &s);
        }
        next_symbol(&code, &s);
        past_symbol(code, s, TAB);
    }
}

void cicl(fragment_code* code)
{
    char command[COMMAND_L], *cur_command; // текущая команда
    char _while[] = "while", _for[] = "for", _if[] = "if";
    char* s = code->symbol;
    int j;

    for (j = 0; j < COMMAND_L; j++) // зануляется
        command[j] = '\0';

    while (1) {
        for (j = 0; j <= COMMAND_L - 2; j++) // сдвиг влево
            command[j] = command[j + 1];
        command[COMMAND_L - 1] = *s; // получение символов 

        if (AVAILABLE_SYMBOL(5)) {
            cur_command = command + 4;
            if (!scmp_command(cur_command, _while)) 
                add_tab_after_command(&code, &s);
        }

        if (AVAILABLE_SYMBOL(7)) {
            cur_command = command + 6;
            if (!scmp_command(cur_command, _for)) 
                add_tab_after_command(&code, &s);
        }

        if (AVAILABLE_SYMBOL(5)) {
            cur_command = command + 7;
            if (!scmp_command(cur_command, _if)) 
                add_tab_after_command(&code, &s);
        }
        if (!next_symbol(&code, &s))
            break;
    }
}

void write_code(fragment_code* code)
{
    FILE* in;
    in = fopen("myfile.c", "w");
    
    char* s;
    while (code != NULL) {
        s = code->symbol;
        while (*s) {
            fprintf(in, "%c", *s);
            s++;
        }
        code = code->next_code;
        // fprintf(in, "_NEXT_");
    }
    fclose(in);
}

void cformat(fragment_code* code)
{
    clean_tab(code);
    write_code(code);
    
    getchar();

    // char* s = &(code->symbol[4]);
    // previous_symbol(&code, &s);
    // printf("%c", *s);

    add_tab(code);
    cicl(code);
}

int main() {
    char name_file[] = "test.c";

    FILE* file;
    file = fopen(name_file, "r");

    fragment_code* code = malloc(sizeof(fragment_code));
    code->previous_code = NULL;
    
    assert(read_code(file, code) == RIGHT);
    printf("cformat:\n");
    cformat(code);
    // print_code(code);

    write_code(code);

    return 0;
}
