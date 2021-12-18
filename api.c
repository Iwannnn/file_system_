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
            PRINT_FONT_RED
            FOLDER_NOT_EXIST
            PRINT_FONT_WHI
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
        PRINT_FONT_RED
        FOLDER_ALREADY_EXIST
        PRINT_FONT_WHI
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
        PRINT_FONT_RED
        FOLDER_NOT_EXIST
        PRINT_FONT_WHI
    } else {
        remove_folder_node(folder);
        save_filesystem();
    }
}

void _touch_(char filename[]) {
    // strcpy(current_user, "test");
    if (is_file_exist(filename)) {
        PRINT_FONT_RED
        FILE_ALREADY_EXIST
        PRINT_FONT_WHI
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
    file_node *file = is_file_exist(filename);
    if (!file) {
        PRINT_FONT_RED
        FILE_NOT_EXIST
        PRINT_FONT_WHI
    } else {
        if ((strcmp(filename, current_folder->file->filename) == 0)) { //文件头
            current_folder->file = file->next_file;
        } else {
            file->prev_file->next_file = file->next_file;
        }
        remove_file(filename);
        save_filesystem();
    }
}

void _chmod_(char filename[], int owner_mode, int other_mode) {
    file_node *file = is_file_exist(filename);
    strcpy(current_user, file->username);
    if (!file) {
        PRINT_FONT_RED
        FILE_NOT_EXIST
        PRINT_FONT_WHI
    } else {
        if (strcmp(current_user, file->username)) {
            PRINT_FONT_RED
            PERMISSION_DENIED
            PRINT_FONT_WHI
        } else {
            change_file_mode(file, other_mode, other_mode);
        }
    }
}

void _tree_() {
    printf("%s\n", current_folder->foldername);
    print_tree(current_folder, 0);
}

FILE *_open_(char filename[]) {
    file_node *file = is_file_exist(filename);
    char tmp[PATH_MAX] = "";
    if (file == NULL) {
        PRINT_FONT_RED
        FILE_NOT_EXIST
        PRINT_FONT_WHI
        return NULL;
    }
    strcat(tmp, current_dir);
    strcat(tmp, SLASH);
    strcat(tmp, filename);
    PRINT_FONT_YEL
    printf("open file :%s\n", tmp);
    PRINT_FONT_WHI
    return fopen(tmp, FILEINFO_READ_MODE);
}

void _cat_(char filename[]) {
    char line[100] = "";
    FILE *file = _open_(filename);
    if (file) {
        while (~fscanf(file, "%s", line)) {
            printf("%s\n", line);
        }
        _close_(file);
    }
}

void _write_(char filename[]) {
    FILE *file = _open_(filename);
    if (file) {
        PRINT_FONT_YEL
        THERE_IS_NOTHING
        PRINT_FONT_WHI
        _close_(file);
    }
}

void _excute_(char filename[]) {
    FILE *file = _open_(filename);
    if (file) {
        PRINT_FONT_YEL
        THERE_IS_NOTHING
        PRINT_FONT_WHI
        _close_(file);
    }
}

void _close_(FILE *f) {
    if (!f) {
        PRINT_FONT_RED
        printf("file not open!\n");
        PRINT_FONT_WHI
        return;
    }
    PRINT_FONT_YEL
    printf("close file\n");
    PRINT_FONT_WHI
    fclose(f);
}

void _login_(char username[], char password[]) {
    login(username, password);
}

void _logout_() {
    logout();
}

void _reg_(char username[], char password[]) {
    reg(username, password);
}

void _help_() {
    printf("there is nothing\n");
}

// int main() {
//     init_filesystem();
//     current_folder = root;
//     strcpy(current_dir, "./disk");
//     printf("-----------------------------\n");
//     _ls_();
//     FILE *file = _open_("a");
//     _cat_("a");
//     _write_("a");
//     _excute_("a");
//     _close_(file);
//      _tree_();
//      _ls_();
//     printf("-----------------------------\n");
//     save_filesystem();
// }