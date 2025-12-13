#ifndef AUTH_H
#define AUTH_H

#include "models.h"

// Authentication functions
int authenticate_user(const char *username, const char *password, User *user);
int user_exists(const char *username);
int register_user(const char *username, const char *password, UserRole role);

#endif // AUTH_H

