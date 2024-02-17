#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_LEN_MAX 128
#define FILE_COUNT_MAX 16
#define COUNT_STR_LEN_MAX 32

typedef struct {
    unsigned int lines  : 1;
    unsigned int words  : 1;
    unsigned int chars  : 1;
    unsigned int bytes  : 1;
} COUNTER_FLAGS;

typedef enum {
    INP_FILE, STDIN
} INPUT_TYPE;

typedef enum {
    NONE, E_INVALID_OPT
} ERROR;

typedef struct {
    char (*files)[FILE_COUNT_MAX];
    int file_count;
    COUNTER_FLAGS counter_flags;
    int flags_count;
    INPUT_TYPE input_type;
    ERROR error;
} WCArgs;

WCArgs* init_wcargs() {
    WCArgs *new_wcargs_p = malloc(sizeof(WCArgs));
    new_wcargs_p->files = malloc(sizeof(char[FILE_COUNT_MAX][ARG_LEN_MAX]));
    new_wcargs_p->file_count = 0;
    (new_wcargs_p->counter_flags).lines = 0;
    (new_wcargs_p->counter_flags).words = 0;
    (new_wcargs_p->counter_flags).chars = 0;
    (new_wcargs_p->counter_flags).bytes = 0;
    new_wcargs_p->flags_count = 0;
    new_wcargs_p->input_type = STDIN;
    new_wcargs_p->error = NONE;
    return new_wcargs_p;
}

/*
 * The first character of flag_str should be a '-' which has to be skipped
 */
void toggle_flags(char flag_str[], WCArgs *wcargs_p) {
    int i = 1;
    while (flag_str[i] != '\0') {
        switch (flag_str[i]) {
            case 'w':
                (wcargs_p->counter_flags).words = 1;
                break;
            case 'l':
                (wcargs_p->counter_flags).lines = 1;
                break;
            case 'm':
                (wcargs_p->counter_flags).chars = 1;
                break;
            case 'c':
                (wcargs_p->counter_flags).bytes = 1;
                break;
            default:
                wcargs_p->error = E_INVALID_OPT;
                break;
        }
        ++(wcargs_p->flags_count);
        ++i;
    }
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
        // Expecting flags to start with a '-'
        if (buf[0] != '-') {
            strcpy((wcargs_p->files)[(wcargs_p->file_count)++], buf);
        } else {
            toggle_flags(buf, wcargs_p);
        }
    }
    if (wcargs_p->file_count > 0) {
        wcargs_p->input_type = INP_FILE;
    }
    // If no flags have been toggled, display all information
    if (wcargs_p->flags_count == 0) {
        toggle_flags("-wlmc", wcargs_p);
    }
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
