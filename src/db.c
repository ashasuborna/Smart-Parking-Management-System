#ifdef ENABLE_MYSQL

#include "db.h"
#include <stdio.h>
#include <string.h>

MYSQL* db_connect(void) {
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        return NULL;
    }
    
    // Configure these according to your MySQL setup
    const char *host = "localhost";
    const char *user = "root";
    const char *password = "";
    const char *database = "smart_parking";
    unsigned int port = 3306;
    
    if (!mysql_real_connect(conn, host, user, password, database, port, NULL, 0)) {
        mysql_close(conn);
        return NULL;
    }
    
    return conn;
}

void db_disconnect(MYSQL *conn) {
    if (conn) {
        mysql_close(conn);
    }
}

int db_init_tables(MYSQL *conn) {
    const char *queries[] = {
        "CREATE TABLE IF NOT EXISTS users ("
        "username VARCHAR(50) PRIMARY KEY, "
        "password VARCHAR(50) NOT NULL, "
        "role VARCHAR(10) NOT NULL)",
        
        "CREATE TABLE IF NOT EXISTS garages ("
        "id INT PRIMARY KEY, "
        "name VARCHAR(100) NOT NULL, "
        "total_capacity INT NOT NULL, "
        "available_slots INT NOT NULL, "
        "hourly_rate_car FLOAT NOT NULL, "
        "hourly_rate_bike FLOAT NOT NULL)",
        
        "CREATE TABLE IF NOT EXISTS parking_active ("
        "car_number VARCHAR(20) PRIMARY KEY, "
        "vehicle_type VARCHAR(10) NOT NULL, "
        "garage_id INT NOT NULL, "
        "duration_hours INT NOT NULL, "
        "entry_time DATETIME NOT NULL, "
        "bill_amount FLOAT NOT NULL)",
        
        "CREATE TABLE IF NOT EXISTS parking_history ("
        "id INT AUTO_INCREMENT PRIMARY KEY, "
        "car_number VARCHAR(20) NOT NULL, "
        "vehicle_type VARCHAR(10) NOT NULL, "
        "garage_id INT NOT NULL, "
        "duration_hours INT NOT NULL, "
        "entry_time DATETIME NOT NULL, "
        "exit_time DATETIME NOT NULL, "
        "bill_amount FLOAT NOT NULL)"
    };
    
    for (int i = 0; i < 4; i++) {
        if (mysql_query(conn, queries[i]) != 0) {
            return 0;
        }
    }
    
    return 1;
}

int db_add_active_car(MYSQL *conn, const ParkingActive *car) {
    char query[500];
    char entry_time[50];
    struct tm *tm_info = localtime(&car->entry_time);
    strftime(entry_time, sizeof(entry_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    snprintf(query, sizeof(query),
             "INSERT INTO parking_active (car_number, vehicle_type, garage_id, duration_hours, entry_time, bill_amount) "
             "VALUES ('%s', '%s', %d, %d, '%s', %.2f) "
             "ON DUPLICATE KEY UPDATE vehicle_type='%s', garage_id=%d, duration_hours=%d, entry_time='%s', bill_amount=%.2f",
             car->car_number, vehicle_type_to_string(car->vehicle_type), car->garage_id,
             car->duration_hours, entry_time, car->bill_amount,
             vehicle_type_to_string(car->vehicle_type), car->garage_id,
             car->duration_hours, entry_time, car->bill_amount);
    
    return (mysql_query(conn, query) == 0);
}

int db_remove_active_car(MYSQL *conn, const char *car_number) {
    char query[200];
    snprintf(query, sizeof(query), "DELETE FROM parking_active WHERE car_number='%s'", car_number);
    return (mysql_query(conn, query) == 0);
}

int db_add_history_car(MYSQL *conn, const ParkingHistory *car) {
    char query[500];
    char entry_time[50], exit_time[50];
    struct tm *tm_info;
    
    tm_info = localtime(&car->entry_time);
    strftime(entry_time, sizeof(entry_time), "%Y-%m-%d %H:%M:%S", tm_info);
    tm_info = localtime(&car->exit_time);
    strftime(exit_time, sizeof(exit_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    snprintf(query, sizeof(query),
             "INSERT INTO parking_history (car_number, vehicle_type, garage_id, duration_hours, entry_time, exit_time, bill_amount) "
             "VALUES ('%s', '%s', %d, %d, '%s', '%s', %.2f)",
             car->car_number, vehicle_type_to_string(car->vehicle_type), car->garage_id,
             car->duration_hours, entry_time, exit_time, car->bill_amount);
    
    return (mysql_query(conn, query) == 0);
}

int db_load_users(MYSQL *conn, User *users, int *count) {
    const char *query = "SELECT username, password, role FROM users";
    if (mysql_query(conn, query) != 0) {
        *count = 0;
        return 0;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        *count = 0;
        return 0;
    }
    
    *count = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) && *count < 100) {
        strncpy(users[*count].username, row[0], MAX_USERNAME_LEN - 1);
        strncpy(users[*count].password, row[1], MAX_PASSWORD_LEN - 1);
        users[*count].role = (strcmp(row[2], "admin") == 0) ? ROLE_ADMIN : ROLE_USER;
        (*count)++;
    }
    
    mysql_free_result(result);
    return 1;
}

int db_save_user(MYSQL *conn, const User *user) {
    char query[300];
    snprintf(query, sizeof(query),
             "INSERT INTO users (username, password, role) VALUES ('%s', '%s', '%s') "
             "ON DUPLICATE KEY UPDATE password='%s', role='%s'",
             user->username, user->password, role_to_string(user->role),
             user->password, role_to_string(user->role));
    
    return (mysql_query(conn, query) == 0);
}

#endif // ENABLE_MYSQL

