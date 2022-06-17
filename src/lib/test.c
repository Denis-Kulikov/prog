#include "library.h"
#include "cformat.h"

#include "cformat.c"
#include "add.c"
#include "operation.c"
#include "check.c"
#include "code.c"

void check_op(fragment_code* code)
{
	char *s = code->symbol;
	fragment_code* back_c = code;
	char *back_s;
	while (1) {
		if (*s == ';' && chec_q(code, s) && check_comment(code, s)) {
			back_s = s;
			if (!previous_symbol(&back_c, &back_s) || !check_parity(code, s)) {
				if (!next_symbol(&code, &s))
					return;
				continue;
			}

			while (1) {
				if (*back_s == '\n')
					break;
				if ((*back_s == ';' || *back_s == '{' || *back_s == '}') &&
				chec_q(back_c, back_s) && check_comment(back_c, back_s)) {
					next_symbol(&back_c, &back_s);
					past_symbol(back_c, back_s, '\n');
					next_symbol(&code, &s);
					break;
				}
				if (!previous_symbol(&back_c, &back_s))
					break;
			}

		}

		if (!next_symbol(&code, &s))
			break;
	}
}

int main()
{
	char name_file[] = "test.c";

	FILE* file;
	file = fopen(name_file, "r");

	fragment_code* code = malloc(sizeof(fragment_code));

	assert(read_code(file, code) == RIGHT);
	cformat(code);

	write_code(code);

	return 0;
}
