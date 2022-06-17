#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define SPACE 32
#define TAB 9 // tabulation
#define NEW_LINE 13

#define BUF_MEMORY 2000
#define MAX_CODE_LENGTH 1000

#define COMMAND_L 10

#define SAVE_FILE "save.txt"
#define HELP_FILE "helper.txt"

enum {
    RIGHT,
    ERROR
};

enum {
    BE,
    SUM
};

typedef struct _fragment_code{
    char symbol[BUF_MEMORY];
    struct _fragment_code* previous_code;
    struct _fragment_code* next_code;
} fragment_code;

typedef struct {
    int open;
    int close;
} code_info;