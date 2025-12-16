#ifdef ENABLE_MYSQL

#include "mysql_integration.h"
#include <stdio.h>
#include <string.h>

// Connect to MySQL database, returns connection handle or NULL on failure
MYSQL* db_connect(void) {
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        return NULL;
    }
    
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

// Disconnect from MySQL database
void db_disconnect(MYSQL *conn) {
    if (conn) {
        mysql_close(conn);
    }
}

// Create database tables if they don't exist
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

// Add car to MySQL active parking table
int db_add_active_car(MYSQL *conn, const ParkingActive *car) {
    char query[500];
    char entry_time[50];
    
    struct tm *tm_info = localtime(&car->entry_time);
    strftime(entry_time, sizeof(entry_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    snprintf(query, sizeof(query),
             "INSERT INTO parking_active (car_number, vehicle_type, garage_id, duration_hours, entry_time, bill_amount) "
             "VALUES ('%s', '%s', %d, %d, '%s', %.2f) "
             "ON DUPLICATE KEY UPDATE vehicle_type='%s', garage_id=%d, duration_hours=%d, entry_time='%s', bill_amount=%.2f",
             car->car_number,
             vehicle_type_to_string(car->vehicle_type),
             car->garage_id,
             car->duration_hours,
             entry_time,
             car->bill_amount,
             vehicle_type_to_string(car->vehicle_type),
             car->garage_id,
             car->duration_hours,
             entry_time,
             car->bill_amount);
    
    return (mysql_query(conn, query) == 0);
}

// Remove car from MySQL active parking table
int db_remove_active_car(MYSQL *conn, const char *car_number) {
    char query[200];
    snprintf(query, sizeof(query), "DELETE FROM parking_active WHERE car_number='%s'", car_number);
    return (mysql_query(conn, query) == 0);
}

// Add car to MySQL parking history table
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
             car->car_number,
             vehicle_type_to_string(car->vehicle_type),
             car->garage_id,
             car->duration_hours,
             entry_time,
             exit_time,
             car->bill_amount);
    
    return (mysql_query(conn, query) == 0);
}

#endif // ENABLE_MYSQL

