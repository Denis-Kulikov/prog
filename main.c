#include "library.h"
#include "cformat.h"

#include "cformat.c"
#include "add.c"
#include "operation.c"
#include "check.c"
#include "code.c"

void check_op(fragment_code* code) // ###
{
    char *s = code->symbol;
    char *back_s;
    int count; 
    while (1) {
        if (*s == ';') {
            back_s = s;
            if (!previous_symbol(&code, &s))
                return;
            count = 0;
            while (*back_s != '\n' && *back_s != '(') {
                if (*back_s == ';' || *back_s == '{' || *back_s == '}' || *back_s == ')') {
                    count++;
                    break;
                }
                if (!previous_symbol(&code, &s))
                    break;
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

int main() {
    char name_file[] = "test.c";

    FILE* file;
    file = fopen(name_file, "r");

    fragment_code* code = malloc(sizeof(fragment_code));

    assert(read_code(file, code) == RIGHT);
    cformat(code);

    write_code(code);

    return 0;
}
