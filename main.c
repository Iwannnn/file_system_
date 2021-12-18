#include "shell.h"
int main() {
    init_filesystem();
    current_folder = root;
    strcpy(current_dir, "./disk");
    start();
}