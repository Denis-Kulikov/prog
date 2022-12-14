#include "library.h"
#include "cformat.h"

void print_code(fragment_code* code)
{
    char* s = code->symbol;
    printf("%c", *s);
    while (next_symbol(&code, &s)) {
        printf("%c", *s);
    }
}

fragment_code* creating_fragment(fragment_code* f)
{
    assert(f != NULL);

    if (f->next_code != NULL) {
        fragment_code* save = f->next_code;
        f->next_code = malloc(sizeof(fragment_code));
        f->next_code->previous_code = f;
        f->next_code->next_code = save;
        save->previous_code = f->next_code;
    } else {
        f->next_code = malloc(sizeof(fragment_code));
        f->next_code->previous_code = f;
    }

    return f->next_code;
}

void clean(fragment_code* code)
{
    while (code->next_code)
        code = code->next_code;
    
    fragment_code* delete = code;
    code = code->previous_code;

    while (delete) {
        free(delete);
        delete = code;
        code = code->previous_code;
    }
}

int read_code(FILE* f, fragment_code* code)
{
    int cout_tab[] = {0, 0};
    int i = 0;
    char s = fgetc(f);
    for (; !feof(f); s = fgetc(f)) {
        if (s == 13)
            continue;
        if (i >= MAX_CODE_LENGTH || (cout_tab[BE] && !cout_tab[SUM])) {
                code->symbol[i++] = s;
                code->symbol[i] = '\0';
                i = 0;
                cout_tab[BE] = 0;
                code = creating_fragment(code);
        } else {
            switch (s) {
                case '{':
                    cout_tab[BE]++;
                    cout_tab[SUM]++;
                    break;
                case '}':
                    cout_tab[SUM]--;
                    break;
            }
            code->symbol[i++] = s;
        }
    }
    code->symbol[++i] = '\0';

    return 0;
}