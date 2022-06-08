#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SPACE 32
#define TAB 9 // tabulation
#define NEW_LINE 13

#define BUF_MEMORY 2000
#define MAX_CODE_LENGTH 1000

#define SAVE_FILE "save.txt"
#define HELP_FILE "helper.txt"

    // fseek(out, pos_start, SEEK_SET)


// int copy_file(char* in_name, char* out_name)
// {
//     FILE *in;
//     FILE *out;
    
//     in = fopen(in_name, "r");
//     out = fopen(out_name, "w");

//     if (in == NULL)
//         return -1;
    
//     char symbol;

//     while (1) {
//         symbol = fgetc(in);
//         if (feof(in))
//             break;
//         fprintf(out, "%c", symbol);
//     }

//     return 0;
// }

enum {
    RIGHT,
    ERROR
};

enum {
    BE,
    SUM
};

typedef struct _fragment_code{
    char symbol[BUF_MEMORY];
    struct _fragment_code* previous_code;
    struct _fragment_code* next_code;
} fragment_code;

typedef struct {
    int open;
    int close;
} code_info;

char* get_start_symbol(fragment_code* code)
{
    while (code->previous_code != NULL) 
        code = code->previous_code;

    return &(code->symbol[0]);
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

int read_code(FILE* f, fragment_code* code)
{
    int cout_tab[] = {0, 0};
    int i = 0;
    char s = fgetc(f);
    for (; !feof(f); s = fgetc(f)) {
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

void print_code(fragment_code* code)
{
    char* s;
    while (code != NULL) {
        s = code->symbol;
        while (*s) {
            printf("%c", *s);
            s++;
        }
        code = code->next_code;
    }
}

void past_symbol(fragment_code* code, char* pos, char s)
{
    // printf("%c", *pos);
    char *cur = pos, *next;
    int i = 0;

    while (*cur) {
        // printf("%c", *cur);
        cur++;
        i++;
    }
    next = cur + 1;

    for (; i >= 0; i--) {
        *next = *cur;
        // printf("%c", *next);
        cur--;
        next--;
    }
    // printf(" = check\n");

    *pos = s;
}

int check_parity(char* s, fragment_code* code)
{
    char *back_s = s - 1, *start_symbol = get_start_symbol(code);
    int parentheses = 0;
    fragment_code* cur_code = code;
    while (back_s != start_symbol) { // если не первый символ
        for (; back_s >= &(cur_code->symbol[0]); back_s--) // если не первый символ фрагмента
            if (*back_s == '(')
                parentheses++;
            else if (*back_s == ')')
                parentheses--;
        if (cur_code->previous_code != NULL) {
            cur_code = cur_code->previous_code;
            back_s = &(cur_code->symbol[0]);
            for (; *back_s != '\0'; back_s++); // переход к последнему символу фрагмента
        }
        back_s = start_symbol; 
    }

    return parentheses; // если чётное количество скобок переход на новую строку
}

void chack_new_line(fragment_code* code)
{
    char *s; //operator[BUF_MEMORY],
    char *back_s; //, *start_symbol = get_start_symbol(code)

    int tab = 0, count; //i = 0, 
    while (code != NULL) {
        s = &(code->symbol[0]);
        while (*s != '\0') {
            switch (*s) {
                case ';':
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

                    break;
            }
            s++;
        }
        code = code->next_code;
    }
}

void cformat(fragment_code* code)
{
    // print_code(code);
    // printf("\n");
    int i, tab = 0, space = 0;
    char* s;
    while (code != NULL) {
        i = 0;
        s = code->symbol;
        while (*s) {
            printf("%c", *s);
            if (*s == '{')
                printf("***"); 
            switch (*s) {
                case '{':
                    tab++;
                    break;
                case '}':
                    tab--;
                    break;
                case '\n':
                    // printf("tab = %d\t", tab);
                    s++;
                    printf("%c", *s);
                    space++;
                    // past_symbol(code, &i, &tab, TAB);
                    // chack_tab(s, &i, tab, code);
                    break;
            }
            i++;
            s++;
        }
        code = code->next_code;
    }
    // printf("\nSpace = %d\n\n", space);
    // pos_error = check(code);
}

void check_new_line(fragment_code* code)
{
    char* s;
    while (code != NULL) {
        s = code->symbol;
        while (*s) {
            for (; *s; s++) {
                if (*s == ';') {
                    s++;
                    past_symbol(code, s, '\n');
                }
            }
        }
        code = code->next_code;
    }
}

int main() {
    char name_file[] = "test.c";

    FILE* file;
    file = fopen(name_file, "r");

    fragment_code* code = malloc(sizeof(fragment_code));
    code->previous_code = NULL;
    
    assert(read_code(file, code) == RIGHT);
    // print_code(code);
    // cformat(code);
    printf("\n\ncformat:\n");
    // chack_tab(code);
    chack_new_line(code);
    // past_symbol(code, code->symbol, TAB);
    print_code(code);

    // int i = 0;
    // while (code != NULL) {
    //     i++;
    //     code = code->next_code;
    // }
    // printf("\n\n%d\n", i);
    
    
    // FILE* in;
    // in = fopen("myfile.txt", "w");
    
    // char* s;
    // while (code != NULL) {
    //     s = code->symbol;
    //     while (*s) {
    //         fprintf(in, "%c", *s);
    //         s++;
    //     }
    //     code = code->next_code;
    // }

    return 0;
}
