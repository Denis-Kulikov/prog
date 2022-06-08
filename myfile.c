#include "library.h"
#include "cformat.h"

#include "check.c"
#include "code.c"

#define CHECK_END_SYMBOL(s, code) \
if (!*s) { \
	code = code->next_code; \
	if (code == NULL) \
		return; \
	s = code->symbol; \
}

#define CHECK_BEGIN_SYMBOL(s, code) \
if (s == &(code->symbol[0])) { \
	code = code->previous_code; \
	s = code->symbol; \
	for (; *s != '\0'; s++) \
	s--; \
}

#define NEXT_SYMBOL(s, code) \
s++; \
CHECK_END_SYMBOL(s, code);

#define PREVIOUS_SYMBOL(s, code) \
s--; \
CHECK_BEGIN_SYMBOL(s, code);

void chack_new_line(fragment_code* code)
{
	char *s;
	char *back_s;
	int count; 
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
			}
			s++;
		}
		code = code->next_code;
	}
}

void delete_symbol(fragment_code* code, char* pos)
{
	char *cur = pos, *next = cur + 1;

	while (*cur) {
		*cur = *next;
		cur++;
		next++;
	}
}

void delete_13(fragment_code* code)
{
	char* s;
	int i = 0;
	while (code != NULL) {
		s = &(code->symbol[0]);
		while (*s != '\0') {
			while (*s == 13) {
				delete_symbol(code, s);
				printf("%d ", i);
			}
			i++;
			s++;
		}
		code = code->next_code;
	} 
}

void delet_tab(fragment_code** code, char** s) {
	fragment_code* cur_code = *code;
	char* cur_s = *s;
	if (*cur_s == '\n') { 
		NEXT_SYMBOL(cur_s, cur_code);   
		while (*cur_s == '\n') { // пока переходы
			NEXT_SYMBOL(cur_s, cur_code);   
		}

		while (*cur_s == ' ' && cur_code != NULL) { // если пробел
			delete_symbol(cur_code, cur_s); // удаляем его
			CHECK_END_SYMBOL(cur_s, cur_code);
		}
	}
}


void clean_tab(fragment_code* code)
{
	char* s;
	while (code != NULL) {
		s = code->symbol;
		while (*s) {
			delet_tab(&code, &s);
			NEXT_SYMBOL(s, code);
		}
		code = code->next_code;
	}
}

void check_op(fragment_code* code)
{
	char *s;
	char *back_s;
	int count; 
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
			}
			s++;
		}
		code = code->next_code;
	}
}

int check_figur_p(fragment_code* code, char* s) // проверка комментариев на {}
{
	char *back_s = s, *start_symbol = get_start_symbol(code);
	int parentheses = 0; // count_comment = 0, count_q = 0;

	if (*(back_s + 1) == '\'' && *(back_s - 1) == '\'')
		return 0;

	back_s = s - 1;
	while (back_s != start_symbol) { // если не первый символ
		for (; back_s >= &(code->symbol[0]); back_s--) { // если не первый символ фрагмента
			if (*back_s == '\"') {
				back_s--;
				if (*back_s != '\\')
					parentheses++;
			}
		}

		if (code->previous_code != NULL) {
			code = code->previous_code;
			back_s = &(code->symbol[0]);
			for (; *back_s != '\0'; back_s++); // переход к последнему символу фрагмента
		}
		back_s = start_symbol; 
	}
	return (parentheses + 1) % 2; // если чётное количество скобок переход на новую строку
}

void check_tab(fragment_code* code, char* s, int* tab)
{
	if (*s == 123)
		(*tab)++;

	if (*s == 125)
		(*tab)--;
}

void add_tab(fragment_code* code)
{
	char *s;
	int tab = 0, i, count_p = 0;;
	while (code != NULL) {
		s = &(code->symbol[0]);
		while (*s != '\0') {
			check_tab(code, s, &tab);
			if (*s == 10) {
				while (*s == 10)
				NEXT_SYMBOL(s, code);
				if (*s == '}') 
					tab--;


				if (*s == '{') 
					count_p++;
				for (i = 0; i < tab; i++) {
					past_symbol(code, s, TAB);
					NEXT_SYMBOL(s, code);
				}
			}
			if (count_p) {
				tab++;
				count_p = 0;
			}
			s++;
		}
		code = code->next_code;
	}
}

void write_code(fragment_code* code)
{
	FILE* in;
	in = fopen("myfile.txt", "w");

	char* s;
	while (code != NULL) {
		s = code->symbol;
		while (*s) {
			fprintf(in, "%c", *s);
			s++;
		}
		code = code->next_code;
	}
	fclose(in);
}

void cformat(fragment_code* code)
{
	clean_tab(code);
	write_code(code);
	getchar();

	add_tab(code);
}

int main() {
	char name_file[] = "tes{t.c";
	int mas[][] = {{1, 2, 3}, {1, 2, 3}};
	/* {

		FILE* file; */
		file = fopen(name_file, "r");

		fragment_code* code = malloc(sizeof(fragment_code));
		code->previous_code = NULL;

		assert(read_code(file, code) == RIGHT);
		printf("\n\ncformat:\n");
		cformat(code);

		write_code(code);

		return 0;
	}
