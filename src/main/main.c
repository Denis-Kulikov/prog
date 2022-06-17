#include "lib/library.h"
#include "lib/cformat.h"
void check_op(fragment_code* code)
{
    char *s = code->symbol;
    fragment_code* back_c = code;
    char *back_s;
    while (1) {
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

int main(int argc, char *argv[]) {
    char* name_file = *(argv + 1);

    FILE* file;
    if (!file = fopen(name_file, "r")) {
        printf("Файл с названием \"%s\" не найден.\n", name_file);
        return 0;
    }

    fragment_code* code = malloc(sizeof(fragment_code));

    assert(read_code(file, code) == RIGHT);
    cformat(code);
    fclose(file);

    file = fopen(name_file, "w");
    write_code(code, file);

    return 0;
}
