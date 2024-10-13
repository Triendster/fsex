#include "fsex.h"
#include "config.h"
#include <dirent.h>
#include <linux/limits.h>
#include <ncurses.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int start_y = 0;
int start_x = 0; 
int highlighted = 0;
char* current_path;

Entry** get_dir_contents(const char *path, int ignore_hidden, int *_entry_count)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        // we need a formatted error message
        printf("\nError trying to open directory: %s\n", path);
        perror("opendir");
        return NULL;
    }

    int entry_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (ignore_hidden && entry->d_name[0] == '.') {
            continue;
        }
        entry_count++;
    }
    rewinddir(dir);

    Entry **entries = malloc((entry_count + 3) * sizeof(Entry*));
    *_entry_count = entry_count + 2;

    entries[0] = malloc(sizeof(Entry));
    entries[0]->name = strdup(".");
    entries[0]->type = FT_DIR;
    entries[1] = malloc(sizeof(Entry));
    entries[1]->name = strdup("..");
    entries[1]->type = FT_DIR;

    int i = 2;
    while ((entry = readdir(dir)) != NULL) {
        if (ignore_hidden && entry->d_name[0] == '.') {
            continue;
        }
        entries[i] = malloc(sizeof(Entry));
        entries[i]->name = strdup(entry->d_name);
        if (entry->d_type == DT_DIR) {
            entries[i]->type = FT_DIR;
        } else {
            entries[i]->type = FT_FILE;
        }
        i++;
    }
    entries[i] = NULL;
    closedir(dir);
    return entries;
}

int is_valid_dir(const char *path)
{
    struct stat sb;
    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 1;
    }
    return 0;
}

void free_entries(int entry_count, Entry **entries)
{
    for (int i = 0; i < entry_count; i++) {
        // Do this because strdup allocates memory
        free(entries[i]->name);
        free(entries[i]);
    }
    free(entries);
}

void draw_entries(int entry_count, Entry **entries)
{
    for (int i = 0; i < entry_count; i++) {
        if (i == highlighted) {
            attron(A_REVERSE);
            mvprintw(start_y + i, start_x, entries[i]->name);
            attroff(A_REVERSE);
        } else {
            mvprintw(start_y + i, start_x, entries[i]->name);
        }
    }
}

char* get_new_path(const char *target)
{
    int path_len = strlen(current_path);
    int target_len = strlen(target);
    char* new_path;

    if (strcmp(target, "..") == 0) {
        if (strcmp(current_path, "/") == 0) {
            new_path = strdup(current_path);
        }
        else {
            int i = 0;
            // we can stop if the index is 0 or if we find a '/'
            while (path_len -i - 1 > 0 && current_path[path_len - i - 1] != '/') {
                i++;
            }

            if (path_len - i - 1 == 0) {
                new_path = strdup("/");
            }
            else {
                new_path = malloc(path_len - i);
                strncpy(new_path, current_path, path_len - i - 1);
                new_path[path_len - i - 1] = '\0';
            }
        }
    }
    else if (strcmp(target, ".") == 0) {
        new_path = strdup(current_path);
    }
    else {
        new_path = malloc(path_len + target_len + 2);
        strcpy(new_path, current_path);
        if (current_path[path_len - 1] != '/') {
            strcat(new_path, "/");
        }
        strcat(new_path, target);
        new_path[path_len + target_len + 1] = '\0';
    }

    return new_path;
}

int main(int argc, char** argv)
{
    Entry **entries;
    int ignore_hidden = 0;
    int entry_count;
    // for now, we'll assume correct formating of command line arguments
    // the first argument is the flag and the second is the path
    // so argv[1] would be -i and argv[2] would be the path
    if (strcmp(argv[1], "-i") == 0) 
        ignore_hidden = 1;

    if (!is_valid_dir(argv[2])) {
        perror("Invalid directory\n");
        return 1;
    }

    // let's categorically deny directorie that start with two dots
    // having two dots introduces additional work when trying to obtain the cwd
    if (argv[2][0] == '.' && argv[2][1] == '.') {
        perror("Invalid directory\n");
        return 1;
    }

    if (strcmp(argv[2], ".") == 0) {
        // let's get the current working directory
        current_path = getcwd(current_path, PATH_MAX); 
    } else {
        current_path = strdup(argv[2]);
    }
    entries = get_dir_contents(current_path, ignore_hidden, &entry_count);

    // Actual GUI code

    initscr();
    clear();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // we need to determine the size of the terminal
    int width, height;
    // this macro is pretty neat
    getmaxyx(stdscr, height, width);

    // so the general approach is WIDTH - LONGEST_ENTRY / 2
    // in this case it's WIDTH - 30 / 2 (since we're using 30 as a constant for now)

    // int start_x = (width - longest_entry) / 2;
    start_y = (height - entry_count) / 2;
    start_x = (width - 30) / 2;

    // let's start drawing the entries
    draw_entries(entry_count, entries);
    refresh();

    int c;
    while((c = getch()) != 'q') {
        switch(c) {
            case FSEX_NAV_MODE_UP:
                if (highlighted > 0) {
                    highlighted--;
                } else {
                    highlighted = entry_count - 1;
                }
                break;
            case FSEX_NAV_MODE_DOWN:
                if (highlighted < entry_count - 1) {
                    highlighted++;
                } else {
                    highlighted = 0;
                }
                break;
            case FSEX_NAV_MODE_ENTER:
                if (entries[highlighted]->type == FT_DIR) {
                    char* new_path = get_new_path(entries[highlighted]->name);
                    free(current_path);
                    current_path = strdup(new_path);
                    free(new_path);
                    free_entries(entry_count, entries);
                    entries = get_dir_contents(current_path, ignore_hidden, &entry_count);
                    highlighted = 0;
                } else {
                    // open the file with the default program
                    // we'll do this later
                }

                break;
        }
        clear();
        draw_entries(entry_count, entries);
        refresh();
    }

    free_entries(entry_count, entries);
    endwin();
    printf("%s\n", current_path);
    return 0;
}
