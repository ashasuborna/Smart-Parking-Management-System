#ifndef DB_H
#define DB_H

#ifdef ENABLE_MYSQL

#include "models.h"
#include <mysql/mysql.h>

// Database connection
MYSQL* db_connect(void);
void db_disconnect(MYSQL *conn);

// Database operations
int db_init_tables(MYSQL *conn);
int db_add_active_car(MYSQL *conn, const ParkingActive *car);
int db_remove_active_car(MYSQL *conn, const char *car_number);
int db_add_history_car(MYSQL *conn, const ParkingHistory *car);
int db_load_users(MYSQL *conn, User *users, int *count);
int db_save_user(MYSQL *conn, const User *user);

#endif // ENABLE_MYSQL

#endif // DB_H

