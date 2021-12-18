#ifndef SHELL_H
#define SHELL_H
#include "api.h"
enum COMMAND {
    null_,
    cd_,
    ls_,
    mkdir_,
    rmdir_,
    touch_,
    rm_,
    chmod_,
    tree,
    open_,
    cat_,
    write_,
    excute_,
    login_,
    logout_,
    reg_,
    exit_,
    help_
};
extern char command_to_string[][10];

int get_command(char string[]);

int check_permission(char filename[], enum COMMAND command, char permission);

int rwx_permission(enum COMMAND command, int mode);

void start();

int check_line();

#endif