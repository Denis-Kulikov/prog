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

void check_new_line(fragment_code* code)
{
    char *s; //operator[BUF_MEMORY],
    char *back_s; //, *start_symbol = get_start_symbol(code)

    int count; //i = 0, 
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
                break;
            }
            s++;
        }
        code = code->next_code;
    }
}