int previous_symbol(fragment_code** _code, char** _s);
int next_symbol(fragment_code** code, char** s);
int scmp_command(char* str1, char* str2);
void delete_symbol(fragment_code* code, char* pos);

char* get_start_symbol(fragment_code* code);
fragment_code* creating_fragment(fragment_code* f);
int read_code(FILE* f, fragment_code* code);
void past_symbol(fragment_code* code, char* pos, char s);
void check_op(fragment_code* code);
void cformat(fragment_code* code);
void check_new_line(fragment_code* code);