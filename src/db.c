#ifdef ENABLE_MYSQL

#include "db.h"
#include <stdio.h>
#include <string.h>

/**
 * ============================================================================
 * SUBORNA AKTER - MySQL Integration Implementation
 * ============================================================================
 * This file implements all MySQL database operations.
 * Build with: make ENABLE_MYSQL=1
 * ============================================================================
 */

/**
 * Connect to MySQL database
 * 
 * @return MYSQL* connection handle on success, NULL on failure
 * 
 * @details
 * This function:
 * 1. Initializes MySQL connection structure
 * 2. Sets connection parameters (host, user, password, database)
 * 3. Attempts to connect to MySQL server
 * 4. Returns connection handle if successful, NULL if failed
 * 
 * @note
 * - Edit connection parameters below to match your MySQL setup
 * - Default: localhost, root user, no password, smart_parking database
 * - Port 3306 is MySQL default port
 * 
 * @error_handling
 * - Returns NULL if mysql_init() fails
 * - Returns NULL if mysql_real_connect() fails
 * - Connection is closed before returning NULL on failure
 */
MYSQL* db_connect(void) {
    // Step 1: Initialize MySQL connection structure
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        return NULL; // Failed to initialize
    }
    
    // Step 2: Configure connection parameters
    // TODO: Change these to match your MySQL setup
    const char *host = "localhost";      // MySQL server address
    const char *user = "root";           // MySQL username
    const char *password = "";           // MySQL password (empty = no password)
    const char *database = "smart_parking"; // Database name
    unsigned int port = 3306;            // MySQL port (default: 3306)
    
    // Step 3: Attempt to connect to MySQL server
    if (!mysql_real_connect(conn, host, user, password, database, port, NULL, 0)) {
        // Connection failed - cleanup and return NULL
        mysql_close(conn);
        return NULL;
    }
    
    // Step 4: Connection successful - return handle
    return conn;
}

/**
 * Disconnect from MySQL database
 * 
 * @param conn MySQL connection handle (can be NULL)
 * 
 * @details
 * - Safely closes MySQL connection
 * - Frees all resources associated with connection
 * - Safe to call with NULL pointer (does nothing)
 * 
 * @usage
 *   Always call this after finishing database operations:
 *   db_disconnect(conn);
 */
void db_disconnect(MYSQL *conn) {
    if (conn) {
        mysql_close(conn); // Close connection and free resources
    }
}

/**
 * Initialize database tables (create if they don't exist)
 * 
 * @param conn MySQL connection handle (must be valid)
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Creates 4 tables in the database:
 * 1. users - Stores user accounts (username, password, role)
 * 2. garages - Stores garage information
 * 3. parking_active - Stores currently parked cars
 * 4. parking_history - Stores completed parking transactions
 * 
 * Uses "CREATE TABLE IF NOT EXISTS" so it's safe to call multiple times.
 * 
 * @usage
 *   Call this once when application starts (in main.c):
 *   MYSQL *conn = db_connect();
 *   if (conn) {
 *       db_init_tables(conn);
 *   }
 */
int db_init_tables(MYSQL *conn) {
    // Array of SQL CREATE TABLE statements
    const char *queries[] = {
        // Table 1: Users table (for authentication)
        "CREATE TABLE IF NOT EXISTS users ("
        "username VARCHAR(50) PRIMARY KEY, "
        "password VARCHAR(50) NOT NULL, "
        "role VARCHAR(10) NOT NULL)",
        
        // Table 2: Garages table (garage information)
        "CREATE TABLE IF NOT EXISTS garages ("
        "id INT PRIMARY KEY, "
        "name VARCHAR(100) NOT NULL, "
        "total_capacity INT NOT NULL, "
        "available_slots INT NOT NULL, "
        "hourly_rate_car FLOAT NOT NULL, "
        "hourly_rate_bike FLOAT NOT NULL)",
        
        // Table 3: Active parking (currently parked cars)
        "CREATE TABLE IF NOT EXISTS parking_active ("
        "car_number VARCHAR(20) PRIMARY KEY, "
        "vehicle_type VARCHAR(10) NOT NULL, "
        "garage_id INT NOT NULL, "
        "duration_hours INT NOT NULL, "
        "entry_time DATETIME NOT NULL, "
        "bill_amount FLOAT NOT NULL)",
        
        // Table 4: Parking history (completed transactions)
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
    
    // Execute each CREATE TABLE query
    for (int i = 0; i < 4; i++) {
        if (mysql_query(conn, queries[i]) != 0) {
            // If any query fails, return failure
            return 0;
        }
    }
    
    // All tables created successfully
    return 1;
}

/**
 * Add a car to MySQL active parking table
 * 
 * @param conn MySQL connection handle (must be valid)
 * @param car Pointer to ParkingActive structure containing car data
 * @return 1 on success, 0 on failure
 * 
 * @details
 * This function:
 * 1. Converts entry_time from time_t to MySQL DATETIME format
 * 2. Builds INSERT SQL query with car data
 * 3. Uses ON DUPLICATE KEY UPDATE to handle duplicates (updates if car_number exists)
 * 4. Executes query and returns result
 * 
 * @usage
 * Called from ui.c when car is added:
 *   ParkingActive *car = &state->active_cars[index];
 *   db_add_active_car(conn, car);
 * 
 * @note
 * - This syncs data to MySQL after file save
 * - Called in on_add_car_clicked() function in ui.c
 */
int db_add_active_car(MYSQL *conn, const ParkingActive *car) {
    char query[500];           // SQL query buffer
    char entry_time[50];       // Formatted timestamp string
    
    // Step 1: Convert entry_time from time_t to MySQL DATETIME format
    // Format: "YYYY-MM-DD HH:MM:SS" (e.g., "2024-12-13 10:30:00")
    struct tm *tm_info = localtime(&car->entry_time);
    strftime(entry_time, sizeof(entry_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Step 2: Build INSERT query with ON DUPLICATE KEY UPDATE
    // This means: Insert new record, or update if car_number already exists
    snprintf(query, sizeof(query),
             "INSERT INTO parking_active (car_number, vehicle_type, garage_id, duration_hours, entry_time, bill_amount) "
             "VALUES ('%s', '%s', %d, %d, '%s', %.2f) "
             "ON DUPLICATE KEY UPDATE vehicle_type='%s', garage_id=%d, duration_hours=%d, entry_time='%s', bill_amount=%.2f",
             car->car_number,                                    // Car number (primary key)
             vehicle_type_to_string(car->vehicle_type),         // "Car" or "Bike"
             car->garage_id,                                    // Garage ID
             car->duration_hours,                               // Parking duration
             entry_time,                                         // Entry time (DATETIME format)
             car->bill_amount,                                  // Calculated bill amount
             // ON DUPLICATE KEY UPDATE values (same as INSERT values)
             vehicle_type_to_string(car->vehicle_type),
             car->garage_id,
             car->duration_hours,
             entry_time,
             car->bill_amount);
    
    // Step 3: Execute query and return success/failure
    return (mysql_query(conn, query) == 0);
}

/**
 * Remove a car from MySQL active parking table
 * 
 * @param conn MySQL connection handle (must be valid)
 * @param car_number Car number to remove (e.g., "ABC-1234")
 * @return 1 on success, 0 on failure
 * 
 * @details
 * - Deletes car record from parking_active table
 * - Called when car exits parking
 * - Car should already be moved to history before calling this
 * 
 * @usage
 * Called from ui.c when car exits:
 *   db_remove_active_car(conn, "ABC-1234");
 * 
 * @note
 * - This syncs deletion to MySQL after file operations
 * - Called in on_exit_car_clicked() function in ui.c
 */
int db_remove_active_car(MYSQL *conn, const char *car_number) {
    char query[200];
    
    // Build DELETE query: Remove car from active table
    snprintf(query, sizeof(query), "DELETE FROM parking_active WHERE car_number='%s'", car_number);
    
    // Execute query and return success/failure
    return (mysql_query(conn, query) == 0);
}

/**
 * Add a car to MySQL parking history table
 * 
 * @param conn MySQL connection handle (must be valid)
 * @param car Pointer to ParkingHistory structure containing completed transaction data
 * @return 1 on success, 0 on failure
 * 
 * @details
 * This function:
 * 1. Converts entry_time and exit_time to MySQL DATETIME format
 * 2. Inserts completed parking transaction into parking_history table
 * 3. Called when car exits parking (after moving from active to history)
 * 
 * @usage
 * Called from ui.c when car exits:
 *   ParkingHistory history = {...}; // Filled with car data
 *   db_add_history_car(conn, &history);
 * 
 * @note
 * - This syncs history data to MySQL after file operations
 * - Called in on_exit_car_clicked() function in ui.c
 * - History table has auto-increment ID, so no need to specify it
 */
int db_add_history_car(MYSQL *conn, const ParkingHistory *car) {
    char query[500];
    char entry_time[50], exit_time[50];  // Formatted timestamp strings
    struct tm *tm_info;
    
    // Step 1: Convert entry_time to MySQL DATETIME format
    tm_info = localtime(&car->entry_time);
    strftime(entry_time, sizeof(entry_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Step 2: Convert exit_time to MySQL DATETIME format
    tm_info = localtime(&car->exit_time);
    strftime(exit_time, sizeof(exit_time), "%Y-%m-%d %H:%M:%S", tm_info);
    
    // Step 3: Build INSERT query for parking_history table
    snprintf(query, sizeof(query),
             "INSERT INTO parking_history (car_number, vehicle_type, garage_id, duration_hours, entry_time, exit_time, bill_amount) "
             "VALUES ('%s', '%s', %d, %d, '%s', '%s', %.2f)",
             car->car_number,                           // Car number
             vehicle_type_to_string(car->vehicle_type), // "Car" or "Bike"
             car->garage_id,                            // Garage ID
             car->duration_hours,                       // Parking duration
             entry_time,                                // Entry time (DATETIME)
             exit_time,                                 // Exit time (DATETIME)
             car->bill_amount);                         // Final bill amount
    
    // Step 4: Execute query and return success/failure
    return (mysql_query(conn, query) == 0);
}

// REMOVED: db_load_users() and db_save_user() - not part of Suborna's tasks (authentication removed)

#endif // ENABLE_MYSQL

