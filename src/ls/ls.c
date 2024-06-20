#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define FILENAME_SIZE_MAX 32
#define DIRECTORY_COUNT_MAX 16

typedef struct {
    unsigned int show_hidden: 1;
    unsigned int show_this_and_parent: 1;
    unsigned int show_size: 1;
} LsFlags;

typedef struct {
    int directory_count;
    char directories[DIRECTORY_COUNT_MAX][FILENAME_SIZE_MAX];
    LsFlags flags;
    unsigned int block_size_bytes;
} LsArgs;

void init_lsargs(LsArgs *restrict lsargs_p) {
    lsargs_p->directory_count = 0;
    lsargs_p->flags.show_hidden = 0;
    lsargs_p->flags.show_this_and_parent = 0;
    lsargs_p->flags.show_size = 0;
    lsargs_p->block_size_bytes = 1024;
}

void toggle_flags(char *const flags_str, LsArgs *restrict lsargs_p) {
    int i = 1;    // All options start with a hyphen; skip that
    while (flags_str[i] != '\0') {
        switch (flags_str[i]) {
            case 'a':
                lsargs_p->flags.show_this_and_parent = 1;
            case 'A':
                lsargs_p->flags.show_hidden = 1;
                break;
            case 's':
                lsargs_p->flags.show_size = 1;
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

int file_blocks_used(char *const file_name, const int block_size) {
    FILE *file = fopen(file_name, "r");
    int blocks = 0;
    if (file) {
        fseek(file, 0L, SEEK_END);
        double size = (double)ftell(file)/block_size;
        blocks = ceil(size);
    } else {
        printf("Error opening file %s\n", file_name);
    }
    return blocks;
}

int ls(LsArgs *const lsargs_p) {
    for (int i = 0; i < lsargs_p->directory_count; ++i) {
        char *const directory_name = lsargs_p->directories[i];
        DIR *dir = opendir(directory_name);
        struct dirent *entry;
        if (dir) {
            while ((entry = readdir(dir)) != NULL) {
                char *const name = entry->d_name;
                if (name[0] == '.') {
                    if (!lsargs_p->flags.show_hidden)
                        continue;
                    if ((strcmp(name, ".") == 0 || strcmp(name, "..") == 0) && !lsargs_p->flags.show_this_and_parent)
                        continue;
                }
                if (lsargs_p->flags.show_size) {
                    if (entry->d_type == DT_DIR) {
                        printf("4 ");
                    } else {
                        char full_file_path[FILENAME_SIZE_MAX * 2] = "";
                        strcpy(full_file_path, directory_name);
                        strcat(full_file_path, "/");
                        strcat(full_file_path, name);
                        printf("%d ", file_blocks_used(full_file_path, lsargs_p->block_size_bytes));
                    }
                }
                printf("%s\n", name);
            }
            closedir(dir);
        } else {
            fputs("Could not open directory\n", stderr);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    LsArgs lsargs;
    init_lsargs(&lsargs);
    parse_options(argv, argc, &lsargs);
    return ls(&lsargs);
}
