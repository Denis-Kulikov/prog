#include "library.h"
#include "cformat.h"

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
    char _while[] = "while", _for[] = "for", _if[] = "if", _switch[] = "switch", _do[] = "do";
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
            if (!scmp_command(cur_command, _while) && check_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 5))
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
                check_do_while(&code, &s);
            }
        }

        if (AVAILABLE_SYMBOL(6, command)) {
            cur_command = command + 7;
            if (!scmp_command(cur_command, _for) && check_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 3)) 
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
            }
        }

        if (AVAILABLE_SYMBOL(7, command)) {
            cur_command = command + 8;
            if (!scmp_command(cur_command, _if) && check_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 2))
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
            }
        }

        if (AVAILABLE_SYMBOL(3, command)) {
            cur_command = command + 4;
            if (!scmp_command(cur_command, _switch) && check_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 6))
                    next_symbol(&code, &s);
                add_tab_after_case(code, s);
            }
        }

        if ((AVAILABLE_SYMBOL(7, command)) && available_end(code, s)) {
            cur_command = command + 8;
            if (!scmp_command(cur_command, _do) && check_q(code, s) && check_comment(code, s)) {
                if (check_shift_cycle(code, s, 2))
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
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
            if (!scmp_command(cur, _else) && check_q(code, s) && check_comment(code, s)) {
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

/*
void tab_after_figur(code)
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
*/


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

void check_tab_brk(fragment_code* code)
{
    char *s = code->symbol, *save = s;
    fragment_code* save_code = code;
    do {
        if (*s == '=') {// проверка инициации массива
            if (check_init(code, s)) {
                while (1) {
                    if (*s == ';' && check_q(code, s) && check_comment(code, s))
                        break;
                    if (!next_symbol(&code, &s))
                        return;
                }
            }
        }
        save = s;
        save_code = code;
        if ((*s == 123 || *s == 125) && check_q(code, s) && check_comment(code, s)) {
            while (previous_symbol(&code, &s)) {
                if (*s != ' ' && *s != TAB) {
                    if (*s != '\n') {
                        past_symbol(save_code, save, '\n');
                        next_symbol(&save_code, &save);
                    }
                    break;
                }
            }

            s = save;
            code = save_code;
            while (next_symbol(&code, &s)) {
                if (*s != ' ' && *s != TAB) {
                    if (*s != '\n') {
                        if (!next_symbol(&save_code, &save))
                            return;
                        past_symbol(save_code, save, '\n');
                    }
                    break;
                }
            }
            s = save;
            code = save_code;
        }
    } while (next_symbol(&code, &s));
}

void cformat(fragment_code* code)
{
    // check_op(code);
    // for (int q = 0; q < 5; q++) printf("\n");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("  сheck_op  ");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("\n");
    // // print_code(code);

    // getchar();
    // clean_tab(code);
    // for (int q = 0; q < 5; q++) printf("\n");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("  clean_tab  ");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("\n");
    // // print_code(code);

    // getchar();
    // // for (int q = 0; q < 5; q++) printf("#");
    // check_tab_brk(code);
    // cycle(code);
    // for (int q = 0; q < 5; q++) printf("\n");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("  cycle  ");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("\n");
    // // print_code(code);
    
    // getchar();
    // add_tab(code);
    // for (int q = 0; q < 5; q++) printf("\n");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("  add_tab  ");
    // for (int q = 0; q < 5; q++) printf("#");
    // printf("\n");


    // add_shift_cycle(code);

    check_op(code);
    // clean_tab(code);
    // check_tab_brk(code);
    // cycle(code);
    // add_tab(code);
}