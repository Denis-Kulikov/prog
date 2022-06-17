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
    int count, i = 1; 
    while (1) {
        if (*s == ';' && chec_q(code, s) && check_comment(code, s)) {
            back_s = s;
            if (!previous_symbol(&code, &back_s) || check_parity(code, s)) {
                if (!next_symbol(&code, &s))
                    return;
                continue;
            }

            count = 0;
            while (1) {
                if (*back_s == ';' || *back_s == '{' || *back_s == '}') {
                    count++;
                    break;
                }

                if (*back_s != '\n' || !previous_symbol(&code, &back_s))
                    break;
            }
            if (count) {
                printf("%d\n", i);
                next_symbol(&code, &back_s);
                past_symbol(code, back_s, '\n');
                // next_symbol(&code, &s);
            }
        }
        
        
        if (!next_symbol(&code, &s))
            break;
        if (*s == '\n')
            i++;
    }
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
