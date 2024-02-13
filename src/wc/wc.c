#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    WORD, BYTE, LINE, CHARS, ALL
} COUNT_MODE;

typedef enum {
    INP_FILE, STDIN
} INPUT_TYPE;

typedef struct {
    char *file_name;
    COUNT_MODE count_mode;
    INPUT_TYPE input_type;
} WCArgs;

int parse_options(char **options, int opt_count, WCArgs *wcargs_p) {
    // Skip the first argv entry
    for (int i = 1; i < opt_count; ++i) {
        char buf[128];
        strcpy(buf, options[i]);
        if (buf[0] != '-') {
            strcpy(wcargs_p->file_name, buf);
        }
    }
    return 0;
}

static inline WCArgs* init_wcargs() {
    WCArgs *new_wcargs_p = (WCArgs*) malloc(sizeof(WCArgs));
    new_wcargs_p->file_name = (char*) malloc(sizeof(char));
    new_wcargs_p->count_mode = ALL;
    new_wcargs_p->input_type = STDIN;
    return new_wcargs_p;
}

int main(int argc, char *argv[]) {
    WCArgs *wcargs_p = init_wcargs();
    if (argc > 1) {
        if (parse_options(argv, argc, wcargs_p) != 0) return -1;
        printf("%s", wcargs_p->file_name);
    } else {
    }
    return 0;
}
