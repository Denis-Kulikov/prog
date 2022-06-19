#include "library.h"
#include "cformat.h"

void add_tab(fragment_code* code)
{
    char *s = code->symbol;
    int tab = 0, i;

    while (1) {
        check_tab(code, s, &tab);

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

        if (*s == '\n' && check_q(code, s) && check_comment(code, s)) {
            while (*s == '\n')
                if (!next_symbol(&code, &s))
                    return;

            if (*s == '}') {
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
    char* s = *_s, *save;
    int brk = 0;

    if (*s != 'o') {
        if (!next_symbol(&code, &s) || !check_q(code, s) || !check_comment(code, s))
            return;

        while (*s == ' ' || *s == TAB) 
            if (!next_symbol(&code, &s))
                return;

        
        do {
            if (*s == '(' && check_q(code, s))
                brk++;
            if (*s == ')' && check_q(code, s))
                brk--;
            if (!next_symbol(&code, &s))
                return;
        } while (brk != 0);
    } else
        if (!next_symbol(&code, &s) || !check_q(code, s) || !check_comment(code, s))
            return;

    save = s;
    while (1) { // удаление отспупов после цикла, перед фигурными скобками
        if (*s != ' ' && *s != TAB && *s != '\n') {
            if (*s == '{') {
                // printf("\n======");
                s = save;
                while (*s != '{')
                    delete_symbol(code, s);
                past_symbol(code, s, ' ');
            }
            break;
        }

        if (!next_symbol(&code, &s))
            break;
    }
    
    s = save;
    while (1) {
        if (*s == '\n')
            break;

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

        if (check_q(code, s) && check_comment(code, s)) {
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
                    if (*s == ':' && check_q(code, s) && check_comment(code, s)) 
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
                for (int i = 0; i < 6; i++)
                    previous_symbol(&code, &s);
                while (*s == ' ' || *s == '\n' || *s == TAB) {
                    delete_symbol(code, s);
                    previous_symbol(&code, &s);
                }
                previous_symbol(&code, &s);
                delete_symbol(code, s); // удаление табуляции

                while (1) { // пропуск до двоеточия
                    next_symbol(&code, &s);
                    if (*s == ':' && check_q(code, s) && check_comment(code, s)) 
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
        }

        if (!next_symbol(&code, &s))
            return;
    }
}
