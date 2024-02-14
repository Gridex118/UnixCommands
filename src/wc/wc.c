#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_LENGTH_MAX 128
#define FILE_COUNT_MAX 16

typedef enum {
    WORD, BYTE, LINE, CHAR, ALL
} COUNT_MODE;

typedef enum {
    INP_FILE, STDIN
} INPUT_TYPE;

typedef struct {
    char (*files)[FILE_COUNT_MAX];
    int file_count;
    COUNT_MODE count_mode;
    INPUT_TYPE input_type;
} WCArgs;

int parse_options(char **options, int opt_count, WCArgs *wcargs_p) {
    // Skip the first argv entry
    for (int i = 1; i < opt_count; ++i) {
        char buf[ARG_LENGTH_MAX];
        strcpy(buf, options[i]);
        if (buf[0] != '-') {
            strcpy((wcargs_p->files)[(wcargs_p->file_count)++], buf);
        } else {
            switch (buf[0]) {
                case 'w':
                    wcargs_p->count_mode = WORD;
                    break;
                case 'l':
                    wcargs_p->count_mode = LINE;
                    break;
                case 'b':
                    wcargs_p->count_mode = CHAR;
                    break;
                case 'c':
                    wcargs_p->count_mode = BYTE;
                    break;
                default:
                    return -1;
                    break;
            }
        }
    }
    if (wcargs_p->file_count > 0) { wcargs_p->input_type = INP_FILE; }
    return 0;
}

static inline WCArgs* init_wcargs() {
    WCArgs *new_wcargs_p = malloc(sizeof(WCArgs));
    new_wcargs_p->files = malloc(sizeof(char[FILE_COUNT_MAX][ARG_LENGTH_MAX]));
    new_wcargs_p->file_count = 0;
    new_wcargs_p->count_mode = ALL;
    new_wcargs_p->input_type = STDIN;
    return new_wcargs_p;
}

int main(int argc, char *argv[]) {
    WCArgs *wcargs_p = init_wcargs();
    if (argc > 1) {
        if (parse_options(argv, argc, wcargs_p) != 0) return -1;
        for (int i = 0; i < wcargs_p->file_count; ++i) {
            printf("%s\n", (wcargs_p->files)[i]);
        }
    } else {
    }
    return 0;
}
