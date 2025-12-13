#include "auth.h"
#include "fileio.h"
#include <string.h>
#include <stdio.h>

int authenticate_user(const char *username, const char *password, User *user) {
    User users[100];
    int count = 0;
    
    if (!load_users(users, &count)) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            *user = users[i];
            return 1;
        }
    }
    
    return 0;
}

int user_exists(const char *username) {
    User users[100];
    int count = 0;
    
    if (!load_users(users, &count)) {
        return 0;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 1;
        }
    }
    
    return 0;
}

int register_user(const char *username, const char *password, UserRole role) {
    if (user_exists(username)) {
        return 0; // User already exists
    }
    
    User new_user;
    strncpy(new_user.username, username, MAX_USERNAME_LEN - 1);
    strncpy(new_user.password, password, MAX_PASSWORD_LEN - 1);
    new_user.role = role;
    
    return save_user(&new_user);
}

