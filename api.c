#include "api.h"

void _cd_(char foldername[]) {
    if ((strcmp(foldername, "..") == 0) && current_folder->parent) {
        pop_folder();
        current_folder = current_folder->parent;
    } else {
        folder_node *folder = is_folder_exist(foldername);
        if (folder) {
            push_folder(foldername);
            current_folder = folder;
        } else {
            printf("folder not exist!\n");
        }
    }
}

void _ls_() {
    list_folder();
    list_file();
}

void _mkdir_(char foldername[]) {
    folder_node *folder = is_folder_exist(foldername);
    if (folder) {
        printf("folder exist!\n");
        return;
    } else {
        create_folder(foldername);
        if (current_folder->child) {
            folder_node *last_folder = current_folder->child;
            while (last_folder->next_sibling) {
                last_folder = last_folder->next_sibling;
            }
            last_folder->next_sibling = create_folder_node(foldername);

        } else {
            current_folder->child = create_folder_node(foldername);
        }
        save_filesystem();
    }
}

void _rmdir_(char foldername[]) {
    folder_node *folder = is_folder_exist(foldername);
    if (!folder) {
        printf("folder not exist!\n");
    } else {
        remove_folder_node(folder);
        save_filesystem();
    }
}

void _touch_(char filename[]) {
    if (is_file_exist(filename)) {
        printf("filename exist!\n");
    } else {
        create_file(filename);
        if (!current_folder->file) {
            current_folder->file = create_file_node(filename, current_user, 7, 5);
        } else {
            file_node *last_file = current_folder->file;
            while (last_file->next_file) {
                last_file = last_file->next_file;
            }
            last_file->next_file = create_file_node(filename, current_user, 7, 5);
        }
        save_filesystem();
    }
}

void _rm_(char filename[]) {
    file_node *remove_file_node = is_file_exist(filename);
    if (!remove_file_node) {
        printf("file not exist!\n");
    } else {
        if (strcmp(filename, current_folder->file->filename)) { //文件头
            current_folder->file = remove_file_node->next_file;
        } else {
            remove_file_node->prev_file->next_file = remove_file_node->next_file;
        }
        remove_file(filename);
    }
}

void _chmod_(char filename[], int owner_mode, int other_mode);

FILE *_open_(char filename[]);

void _cat_(char filename[]);

void _write_(char filename[]);

void _excute_(char filename[]);

void _close_(FILE *f);

int main() {
    init_filesystem();
    current_folder = root;
    strcpy(current_dir, "./disk");
    printf("-----------------------------\n");
    _ls_();
    _rmdir_("s1");
    _ls_();
    printf("-----------------------------\n");
}