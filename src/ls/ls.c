#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FILENAME_SIZE_MAX 32
#define DIRECTORY_COUNT_MAX 16

typedef struct {
    unsigned int hidden: 1;
    unsigned int implied: 1;
} LsFlags;

typedef struct {
    int directory_count;
    char directories[DIRECTORY_COUNT_MAX][FILENAME_SIZE_MAX];
    LsFlags flags;
} LsArgs;

void init_lsargs(LsArgs *restrict lsargs_p) {
    lsargs_p->directory_count = 0;
    lsargs_p->flags.hidden = 0;
    lsargs_p->flags.implied = 0;
}

void toggle_flags(char *const flags_str, LsArgs *restrict lsargs_p) {
    int i = 1;    // All options start with a hyphen; skip that
    while (flags_str[i] != '\0') {
        switch (flags_str[i]) {
            case 'a':
                lsargs_p->flags.implied = 1;
            case 'A':
                lsargs_p->flags.hidden = 1;
                break;
        }
        ++i;
    }
}

void parse_options(char **const options, const int opt_count, LsArgs *restrict lsargs_p) {
    for (int i = 1; i < opt_count; ++i) {
        char buf[FILENAME_SIZE_MAX];
        strcpy(buf, options[i]);
        if (buf[0] == '-') {
            toggle_flags(options[i], lsargs_p);
        } else {
            strcpy(lsargs_p->directories[lsargs_p->directory_count], buf);
            lsargs_p->directory_count++;
        }
    }
    if (lsargs_p->directory_count == 0) {
        strcpy(lsargs_p->directories[lsargs_p->directory_count++], ".");
    }
}

void ls(LsArgs *const lsargs_p) {
    for (int i = 0; i < lsargs_p->directory_count; ++i) {
        char *const directory_name = lsargs_p->directories[i];
        DIR *dir = opendir(directory_name);
        struct dirent *entry;
        if (dir) {
            while ((entry = readdir(dir)) != NULL) {
                char *const name = entry->d_name;
                if (name[0] == '.') {
                    if (!lsargs_p->flags.hidden) continue;
                }
                printf("%s\n", name);
            }
            closedir(dir);
        } else {
            fputs("Could not open directory\n", stderr);
        }
    }
}

int main(int argc, char *argv[]) {
    LsArgs lsargs;
    init_lsargs(&lsargs);
    parse_options(argv, argc, &lsargs);
    ls(&lsargs);
    return 0;
}
