#include "lib/library.h"
#include "lib/cformat.h"

int main(int argc, char *argv[]) {
    char* name_file = *(argv + 1);

    FILE* file;
    if (!(file = fopen(name_file, "r"))) {
        printf("Файл с названием \"%s\" не найден.\n", name_file);
        return 0;
    }

    fragment_code* code = malloc(sizeof(fragment_code));

    assert(read_code(file, code) == RIGHT);
    cformat(code);
    fclose(file);

    file = fopen(name_file, "w");
    write_code(code, file);

    clean(code);

    return 0;
}
