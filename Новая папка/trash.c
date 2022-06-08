fseek(out, pos_start, SEEK_SET)


int copy_file(char* in_name, char* out_name)
{
    FILE *in;
    FILE *out;
    
    in = fopen(in_name, "r");
    out = fopen(out_name, "w");

    if (in == NULL)
        return -1;
    
    char symbol;

    while (1) {
        symbol = fgetc(in);
        if (feof(in))
            break;
        fprintf(out, "%c", symbol);
    }

    return 0;
}

void check_new_line(fragment_code* code)
{
    char *s; //operator[BUF_MEMORY],
    char *back_s; //, *start_symbol = get_start_symbol(code)

    int tab = 0, count; //i = 0, 
    while (code != NULL) {
        s = &(code->symbol[0]);
        while (*s != '\0') {
            switch (*s) {
                // case '{':
                //     if (tab == 0) {
                //         past_symbol(code, s, '\n');
                //         s++;
                //     }
                //     tab++;
                //     break;
                // case '}':
                //     tab--;
                //     back_s = s - 1;
                //     count = 0;
                //     while (*back_s != '\n' && *back_s != NEW_LINE) { // если на этой же строке есть символы
                //         if (*back_s != ' ' || *back_s != TAB) 
                //             count++;
                //         back_s--;
                //     }
                //     if (count == 0)
                //         printf("123");
                //     if (count) { // то переход на следующую строку
                //         past_symbol(code, s, '\n');
                //         s++;
                //     }
                //     break;
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