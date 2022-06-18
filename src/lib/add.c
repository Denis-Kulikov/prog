#include "library.h"

void add_tab(fragment_code* code)
{
    char *s = code->symbol, *helper;
    int tab = 0, i, save, change, count;

    while (1) {
        count = 0;
        save = tab;
        check_tab(code, s, &tab);
        change = tab - save;

        if (change == 1) { // {
            if (tab == 1) { // первый
                helper = s;
                while (*helper != ')' && previous_symbol(&code, &helper)) 
                    if (*helper == ' ' || *helper == TAB || *helper == '\n')
                        delete_symbol(code, helper);
                
                if (!next_symbol(&code, &helper))
                    return;
                past_symbol(code, helper, '\n');
                if (!next_symbol(&code, &helper) || !next_symbol(&code, &helper))
                    return;
                s = helper;
            } else { // не первый и перед циклом
                helper = s;
                while (1) {
                    if (!previous_symbol(&code, &helper))
                        break;
                    if (*helper == ' ' || *helper == TAB || *helper == '\n') {
                        if (previous_symbol(&code, &helper))
                            if (*helper == ')')
                                count++;

                        break;
                    }
                }

                if (count) { // не первый и перед циклом
                    while (1) {
                        previous_symbol(&code, &s); // назад пока не функция
                        if (*s == ')')
                            break;
                        else
                            delete_symbol(code, s); // удаляем отступы
                    }
                    next_symbol(&code, &s);
                    past_symbol(code, s, ' '); // пробел после цикла
                    if (!next_symbol(&code, &s) || !next_symbol(&code, &s))
                        return;
                }
            }
        }

        if (change == -1 || (change == 1 && tab != 1)) {
            helper = s;
            if (!next_symbol(&code, &helper))
                return;
            while (*helper != '\n') {
                if (*helper != ' ' || *helper != TAB) {
                    if (!count)
                        past_symbol(code, helper, '\n');
                    break;
                }
                if (!next_symbol(&code, &helper))
                    break;
            }
        }

        if (*s == '=') {// проверка инициации массива
            if (check_init(code, s)) {
                // printf("%d\n", str);
                while (1) {
                    // printf("#");
                    if (*s == ';' && chec_q(code, s) && check_comment(code, s))
                        break;
                    if (!next_symbol(&code, &s))
                        return;
                }
            }
        }

        if (*s == '\n') {
            while (*s == '\n')
                if (!next_symbol(&code, &s))
                    return;

            if (*s == '}' && chec_q(code, s) && check_comment(code, s)) { // ###
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

    if (!next_symbol(&code, &s) || !chec_q(code, s) || !check_comment(code, s))
        return;

    while (*s == ' ' || *s == TAB) 
        if (!next_symbol(&code, &s))
            return;

    do {
        if (*s == '(' && chec_q(code, s))
            brk++;
        if (*s == ')' && chec_q(code, s))
            brk--;
        if (!next_symbol(&code, &s))
            return;
    } while (brk != 0);

    while (1) {
        if (*s == '\n')
            break;

        if (*s == '/') {
            if (!next_symbol(&code, &s) || (*s != '*' && *s != '/')) 
                return;
            else {
                if (*s == '*') {
                    while (1) {
                        if (*s == '\n')
                            return;
                        if (*s == '*') {
                            previous_symbol(&code, &s);
                            if (*s == '/')
                                break;
                            next_symbol(&code, &s);
                        }
                    }
                } else 
                    while (*s != '\n') 
                        next_symbol(&code, &s);
            }
        }

        if (*s == '{' || *s == ';')
            return;
        if (*s != ' ' && *s != TAB)
            break;
        if (!next_symbol(&code, &s))
            return;
    }
    if (*s != '\n')
        past_symbol(code, s, '\n');
    if (!next_symbol(&code, &s))
        return;

    past_symbol(code, s, TAB);
}

void add_tab_after_case(fragment_code* code, char* s)
{
    while (*s != '{')
        if (!next_symbol(&code, &s))
            return;
    if (!next_symbol(&code, &s))
        return;
    char command[COMMAND_L]; // текущая команда
    char _case[] = "case", _default[] = "default";
    int j, tab = 1;

    for (j = 0; j < COMMAND_L; j++) // зануляется
        command[j] = '\0';

    while (tab) {
        if ((*s == '}' || *s == '{')) {
            check_tab(code, s, &tab);
            if (!tab) {
                previous_symbol(&code, &s);
                delete_symbol(code, s);
                return;
            }
        }

        if (*s == '\n') {
            if (!next_symbol(&code, &s))
                return;
            past_symbol(code, s, TAB);
        }

        for (j = 0; j <= COMMAND_L - 2; j++) // сдвиг влево
            command[j] = command[j + 1];
        command[COMMAND_L - 1] = *s; // получение символов 

        if (chec_q(code, s) && check_comment(code, s)) {
            if (!scmp_command(command + 6, _case)) {
                for (int i = 0; i < 3; i++)
                    previous_symbol(&code, &s);
                while (*s == ' ' || *s == '\n' || *s == TAB) {
                    delete_symbol(code, s);
                    previous_symbol(&code, &s);
                }
                previous_symbol(&code, &s);
                delete_symbol(code, s); // удаление табуляции

                while (1) { // пропуск до двоеточия
                    next_symbol(&code, &s);
                    if (*s == ':' && chec_q(code, s) && check_comment(code, s)) 
                        break;
                }

                if (!next_symbol(&code, &s))
                    return;

                while (*s == ' ' || *s == TAB) // проверка после :
                    if (!next_symbol(&code, &s))
                        return;
                if (*s != '\n')
                    past_symbol(code, s, '\n');
                continue;                
            }

            if (!scmp_command(command + 3, _default)) {
                for (int i = 0; i <= 7; i++)
                    previous_symbol(&code, &s);
                while (*s == ' ' || *s == '\n' || *s == TAB) {
                    delete_symbol(code, s);
                    previous_symbol(&code, &s);
                }
                past_symbol(code, s, TAB);
                past_symbol(code, s, '\n');
                for (int i = 0; i < 8; i++)
                    next_symbol(&code, &s);
            }
        }

        if (!next_symbol(&code, &s))
            return;
    }
}