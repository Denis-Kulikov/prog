

#define AVAILABLE_SYMBOL(i) command[i] == ' ' || command[i] == TAB || command[i] == '{' || \
            command[i] == '}' || command[i] == ';' || command[i] == '(' || \
            command[i] == ')' || command[i] == TAB || command[i] == '\n'


int check_shift_cicle(fragment_code* code, char* s, int i)
{
    for (int j = 0; j < i; j++)
        previous_symbol(&code, &s);

    while (*s == ' ' || *s == TAB) {
        if (!previous_symbol(&code, &s))
            break;
        if (*s == '\n')
            return 0;
    }
    next_symbol(&code, &s);
    past_symbol(code, s, '\n');

    return 1; // если перенесли на новую строку
}

void clean_tab(fragment_code* code)
{
    char* s = code->symbol;
    while (1) {
        if (*s == '\n') {
            while (*s == '\n') // нужно ли это?
                if (!next_symbol(&code, &s))
                    return;
            while (*s == ' ' || *s == TAB) {
                delete_symbol(code, s);
                if (!(*s) && (!next_symbol(&code, &s)))
                    return;
            }
        } else
            if (!next_symbol(&code, &s))
                break;
    }
}

void cicl(fragment_code* code)
{
    char command[COMMAND_L], *cur_command; // текущая команда
    char _while[] = "while", _for[] = "for", _if[] = "if";
    char* s = code->symbol;
    int j, i = 1;

    for (j = 0; j < COMMAND_L; j++) // зануляется
        command[j] = '\0';

    while (1) {
        if (*s == '\n')
            i++;
        for (j = 0; j <= COMMAND_L - 2; j++) // сдвиг влево
            command[j] = command[j + 1];
        command[COMMAND_L - 1] = *s; // получение символов 

        if (AVAILABLE_SYMBOL(4)) {
            cur_command = command + 5;
            if (!scmp_command(cur_command, _while)) {
                add_tab_after_command(&code, &s);
                printf("%d) %s\n", i, command);
                // i++;
            }
        }

        if (AVAILABLE_SYMBOL(6)) {
            cur_command = command + 7;
            if (!scmp_command(cur_command, _for)) {
                add_tab_after_command(&code, &s);
                printf("%d) %s\n", i, command);
                // i++;
            }
        }

        if (AVAILABLE_SYMBOL(7)) {
            cur_command = command + 8;
            if (!scmp_command(cur_command, _if)) {
                if (check_shift_cicle(code, s, 2))
                    next_symbol(&code, &s);
                add_tab_after_command(&code, &s);
                printf("%d)", i);
                for (int k = 0; k < COMMAND_L; k++)
                    printf("%c=", command[k]);
                printf("\n");
                // i++;
            }
        }

        if (!next_symbol(&code, &s))
            break;
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

    // moving_figur(code);
    // getchar();

    // check_new_line(code);
    // check_op(code);
    add_tab(code);
    cicl(code);
}