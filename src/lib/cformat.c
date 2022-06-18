#include "library.h"

#define AVAILABLE_SYMBOL(i, command) command[i] == ' ' || command[i] == TAB || command[i] == '{' || \
            command[i] == '}' || command[i] == ';' || command[i] == '(' || \
            command[i] == ')' || command[i] == TAB || command[i] == '\n'


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

void cycle(fragment_code* code)
{
    char command[COMMAND_L], *cur_command; // текущая команда
    char _while[] = "while", _for[] = "for", _if[] = "if", _switch[] = "switch";
    char* s = code->symbol;
    int j, i = 1;

    for (j = 0; j < COMMAND_L; j++) // зануляется
        command[j] = '\0';

    while (1) {
        if (*s == '\n')
            i++;
        for (j = 0; j <= COMMAND_L - 2; j++) // сдвиг влево
            command[j] = command[j + 1];
        command[COMMAND_L - 1] = *s; // получение символов 

        if (AVAILABLE_SYMBOL(4, command)) {
            cur_command = command + 5;
            if (!scmp_command(cur_command, _while) && chec_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 5))
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
            }
        }

        if (AVAILABLE_SYMBOL(6, command)) {
            cur_command = command + 7;
            if (!scmp_command(cur_command, _for) && chec_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 3)) 
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
            }
        }

        if (AVAILABLE_SYMBOL(7, command)) {
            cur_command = command + 8;
            if (!scmp_command(cur_command, _if) && chec_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 2))
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
            }
        }

        if (AVAILABLE_SYMBOL(3, command)) {
            cur_command = command + 4;
            if (!scmp_command(cur_command, _switch) && chec_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 6))
                    next_symbol(&code, &s);
                add_tab_after_case(code, s);
            }
        }

        if (!next_symbol(&code, &s))
            break;
    }
}

void clean_else(fragment_code* code)
{
    char save[6], *cur; // текущая команда
    char _else[] = "else";
    char* s = code->symbol;
    int j;

    for (j = 0; j < 6; j++) // зануляется
        save[j] = '\0';

    while (1) {
        for (j = 0; j <= 6 - 2; j++) // сдвиг влево
            save[j] = save[j + 1];
        save[6 - 1] = *s; // получение символов 

        if (AVAILABLE_SYMBOL(0, save)) {
            cur = save + 1;
            if (!scmp_command(cur, _else) && chec_q(code, s) && check_comment(code, s)) {
                while (*s != TAB) // пропускаем else
                    previous_symbol(&code, &s);

                while (*s == TAB || *s == ' ' || *s == '\n') { // пока пропуски
                    delete_symbol(code, s);
                    if (!previous_symbol(&code, &s)) {
                        next_symbol(&code, &s);
                        next_symbol(&code, &s);
                        break;
                    }   
                }

                next_symbol(&code, &s);
                for (int i = 0; i < 6; i++)
                    next_symbol(&code, &s);
            }
        }
        if (!next_symbol(&code, &s))
            break;
    }
}

void write_code(fragment_code* code, FILE* file)
{
    char* s;
    while (code != NULL) {
        s = code->symbol;
        while (*s) {
            fprintf(file, "%c", *s);
            s++;
        }
        code = code->next_code;
    }
    fclose(file);
}

void cformat(fragment_code* code)
{
    check_op(code);
    printf("\n#check_op\n");
    print_code(code);
    getchar();
    clean_tab(code);
    printf("\n#clean_tab\n");
    print_code(code);
    getchar();
    cycle(code);
    printf("\n#cycle\n");
    print_code(code);
    getchar();
    add_tab(code);
}