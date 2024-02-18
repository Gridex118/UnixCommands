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

typedef struct {
    size_t words, lines, chars, bytes;
} Counter;

static inline int is_whitespace(char c) {
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') return 1;
    return 0;
}

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
        toggle_flags("-wlm", wcargs_p);
    }
}

/*
 * Store the queried count information for file in count_str
 */
void count(FILE *file, char *count_str, COUNTER_FLAGS counter_flags) {
    count_str[0] = '\0';
    Counter counter = {0, 0, 0, 0};
    char c;
    while ((c = fgetc(file)) != EOF) {
        ++counter.chars;
        ++counter.bytes;    // Yeah, no clue how to count bytes
        if (c == ' ' || c == '\n') {
            while (is_whitespace(c)) {
                if (c == '\n') ++counter.lines;
                ++counter.chars;
                c = fgetc(file);
            }
            ++counter.words;
        }
    }
    if (counter_flags.lines) {
        char lines_str[16];
        snprintf(lines_str, 16, "%ld ", counter.lines);
        strcat(count_str, lines_str);
    }
    if (counter_flags.words) {
        char words_str[16];
        snprintf(words_str, 16, "%ld ", counter.words);
        strcat(count_str, words_str);
    }
    if (counter_flags.chars) {
        char chars_str[16];
        snprintf(chars_str, 16, "%ld ", counter.chars);
        strcat(count_str, chars_str);
    }
    if (counter_flags.bytes) {
        char bytes_str[16];
        snprintf(bytes_str, 16, "%ld ", counter.bytes);
        strcat(count_str, bytes_str);
    }
}

int wc(WCArgs *wcargs_p) {
    if (wcargs_p->input_type == INP_FILE) {
        for (int i = 0; i < wcargs_p->file_count; ++i) {
            char *file_name = (wcargs_p->files)[i];
            FILE *file = fopen(file_name, "r");
            if (file == NULL) return -1;
            char count_str[COUNT_STR_LEN_MAX];
            count(file, count_str, wcargs_p->counter_flags);
            printf("%s %s\n", count_str, file_name);
        }
    } else {
        char count_str[COUNT_STR_LEN_MAX];
        count(stdin, count_str, wcargs_p->counter_flags);
        printf("%s\n", count_str);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    WCArgs *wcargs_p = init_wcargs();
    if (wcargs_p == NULL) return -1;
    // if no cmd line options were passed, use options set up in init_wcargs
    if (argc > 1) {
        parse_options(argv, argc, wcargs_p);
        if (wcargs_p->error != NONE) return -1;
    }
    return wc(wcargs_p);
}
