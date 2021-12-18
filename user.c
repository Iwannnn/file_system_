#include "user.h"

int login(char username[], char password[]) {
    if (!strcmp(username, "")) {
        printf("alraedy login\n");
        return FAILURE;
    }
    if (find_user(username)) {
        if (check_password(username, password)) {
            strcpy(current_user, username);
            return SUCCESS;
        } else {
            printf("password error!\n");
            return FAILURE;
        }
    } else {
        printf("user does not exist!\n");
        return FAILURE;
    }
}

int reg(char username[], char password[]) {
    if (!find_user(username)) {
        FILE *user_info = NULL;
        user_info = fopen(USERINFO_DIR, USERINFO_MODE);
        fprintf(user_info, "%s %s\n", username, password);
        fclose(user_info);
        printf("register success\n");
        return SUCCESS;
    }
    printf("user already exist!\n");
    return FAILURE;
}

int logout() {
    if (strcmp(current_user, "")) {
        strcpy(current_user, "");
        return SUCCESS;
    }
    printf("user is not logged in!\n");
    return FAILURE;
}

int find_user(char username[]) {
    char username_[NAME_MAX], password_[NAME_MAX];
    int t = 0, tt = 0;
    FILE *user_info = fopen(USERINFO_DIR, USERINFO_MODE);
    while (fscanf(user_info, "%s %s", username_, password_) != EOF) {
        if (strcmp(username_, username) == 0) {
            return SUCCESS;
        }
    }
    fclose(user_info);
    return FAILURE;
}

int check_password(char username[], char password[]) {
    char username_[NAME_MAX], password_[NAME_MAX];
    FILE *user_info = NULL;
    user_info = fopen(USERINFO_DIR, USERINFO_MODE);
    while (fscanf(user_info, "%s %s", username_, password_) != EOF) {
        if (strcmp(username, username_) == 0) {
            if (strcmp(password, password_) == 0) {
                return SUCCESS;
            } else {
                return FAILURE;
            }
        }
    }
    fclose(user_info);
    return FAILURE;
}
