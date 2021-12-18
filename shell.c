#include "shell.h"
int get_command(char string[]) {
    for (int i = 0; i < sizeof(command_to_string) / sizeof(command_to_string[0]); i++) {
        if ((strcmp(string, command_to_string[i]) == 0)) {
            return i;
        }
    }
    PRINT_FONT_RED
    COMMAND_ERROR
    PRINT_FONT_WHI
    return FAILURE;
}

int check_permission(char filename[], enum COMMAND command, char permission) {
    file_node *file = is_file_exist(filename);
    if (!file) {
        PRINT_FONT_RED
        FILE_NOT_EXIST
        PRINT_FONT_WHI
        return FAILURE;
    } else {
        if (permission == '#') {
            return SUCCESS;
        }
        if ((strcmp(file->username, current_user) == 0)) { // owner
            if (command == cat_ || command == write_ || command == excute_) {
                if (rwx_permission(command, file->owner_mode)) {
                    return SUCCESS;
                } else {
                    PRINT_FONT_RED
                    PERMISSION_DENIED
                    PRINT_FONT_WHI
                    return FAILURE;
                }
            }
        } else { // other
            if (command == cat_ || command == write_ || command == excute_) {
                if (rwx_permission(command, file->other_mode)) {
                    return SUCCESS;
                } else {
                    PRINT_FONT_RED
                    PERMISSION_DENIED
                    PRINT_FONT_WHI
                    return FAILURE;
                }
            }
            if (command == rm_ || command == chmod_) {
                PRINT_FONT_RED
                PERMISSION_DENIED
                PRINT_FONT_WHI
                return FAILURE;
            }
        }
    }
    return FAILURE;
}

int rwx_permission(enum COMMAND command, int mode) {
    if (command == cat_) {
        if (mode == 4 || mode == 5 || mode == 6 || mode == 7) {
            return SUCCESS;
        } else {
            return FAILURE;
        }
    } else if (command == write_) {
        if (mode == 2 || mode == 3 || mode == 6 || mode == 7) {
            return SUCCESS;
        } else {
            return FAILURE;
        }
    } else if (command == excute_) {
        if (mode == 1 || mode == 3 || mode == 5 || mode == 7) {
            return SUCCESS;
        } else {
            return FAILURE;
        }
    }
    return FAILURE;
}

void start() {
    while (1) {
        char permission = (strcmp(current_user, "root") == 0) ? '#' : '$';
        char command[] = "";
        FILE *file = NULL;
        enum COMMAND command_ = null_;
        PRINT_FONT_GRE
        printf("%s@local:", current_user);
        PRINT_FONT_BLU
        printf("%s", current_dir);
        PRINT_FONT_WHI
        printf("%c ", permission);
        scanf("%s", command);
        command_ = get_command(command);
        // printf("%s\n", command_to_string[command_]);
        // printf("%d\n", command_);
        switch (command_) {
        case null_:
            break;
        case cd_: {
            char filename[] = "";
            scanf("%s", filename);
            _cd_(filename);
            break;
        }
        case ls_: {
            _ls_();
            break;
        }
        case mkdir_: {
            char foldername[] = "";
            scanf("%s", foldername);
            _mkdir_(foldername);
            break;
        }
        case rmdir_: {
            char foldername[] = "";
            scanf("%s", foldername);
            _rmdir_(foldername);
            break;
        }
        case touch_: {
            char filename[] = "";
            scanf("%s", filename);
            _touch_(filename);
            break;
        }
        case rm_: {
            char filename[] = "";
            scanf("%s", filename);
            if (!check_permission(filename, rm_, permission)) break;
            _touch_(filename);
            break;
        }
        case chmod_: {
            char filename[] = "";
            int owner_mode = 0, other_mode = 0;
            scanf("%s %d %d", filename, &owner_mode, &other_mode);
            if (!check_permission(filename, chmod_, permission)) break;
            _chmod_(filename, owner_mode, other_mode);
            break;
        }
        case tree: {
            _tree_();
            break;
        }
        case open_: {
            char filename[] = "";
            scanf("%s", filename);
            if (!check_permission(filename, open_, permission)) break;
            file = _open_(filename);
            break;
        }
        case cat_: {
            char filename[] = "";
            scanf("%s", filename);
            if (!check_permission(filename, cat_, permission)) break;
            _cat_(filename);
            break;
        }
        case write_: {
            char filename[] = "";
            scanf("%s", filename);
            if (!check_permission(filename, write_, permission)) break;
            _write_(filename);
            break;
        }
        case excute_: {
            char filename[] = "";
            scanf("%s", filename);
            if (!check_permission(filename, excute_, permission)) break;
            _excute_(filename);
            break;
        }
        case close_: {
            _close_(file);
            break;
        }
        case login_: {
            char username[NAME_MAX] = "", password[NAME_MAX] = "";
            scanf("%s %s", username, password);
            // printf("%s %s\n", username, password);
            _login_(username, password);
            break;
        }
        case logout_: {
            _logout_();
            break;
        }
        case reg_: {
            char username[NAME_MAX] = "", password[NAME_MAX] = "";
            scanf("%s %s", username, password);
            // printf("%s %s\n", username, password);
            _reg_(username, password);
            break;
        }
        case exit_: {
            PRINT_FONT_YEL
            printf("thanks for using!\n");
            PRINT_FONT_WHI
            exit(0);
            break;
        }
        case help_: {
            _help_();
            break;
        }
        default:
            break;
        }
    }
}

int main() {
    init_filesystem();
    current_folder = root;
    strcpy(current_dir, "./disk");
    start();
}