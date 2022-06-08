#include "library.h"
#include "cformat.h"

#include "check.c"
#include "code.c"

#define CHECK_END_SYMBOL(s, code) \
if (!*s) { \
    code = code->next_code; \
    if (code == NULL) \
        return; \
    s = code->symbol; \
}

#define CHECK_BEGIN_SYMBOL(s, code) \
if (s == &(code->symbol[0]) - 1) { \
    code = code->previous_code; \
    s = code->symbol; \
    for (; *s != '\0'; s++); \
    s--; \
}

#define NEXT_SYMBOL(s, code) \
s++; \
CHECK_END_SYMBOL(s, code);

#define PREVIOUS_SYMBOL(s, code) \
s--; \
CHECK_BEGIN_SYMBOL(s, code);

#define AVAILABLE_SYMBOL(i) command[i] == ' ' || command[i] == TAB || command[i] == '{' || \
            command[i] == '}' || command[i] == ';' || command[i] == '(' || \
            command[i] == ')' || command[i] == TAB || command[i] == '\n'

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

void delete_symbol(fragment_code* code, char* pos)
{
    char *cur = pos, *next = cur + 1;

    while (*cur) {
        *cur = *next;
        cur++;
        next++;
    }
}

void delete_13(fragment_code* code)
{
    char* s;
    int i = 0;
    while (code != NULL) {
        s = &(code->symbol[0]);
        while (*s != '\0') {
            while (*s == 13) {
                delete_symbol(code, s);
                printf("%d ", i);
            }
            i++;
            s++;
        }
        code = code->next_code;
    } 
}

void delet_tab(fragment_code** code, char** s) {
    fragment_code* cur_code = *code;
    char* cur_s = *s;
    if (*cur_s == '\n') { 
        // past_symbol(cur_code, cur_s, '*');
        NEXT_SYMBOL(cur_s, cur_code);   
        while (*cur_s == '\n') { // пока переходы
            NEXT_SYMBOL(cur_s, cur_code);   
        }

        while (*cur_s == ' ' && cur_code != NULL) { // если пробел
            delete_symbol(cur_code, cur_s); // удаляем его
            CHECK_END_SYMBOL(cur_s, cur_code);
        }
    }
}

void clean_tab(fragment_code* code)
{
    char* s;
    while (code != NULL) {
        s = code->symbol;
        while (*s) {
            delet_tab(&code, &s);
            // if (*s == '\n') { 
            //     past_symbol(code, s, '*');
            //     NEXT_SYMBOL(s, code);   
            //     while (*s == '\n') { // пока переходы
            //         NEXT_SYMBOL(s, code);   
            //     }

            //     while (*s == ' ' && code != NULL) { // если пробел
            //         delete_symbol(code, s); // удаляем его
            //         CHECK_END_SYMBOL(s, code);
            //     }
            // }

            NEXT_SYMBOL(s, code);
        }
        code = code->next_code;
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
    char *start_symbol = get_start_symbol(code);
    int count = 0;

    if (*(s + 1) == '\'' && *(s - 1) == '\'')
        return 0;

    PREVIOUS_SYMBOL(s, code);
    while (s != start_symbol) { 
        if (*s == '\"') {
            PREVIOUS_SYMBOL(s, code);
            if (s == start_symbol)
                break;
            if (*s != '\\')
                count++;
        }
        
        PREVIOUS_SYMBOL(s, code);
        if (s == start_symbol)
            break;
    }

    return (count + 1) % 2;
}

int check_str(fragment_code* code, char* s) 
{
    char *start_symbol = get_start_symbol(code); 
    while (*s != '\n') {
        if (*s == '/') {
            if (chec_q(code, s)) {
                PREVIOUS_SYMBOL(s, code);
                if (s == start_symbol - 1)
                    break;
                if (*s == '/')
                    return 0;
            }
        }
        PREVIOUS_SYMBOL(s, code);
        if (s == start_symbol - 1)
            break;
    }
    return 1;
}

int check_comment(fragment_code* code, char* s)
{
    char *start_symbol = get_start_symbol(code);

    if (check_str(code, s))
        return 1;

    PREVIOUS_SYMBOL(s, code);
    while (s != start_symbol) { 
        switch (*s) {
            case '/':
                PREVIOUS_SYMBOL(s, code);
                if (s == start_symbol)
                    break;
                if (*s == '*')
                    if (chec_q(code, s))
                        return 1;
                break;
            case '*':
                PREVIOUS_SYMBOL(s, code);
                if (s == start_symbol) 
                    break;
                printf("%c\n", *s);
                if (*s == '/') {
                    if (chec_q(code, s)) 
                        return 1;
                    else 
                        return 0;
                }
                break;
        }

        PREVIOUS_SYMBOL(s, code);
        if (s == start_symbol)
            break;
    }

    return 1;
}

int check_init(fragment_code* code, char* s)
{
    char *start_symbol = get_start_symbol(code);
    int count = 0;
    if (*s == '{') {
        PREVIOUS_SYMBOL(s, code);
        if (s == start_symbol)
            return 0;
        while (*s == ' ') {
            PREVIOUS_SYMBOL(s, code);
            if (s == start_symbol)
                return 0;
        }
        if (*s == '=')
            return 0;
    } else {
        count++;
        while (*s != '\n') {
            PREVIOUS_SYMBOL(s, code);

            if (*s == '{')
                count--;
            if (*s == '{')
                count++;
            
            if (s == start_symbol)
                return 0;
        }

        while (*s == ' ') {
            PREVIOUS_SYMBOL(s, code);
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
    if (*s == 123 && chec_q(code, s) && check_init(code, s))
        (*tab)++;

    // if (*s == 125 && chec_q(code, s) && check_comment(code, s) && check_init(code, s))
    if (*s == 125 && chec_q(code, s) && check_init(code, s))
        (*tab)--;
}

void add_tab(fragment_code* code)
{
    char *s;
    int tab = 0, i, count_p = 0;;
    while (code != NULL) {
        s = &(code->symbol[0]);
        while (*s != '\0') {
            check_tab(code, s, &tab);
            if (*s == 10) {
                while (*s == 10)
                    NEXT_SYMBOL(s, code);
                if (*s == '}')  
                    tab--;

                if (*s == '{') 
                    count_p++;
                for (i = 0; i < tab; i++) {
                    past_symbol(code, s, TAB);
                    NEXT_SYMBOL(s, code);
                }
            }
            if (count_p) {
                tab++;
                count_p = 0;
            }
            NEXT_SYMBOL(s, code);
        }
        code = code->next_code;
    }
}

void add_tab_after_command(fragment_code** _code, char** _s)
{
    fragment_code* code = *_code;
    char* s = *_s;
    int brk = 0;
    if (chec_q(code, s)) {
        while (*s == ' ' || *s == TAB) 
            NEXT_SYMBOL(s, code);
    
        do {
            if (*s == '(')
                brk++;
            if (*s == ')')
                brk--;
                 NEXT_SYMBOL(s,code);
        } while (brk != 0);
        while (*s != '\n') {
            if (*s == '{' || *s == ';')
                return;
            NEXT_SYMBOL(s,code);
        }
        NEXT_SYMBOL(s,code);
        past_symbol(code, s, TAB);
    }
}

void cicl(fragment_code* code)
{
    char command[COMMAND_L], *cur_command; // текущая команда
    char _while[] = "while", _for[] = "for", _if[] = "if";
    char* s;
    int j;

    for (j = 0; j < COMMAND_L; j++) // зануляется
        command[j] = '\0';

    while (code != NULL) {
        s = code->symbol;
        while (*s) {
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
            NEXT_SYMBOL(s, code);
        }
    code = code->next_code;
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
