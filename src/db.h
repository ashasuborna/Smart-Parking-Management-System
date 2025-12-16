#ifndef DB_H
#define DB_H

#ifdef ENABLE_MYSQL

#include "models.h"
#include <mysql/mysql.h>

/**
 * ============================================================================
 * SUBORNA AKTER - MySQL Integration Module
 * ============================================================================
 * This module handles all MySQL database operations for the parking system.
 * It provides functions to connect, manage tables, and sync data with MySQL.
 * 
 * NOTE: This code only compiles when ENABLE_MYSQL flag is set during build.
 * ============================================================================
 */

// ============================================================================
// DATABASE CONNECTION FUNCTIONS
// ============================================================================

/**
 * Connect to MySQL database
 * 
 * @return MYSQL* connection handle on success, NULL on failure
 * 
 * @details
 * - Initializes MySQL connection
 * - Connects to database using configured parameters
 * - Returns NULL if connection fails
 * 
 * @usage
 *   MYSQL *conn = db_connect();
 *   if (conn) {
 *       // Use connection
 *       db_disconnect(conn);
 *   }
 */
MYSQL* db_connect(void);

/**
 * Disconnect from MySQL database
 * 
 * @param conn MySQL connection handle (can be NULL)
 * 
 * @details
 * - Closes the MySQL connection
 * - Frees resources
 * - Safe to call with NULL pointer
 */
void db_disconnect(MYSQL *conn);

// ============================================================================
// DATABASE TABLE OPERATIONS
// ============================================================================

/**
 * Initialize database tables (create if they don't exist)
 * 
 * @param conn MySQL connection handle
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Creates the following tables:
 * - users: User accounts (username, password, role)
 * - garages: Garage information
 * - parking_active: Currently parked cars
 * - parking_history: Parking transaction history
 * 
 * @usage
 *   MYSQL *conn = db_connect();
 *   if (conn) {
 *       db_init_tables(conn);
 *   }
 */
int db_init_tables(MYSQL *conn);

// ============================================================================
// PARKING OPERATIONS (YOUR MAIN FUNCTIONS)
// ============================================================================

/**
 * Add a car to MySQL active parking table
 * 
 * @param conn MySQL connection handle
 * @param car Pointer to ParkingActive structure with car data
 * @return 1 on success, 0 on failure
 * 
 * @details
 * - Inserts car record into parking_active table
 * - Uses INSERT ... ON DUPLICATE KEY UPDATE to handle duplicates
 * - Converts entry_time to MySQL DATETIME format
 * 
 * @usage
 *   ParkingActive car = {...}; // Fill with car data
 *   db_add_active_car(conn, &car);
 */
int db_add_active_car(MYSQL *conn, const ParkingActive *car);

/**
 * Remove a car from MySQL active parking table
 * 
 * @param conn MySQL connection handle
 * @param car_number Car number to remove
 * @return 1 on success, 0 on failure
 * 
 * @details
 * - Deletes car record from parking_active table
 * - Called when car exits parking
 * 
 * @usage
 *   db_remove_active_car(conn, "ABC-1234");
 */
int db_remove_active_car(MYSQL *conn, const char *car_number);

/**
 * Add a car to MySQL parking history table
 * 
 * @param conn MySQL connection handle
 * @param car Pointer to ParkingHistory structure with car data
 * @return 1 on success, 0 on failure
 * 
 * @details
 * - Inserts completed parking transaction into parking_history
 * - Includes both entry_time and exit_time
 * - Called when car exits parking
 * 
 * @usage
 *   ParkingHistory history = {...}; // Fill with history data
 *   db_add_history_car(conn, &history);
 */
int db_add_history_car(MYSQL *conn, const ParkingHistory *car);

// ============================================================================
// USER OPERATIONS - REMOVED (not part of Suborna's tasks)
// ============================================================================
// db_load_users() and db_save_user() removed - authentication not needed

#endif // ENABLE_MYSQL

#endif // DB_H

