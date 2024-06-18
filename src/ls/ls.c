#include <dirent.h>
#include <stdio.h>

typedef struct {
    
} LsArgs;

void parse_options(char **options) {

}

void print_contents(char *const directory_name) {
    DIR *dir = opendir(directory_name);
    struct dirent *entry;
    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    return 0;
}
