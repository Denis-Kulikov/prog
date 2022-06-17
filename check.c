void check_tab(fragment_code* code, char* s, int* tab)
{
    // if (*s == 123 && chec_q(code, s) && check_comment(code, s) && check_init(code, s))
    if (*s == 123 && chec_q(code, s) && check_comment(code, s)) {
        (*tab)++;
    }

    // if (*s == 125 && chec_q(code, s) && check_comment(code, s) && check_init(code, s))
    if (*s == 125 && chec_q(code, s) && check_comment(code, s)) {
        (*tab)--;
    }
}

int chec_q(fragment_code* code, char* s)
{
    // char *start_symbol = get_start_symbol(code);
    int count = 0;

    if (*(s + 1) == '\'' && *(s - 1) == '\'')
        return 0;

    if (!previous_symbol(&code, &s))
        return 1;

    while (1) { 
        if (*s == '\"') {
            if (!previous_symbol(&code, &s)) {
                count++;
                break;
            }
            if (*s != '\\')
                count++;
        }
        
        if (!previous_symbol(&code, &s))
            break;
    }

    return (count + 1) % 2; // 1 если НЕ внутри
}

int check_str(fragment_code* code, char* s) 
{
    
    while (*s != '\n') {
        if (*s == '/')
            if (chec_q(code, s)) {
                if (!previous_symbol(&code, &s))
                    return 1;

                if (*s == '/')
                    return 0;
            }
        
        if (!previous_symbol(&code, &s))
            break;
    }
    return 1;
}

int check_comment(fragment_code* code, char* s)
{
    int comment_count = 0;
    if (!check_str(code, s))
        return 0;

    if (!previous_symbol(&code, &s))
        return 1;

    while (1) { 
        switch (*s) {
            case '/':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '*')
                    if (chec_q(code, s))
                        comment_count--;
                break;
            case '*':
                if (!previous_symbol(&code, &s))
                    break;
                if (*s == '/') 
                    if (chec_q(code, s)) 
                        comment_count++;
                break;
        }

        if (!previous_symbol(&code, &s))
            break;
    }

    return (comment_count + 1) % 2;
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