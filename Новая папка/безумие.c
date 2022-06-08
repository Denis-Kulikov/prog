int read_code(FILE* f, fragment_code* code)
{
    int cout_tab[] = {0, 0};
    int i = 0;
    char s = fgetc(f);
    for (; !feof(f); s = fgetc(f)) {
        // printf("%c", s);
        if (s == '\n') {
            printf("%c", s);
            continue;
        }
        if (i >= MAX_CODE_LENGTH || (cout_tab[BE] && !cout_tab[SUM])) {
                printf("%c", s);   
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
            printf("%c", s);   
            code->symbol[i] = s;
            i++;
        }
    }
    code->symbol[++i] = '\0';

    return 0;
}