#ifndef MYSQL_INTEGRATION_H
#define MYSQL_INTEGRATION_H

#ifdef ENABLE_MYSQL

#include "models.h"
#include <mysql/mysql.h>

// MySQL Integration Module - Build with: make ENABLE_MYSQL=1

// Database connection functions
MYSQL* db_connect(void);
void db_disconnect(MYSQL *conn);

// Database table operations
int db_init_tables(MYSQL *conn);

// Parking operations
int db_add_active_car(MYSQL *conn, const ParkingActive *car);
int db_remove_active_car(MYSQL *conn, const char *car_number);
int db_add_history_car(MYSQL *conn, const ParkingHistory *car);

#endif // ENABLE_MYSQL

#endif // MYSQL_INTEGRATION_H

