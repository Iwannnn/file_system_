#ifndef FILE_H
#define FILE_H
#include "global.h"

typedef struct FILENODE {
    char filename[NAME_MAX];
    char username[NAME_MAX];
    // read write excute 4 2 1
    int owner_mode;
    int other_mode;
    struct FILENODE *next_file;
    struct FILENODE *prev_file;
} file_node;

typedef struct FOLDERNODE {
    char foldername[NAME_MAX];
    file_node *file;
    struct FOLDERNODE *child;
    struct FOLDERNODE *parent;
    struct FOLDERNODE *next_sibling;
    struct FOLDERNODE *prev_sibling;
} folder_node;

extern folder_node *root;
extern folder_node *current_folder;

void init_filesystem();

void save_filesystem();

folder_node *init_folder_tree(FILE *file_info, char *op, folder_node *parent);

file_node *init_file_link_node(FILE *file_info);

void LDR(FILE *file_info, folder_node *node);

void traverse_folder(FILE *file_info, file_node *file);

folder_node *create_folder_node(char foldername[]);

void create_folder(char foldername[]);

file_node *create_file_node(char filename[], char username[], int user_mode, int other_mode);

void create_file(char filename[]);

void push_folder(char foldername[]);

void pop_folder();

folder_node *is_folder_exist(char foldername[]);

file_node *is_file_exist(char filename[]);

void remove_folder_node(folder_node *folder);

void remove_folder(folder_node *folder);

void free_folder_node(folder_node *folder);

void remove_file_node(file_node *file);

void remove_file(char filename[]);

void remove_all_files(file_node *file);

void free_file_node(file_node *file);

void list_folder();

void list_file();

void change_file_mode(file_node *file, int owner_mode, int other_mode);

void read_file(char filename[]);

void write_file(char filename[]);

#endif