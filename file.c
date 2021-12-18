#include "file.h"

folder_node *root;
folder_node *current_folder;

void init_filesystem() {
    FILE *file_info = fopen(FILEINFO_DIR, FILEINFO_READ_MODE);
    char op[20];
    fscanf(file_info, "%s", op);
    if (strcmp(op, FOLDER_) == 0) {
        root = init_folder_tree(file_info, op, NULL);
    }
    fclose(file_info);
}

folder_node *init_folder_tree(FILE *file_info, char *op, folder_node *parent) {
    folder_node *node = (folder_node *)malloc(sizeof(folder_node));
    char foldername[NAME_MAX];
    fscanf(file_info, "%s", foldername);
    // printf("FOLDER %s\n", foldername);
    node = create_folder_node(foldername);
    node->parent = parent;
    current_folder = node;
    // printf("current: %s\n", current_folder->foldername);
    push_folder(foldername);
    fscanf(file_info, "%s", op);
    if (strcmp(op, FILE_) == 0) {
        file_node *file = NULL, *next = NULL;
        do {
            if (!file) {
                // printf("first\t");
                file = init_file_link_node(file_info);
                node->file = file;
            } else {
                // printf("next\t");
                next = init_file_link_node(file_info);
                file->next_file = next;
                next->prev_file = file;
                file = file->next_file;
            }
            fscanf(file_info, "%s", op);
        } while (strcmp(op, FILE_) == 0);
    }
    if (strcmp(op, FOLDER_) == 0) {
        node->child = init_folder_tree(file_info, op, node);
        node->child->parent = node;
        // printf("%s parent %s\n", node->child->foldername, node->child->parent->foldername);
        // printf("current->child->parent  %s\n", node->child->parent->foldername);
    } else {
        // printf("EMPTY\n");
    }
    pop_folder();
    fscanf(file_info, "%s", op);
    if (strcmp(op, FOLDER_) == 0) {
        node->next_sibling = init_folder_tree(file_info, op, parent);
        node->next_sibling->prev_sibling = node;
        node->next_sibling->parent = node->parent;
        // printf("current->next_sibling->prev_sibling  %s\n", node->next_sibling->prev_sibling->foldername);
    } else {
        // printf("EMPTY\n");
    }
    return node;
}

file_node *init_file_link_node(FILE *file_info) {
    file_node *node = (file_node *)malloc(sizeof(file_node));
    char filename[NAME_MAX];
    char username[NAME_MAX];
    int owner_mode;
    int other_mode;
    fscanf(file_info, "%s %s %d %d", filename, username, &owner_mode, &other_mode);
    // printf("FILE %s %s %d %d\n", filename, username, owner_mode, other_mode);
    node = create_file_node(filename, username, owner_mode, other_mode);
    return node;
}

void save_filesystem() {
    FILE *file_info = fopen(FILEINFO_DIR, FILEINFO_SAVE_MODE);
    LDR(file_info, root);
    fclose(file_info);
}

void LDR(FILE *file_info, folder_node *node) {
    fprintf(file_info, "FOLDER ");
    fprintf(file_info, "%s\n", node->foldername);
    // printf("FOLDER %s\n", node->foldername);
    if (node->file) traverse_folder(file_info, node->file);
    if (node->child) {
        LDR(file_info, node->child);
    } else {
        fprintf(file_info, "EMPTY\n");
        // printf("EMPTY\n");
    }
    if (node->next_sibling) {
        LDR(file_info, node->next_sibling);
    } else {
        fprintf(file_info, "EMPTY\n");
        // printf("EMPTY\n");
    }
}

void traverse_folder(FILE *file_info, file_node *node) {
    fprintf(file_info, "FILE ");
    fprintf(file_info, "%s %s %d %d\n", node->filename, node->username, node->owner_mode, node->other_mode);
    // printf("FILE %s %s %d %d\n", node->filename, node->username, node->owner_mode, node->other_mode);
    if (node->next_file) {
        traverse_folder(file_info, node->next_file);
    }
}

folder_node *create_folder_node(char foldername[]) {
    folder_node *folder = (folder_node *)malloc(sizeof(folder_node));
    strcpy(folder->foldername, foldername);
    folder->file = NULL;
    folder->child = NULL;
    folder->next_sibling = NULL;
    create_folder(foldername);
    return folder;
}

void create_folder(char foldername[]) {
    char tmp[PATH_MAX] = "";
    strcat(tmp, current_dir);
    strcat(tmp, "/");
    strcat(tmp, foldername);
    mkdir(tmp, 0775);
}

file_node *create_file_node(char filename[], char username[], int owner_mode, int other_mode) {
    file_node *file = (file_node *)malloc(sizeof(file_node));
    strcpy(file->filename, filename);
    strcpy(file->username, username);
    file->owner_mode = owner_mode;
    file->other_mode = other_mode;
    file->next_file = NULL;
    create_file(filename);
    return file;
}

void create_file(char filename[]) {
    char tmp[PATH_MAX] = "";
    strcat(tmp, current_dir);
    strcat(tmp, "/");
    strcat(tmp, filename);
    // printf("file:%s\n", tmp);
    open(tmp, O_CREAT | O_RDWR, 0775);
}

void push_folder(char foldername[]) {
    strcat(current_dir, SLASH);
    strcat(current_dir, foldername);
}

void pop_folder() {
    char tmp[PATH_MAX] = "";
    int last_slash_index = 0;
    for (int i = strlen(current_dir) - 1; i >= 0; i--) {
        if (current_dir[i] == '/') {
            last_slash_index = i;
            break;
        }
    }
    for (int i = 0; i < last_slash_index; i++) {
        tmp[i] = current_dir[i];
    }
    strcpy(current_dir, tmp);
    // printf("%s\n", current_dir);
}

folder_node *is_folder_exist(char foldername[]) {
    folder_node *child = current_folder->child;
    while (child) {
        if (strcmp(foldername, child->foldername) == 0) {
            return child;
        }
        child = child->next_sibling;
    }
    return NULL;
}

file_node *is_file_exist(char filename[]) {
    file_node *file = current_folder->file;
    while (file) {
        if ((strcmp(filename, file->filename) == 0)) {
            return file;
        }
        file = file->next_file;
    }
    return NULL;
}

void remove_folder_node(folder_node *folder) {
    //独立该节点
    folder->parent->child = folder->next_sibling;
    if (folder->prev_sibling) {
        folder->prev_sibling->next_sibling = folder->next_sibling;
    }
    folder->next_sibling = NULL;
    folder->prev_sibling = NULL;
    folder->parent = NULL;

    //递归删除路径
    remove_folder(folder);
    //递归释放节点
    free_folder_node(folder);
}

void remove_folder(folder_node *folder) {
    //进入路径
    push_folder(folder->foldername);
    if (folder->child) {
        remove_folder(folder->child);
    }
    if (folder->next_sibling) {
        //进入兄弟路径
        pop_folder();
        remove_folder(folder->next_sibling);
        push_folder(folder->foldername);
    }
    //删除当前文件夹下的文件
    if (folder->file) {
        remove_all_files(folder->file);
    }
    //删除当前文件夹
    rmdir(current_dir);
    //恢复路径
    pop_folder();
}

void free_folder_node(folder_node *folder) {
    if (folder->child) {
        free_folder_node(folder->child);
    }
    if (folder->next_sibling) {
        free_folder_node(folder->next_sibling);
    }
    if (folder->file) {
        remove_all_files(folder->file);
    }
    free(folder);
}

void remove_file_node(file_node *file) {
    if (file->prev_file) {
        file->prev_file->next_file = file->next_file;
    }
    free_file_node(file);
}

void remove_all_files(file_node *file) {
    while (file) {
        printf("file_name:%s\n", file->filename);
        remove_file(file->filename);
        file = file->next_file;
    }
}

void remove_file(char filename[]) {
    char tmp[PATH_MAX] = "";
    strcat(tmp, current_dir);
    strcat(tmp, SLASH);
    strcat(tmp, filename);
    printf("%s %d\n", tmp, remove(tmp));
}

void free_file_node(file_node *file) {
    if (file->next_file) {
        free_file_node(file->next_file);
    }
    free(file);
}

void list_folder() {
    folder_node *child = current_folder->child;
    PRINT_FONT_CYA
    printf("folder:");
    if (child) {
        printf("%s", child->foldername);
        while (child->next_sibling) {
            child = child->next_sibling;
            printf(" %s", child->foldername);
        }
    }
    PRINT_FONT_WHI
    printf("\n");
}

void list_file() {
    file_node *file = current_folder->file;
    PRINT_FONT_PUR
    printf("file:");
    if (file) {
        printf("%s", file->filename);
        while (file->next_file) {
            file = file->next_file;
            printf(" %s", file->filename);
        }
    }
    PRINT_FONT_WHI
    printf("\n");
}

void change_file_mode(file_node *file, int owner_mode, int other_mode) {
    file->owner_mode = owner_mode;
    file->other_mode = other_mode;
    save_filesystem();
}

void print_tree(folder_node *node, int tab) {
    if (strcmp(node->foldername, current_folder->foldername)) {
        print_format(node->foldername, tab);
    }
    if (node->child) {
        print_tree(node->child, tab + 1);
    }
    if (node->file) {
        print_link(node->file, tab + 1);
    }
    if (strcmp(node->foldername, current_folder->foldername)) {
        if (node->next_sibling) {
            print_tree(node->next_sibling, tab);
        }
    }
}
void print_link(file_node *node, int tab) {
    print_format(node->filename, tab);
    while (node->next_file) {
        node = node->next_file;
        print_format(node->filename, tab);
    }
}

void print_format(char str[], int tab) {
    printf("│");
    for (int i = 1; i < tab; i++) {
        printf("   │");
    }
    printf("── %s\n", str);
}

void read_file(char filename[]);

void write_file(char filename[]);

// void test() {
//     FILE *file_info = fopen(FILEINFO_DIR, FILEINFO_MODE);
//     char s[20], c;
//     while (~fscanf(file_info, "%c", &c)) {
//         printf("%c\n", c);
//         if (c == '.') {
//             fscanf(file_info, "%s", s);
//             printf("%s", s);
//         }
//     }
//     fclose(file_info);
// }

// int main() {
//     init_filesystem();
//     printf("-----------------------------\n");
//     save_filesystem();
// }