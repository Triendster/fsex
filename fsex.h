#ifndef FSEX_H
#define FSEX_H

#include <ncurses.h>

// Custom type definitions
typedef enum {
    FT_DIR,
    FT_FILE,
} EntryType;


typedef struct {
    char* name;
    EntryType type;
} Entry;

// Function prototypes

// Returns the current directory entries
Entry** get_dir_contents(const char *path, int ignore_hidden, int *entry_count);

// If it's a valid path, opendir should not return NULL
int is_valid_dir(const char *path);

// free the entries once they're no longer in use
void free_entries(int entry_count, Entry** entries);

// gets the new path based on the target and the current path
char* get_new_path(const char *target);
#endif
