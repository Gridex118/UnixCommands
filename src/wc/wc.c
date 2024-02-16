#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_LEN_MAX 128
#define FILE_COUNT_MAX 16
#define COUNT_STR_LEN_MAX 32

typedef enum {
    WORD, BYTE, LINE, CHAR, ALL
} COUNT_MODE;

typedef enum {
    INP_FILE, STDIN
} INPUT_TYPE;

typedef enum {
    NONE, E_INVALID_OPT
} ERROR;

typedef struct {
    char (*files)[FILE_COUNT_MAX];
    int file_count;
    COUNT_MODE count_mode;
    INPUT_TYPE input_type;
    ERROR error;
} WCArgs;

WCArgs* init_wcargs() {
    WCArgs *new_wcargs_p = malloc(sizeof(WCArgs));
    new_wcargs_p->files = malloc(sizeof(char[FILE_COUNT_MAX][ARG_LEN_MAX]));
    new_wcargs_p->file_count = 0;
    new_wcargs_p->count_mode = ALL;
    new_wcargs_p->input_type = STDIN;
    new_wcargs_p->error = NONE;
    return new_wcargs_p;
}

/*
 * Set up the program's flags
 * options expects argv to be passed to it
 * opt_count expects argc to be passed to it
 */
void parse_options(char **options, int opt_count, WCArgs *wcargs_p) {
    // Skip the first argv entry
    for (int i = 1; i < opt_count; ++i) {
        char buf[ARG_LEN_MAX];
        strcpy(buf, options[i]);
        // Expecting flags to be a single character starting with a '-'
        if (buf[0] != '-') {
            strcpy((wcargs_p->files)[(wcargs_p->file_count)++], buf);
        } else {
            switch (buf[1]) {
                case 'w':
                    wcargs_p->count_mode = WORD;
                    break;
                case 'l':
                    wcargs_p->count_mode = LINE;
                    break;
                case 'm':
                    wcargs_p->count_mode = CHAR;
                    break;
                case 'c':
                    wcargs_p->count_mode = BYTE;
                    break;
                default:
                    wcargs_p->error = E_INVALID_OPT;
                    break;
            }
        }
    }
    if (wcargs_p->file_count > 0) { wcargs_p->input_type = INP_FILE; }
}

int main(int argc, char *argv[]) {
    WCArgs *wcargs_p = init_wcargs();
    if (wcargs_p == NULL) return -1;
    // if no cmd line options were passed, use options set up in init_wcargs
    if (argc > 1) {
        parse_options(argv, argc, wcargs_p);
        if (wcargs_p->error != NONE) return -1;
        for (int i = 0; i < wcargs_p->file_count; ++i) {
            printf("%s\n", (wcargs_p->files)[i]);
        }
    } else {
    }
    return 0;
}
