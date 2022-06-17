void cicl(fragment_code* code);
void write_code(fragment_code* code, FILE* file);
void cformat(fragment_code* code);
void clean_tab(fragment_code* code);

void add_tab(fragment_code* code);
void add_tab_after_command(fragment_code** _code, char** _s);
void add_tab_after_case(fragment_code* code, char* s);

void check_tab(fragment_code* code, char* s, int* tab);
int chec_q(fragment_code* code, char* s);
int check_str(fragment_code* code, char* s);
int check_comment(fragment_code* code, char* s); 
int check_init(fragment_code* code, char* s);

int previous_symbol(fragment_code** code, char** s);
int next_symbol(fragment_code** code, char** s);
int scmp_command(char* str1, char* str2);
void delete_symbol(fragment_code* code, char* pos);
void past_symbol(fragment_code* code, char* pos, char s);

char* get_start_symbol(fragment_code* code);
fragment_code* creating_fragment(fragment_code* f);
int read_code(FILE* f, fragment_code* code);
void check_op(fragment_code* code);
void cformat(fragment_code* code);
void check_new_line(fragment_code* code);