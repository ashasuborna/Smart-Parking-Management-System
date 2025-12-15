#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

/**
 * ============================================================================
 * SUBORNA AKTER - File Handling Functions
 * ============================================================================
 * These functions handle reading from and writing to CSV files.
 * All CSV operations are YOUR responsibility.
 * ============================================================================
 */

/**
 * Ensure data directories exist (create if they don't)
 * 
 * @details
 * - Creates "data" directory if it doesn't exist
 * - Creates "data/receipts" directory if it doesn't exist
 * - Called automatically before file operations
 * 
 * @note
 * - Uses mkdir() on Linux/Mac
 * - Uses _mkdir() on Windows (handled by #ifdef)
 * - Safe to call multiple times (doesn't error if exists)
 */
void ensure_data_directory(void) {
    mkdir(DATA_DIR, 0755);        // Create "data" directory
    mkdir(RECEIPTS_DIR, 0755);    // Create "data/receipts" directory
}

/**
 * Format timestamp to string for CSV storage
 * 
 * @param t Time value (time_t)
 * @param buffer Buffer to store formatted string
 * @param size Size of buffer
 * @return Pointer to buffer (same as buffer parameter)
 * 
 * @details
 * Converts time_t to string format: "YYYY-MM-DD HH:MM:SS"
 * Example: "2024-12-13 14:30:00"
 * 
 * @usage
 *   char time_str[50];
 *   format_timestamp(time(NULL), time_str, sizeof(time_str));
 *   // time_str now contains "2024-12-13 14:30:00"
 */
char* format_timestamp(time_t t, char *buffer, size_t size) {
    // Convert time_t to struct tm (local time)
    struct tm *tm_info = localtime(&t);
    // Format as "YYYY-MM-DD HH:MM:SS"
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer; // Return buffer for convenience
}

/**
 * Parse timestamp string from CSV back to time_t
 * 
 * @param str Timestamp string in format "YYYY-MM-DD HH:MM:SS"
 * @return time_t value, or current time if parsing fails
 * 
 * @details
 * Converts string format "YYYY-MM-DD HH:MM:SS" back to time_t
 * Example: "2024-12-13 14:30:00" -> time_t value
 * 
 * @note
 * - Uses strptime() on Linux/Mac
 * - Uses sscanf() on Windows (strptime not available)
 * - Returns current time if parsing fails (fallback)
 * 
 * @usage
 *   time_t entry_time = parse_timestamp("2024-12-13 14:30:00");
 */
time_t parse_timestamp(const char *str) {
    struct tm tm = {0};
    
#ifdef _WIN32
    // Windows: Use sscanf (strptime not available on Windows)
    if (sscanf(str, "%d-%d-%d %d:%d:%d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
        // Adjust year (sscanf gives full year, tm needs years since 1900)
        tm.tm_year -= 1900;
        // Adjust month (sscanf gives 1-12, tm needs 0-11)
        tm.tm_mon -= 1;
        return mktime(&tm); // Convert to time_t
    }
#else
    // Linux/Mac: Use strptime (standard function)
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        return mktime(&tm); // Convert to time_t
    }
#endif
    
    // Parsing failed - return current time as fallback
    return time(NULL);
}

int load_users(User *users, int *count) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        *count = 0;
        return 0;
    }
    
    *count = 0;
    char line[200];
    while (fgets(line, sizeof(line), fp) && *count < 100) {
        char username[MAX_USERNAME_LEN];
        char password[MAX_PASSWORD_LEN];
        char role_str[10];
        
        if (sscanf(line, "%49[^:]:%49[^:]:%9s", username, password, role_str) == 3) {
            strncpy(users[*count].username, username, MAX_USERNAME_LEN - 1);
            strncpy(users[*count].password, password, MAX_PASSWORD_LEN - 1);
            users[*count].role = (strcmp(role_str, "admin") == 0) ? ROLE_ADMIN : ROLE_USER;
            (*count)++;
        }
    }
    
    fclose(fp);
    return 1;
}

int save_user(const User *user) {
    ensure_data_directory();
    FILE *fp = fopen(USERS_FILE, "a");
    if (!fp) return 0;
    
    fprintf(fp, "%s:%s:%s\n", user->username, user->password, role_to_string(user->role));
    fclose(fp);
    return 1;
}

/**
 * Load active cars from CSV file
 * 
 * @param cars Array to store loaded cars (must have space for MAX_ACTIVE_CARS)
 * @param count Pointer to store number of cars loaded (output parameter)
 * @return 1 on success, 0 on failure (file not found or error)
 * 
 * @details
 * Reads data/active.csv file and loads all currently parked cars.
 * CSV format: car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
 * 
 * @file_format
 * Header: car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
 * Data:   ABC-1234,Car,1,2,2024-12-13 14:30:00,10.00
 * 
 * @usage
 * Called on application startup (in main.c):
 *   ParkingActive cars[MAX_ACTIVE_CARS];
 *   int count;
 *   load_active_cars(cars, &count);
 * 
 * @note
 * - Skips header row automatically
 * - Parses timestamps back to time_t
 * - Converts vehicle_type string to enum
 * - Maximum MAX_ACTIVE_CARS (1000) cars loaded
 */
int load_active_cars(ParkingActive *cars, int *count) {
    // Step 1: Open CSV file for reading
    FILE *fp = fopen(ACTIVE_FILE, "r"); // ACTIVE_FILE = "data/active.csv"
    if (!fp) {
        *count = 0;
        return 0; // File not found or error opening
    }
    
    char line[500]; // Buffer for reading each line
    *count = 0;     // Initialize count
    
    // Step 2: Skip header row (first line)
    if (fgets(line, sizeof(line), fp)) {
        // Step 3: Read each data line
        while (fgets(line, sizeof(line), fp) && *count < MAX_ACTIVE_CARS) {
            // Variables to parse from CSV
            char car_number[MAX_CAR_NUMBER_LEN];
            char vehicle_str[10];        // "Car" or "Bike"
            int garage_id, duration_hours;
            char entry_time_str[50];     // "2024-12-13 14:30:00"
            float bill_amount;
            
            // Parse CSV line: car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
            // %19[^,] means: read up to 19 chars until comma
            if (sscanf(line, "%19[^,],%9[^,],%d,%d,%49[^,],%f",
                      car_number, vehicle_str, &garage_id, &duration_hours, entry_time_str, &bill_amount) == 6) {
                // Successfully parsed 6 fields - fill in car structure
                strncpy(cars[*count].car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
                // Convert "Car"/"Bike" string to enum
                cars[*count].vehicle_type = (strcmp(vehicle_str, "Car") == 0) ? VEHICLE_CAR : VEHICLE_BIKE;
                cars[*count].garage_id = garage_id;
                cars[*count].duration_hours = duration_hours;
                // Parse timestamp string back to time_t
                cars[*count].entry_time = parse_timestamp(entry_time_str);
                cars[*count].bill_amount = bill_amount;
                (*count)++; // Move to next car
            }
            // If parsing failed, skip this line (invalid data)
        }
    }
    
    // Step 4: Close file
    fclose(fp);
    return 1; // Success
}

/**
 * Save active cars to CSV file
 * 
 * @param cars Array of active cars to save
 * @param count Number of cars in array
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Writes all active cars to data/active.csv file.
 * Overwrites existing file (complete save, not append).
 * 
 * @file_format
 * Header: car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
 * Data:   ABC-1234,Car,1,2,2024-12-13 14:30:00,10.00
 * 
 * @usage
 * Called after adding/removing cars (in ui.c):
 *   save_active_cars(app_state->active_cars, app_state->active_count);
 * 
 * @note
 * - Creates directories if they don't exist
 * - Formats timestamps as strings
 * - Converts vehicle_type enum to string
 * - Overwrites entire file (not append)
 */
int save_active_cars(const ParkingActive *cars, int count) {
    // Step 1: Ensure data directory exists
    ensure_data_directory();
    
    // Step 2: Open file for writing (overwrites existing file)
    FILE *fp = fopen(ACTIVE_FILE, "w"); // "w" = write mode (overwrite)
    if (!fp) return 0; // Failed to open file
    
    // Step 3: Write CSV header
    fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount\n");
    
    // Step 4: Write each car as a CSV row
    char time_buf[50];
    for (int i = 0; i < count; i++) {
        // Format timestamp to string
        format_timestamp(cars[i].entry_time, time_buf, sizeof(time_buf));
        
        // Write CSV row: car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
        fprintf(fp, "%s,%s,%d,%d,%s,%.2f\n",
                cars[i].car_number,                                    // Car number
                vehicle_type_to_string(cars[i].vehicle_type),         // "Car" or "Bike"
                cars[i].garage_id,                                    // Garage ID
                cars[i].duration_hours,                               // Duration
                time_buf,                                             // Entry time (formatted)
                cars[i].bill_amount);                                 // Bill amount
    }
    
    // Step 5: Close file
    fclose(fp);
    return 1; // Success
}

/**
 * Load parking history from CSV file
 * 
 * @param cars Array to store loaded history records
 * @param count Pointer to store number of records loaded
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Reads data/history.csv file and loads all completed parking transactions.
 * CSV format includes both entry_time and exit_time.
 * 
 * @file_format
 * Header: car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount
 * Data:   ABC-1234,Car,1,2,2024-12-13 14:30:00,2024-12-13 16:30:00,10.00
 * 
 * @usage
 * Called on application startup (in main.c):
 *   ParkingHistory history[MAX_HISTORY_CARS];
 *   int count;
 *   load_history_cars(history, &count);
 */
int load_history_cars(ParkingHistory *cars, int *count) {
    // Step 1: Open CSV file for reading
    FILE *fp = fopen(HISTORY_FILE, "r"); // HISTORY_FILE = "data/history.csv"
    if (!fp) {
        *count = 0;
        return 0; // File not found or error
    }
    
    char line[500];
    *count = 0;
    
    // Step 2: Skip header row
    if (fgets(line, sizeof(line), fp)) {
        // Step 3: Read each data line
        while (fgets(line, sizeof(line), fp) && *count < MAX_HISTORY_CARS) {
            // Variables to parse from CSV
            char car_number[MAX_CAR_NUMBER_LEN];
            char vehicle_str[10];
            int garage_id, duration_hours;
            char entry_time_str[50], exit_time_str[50]; // Both entry and exit times
            float bill_amount;
            
            // Parse CSV line: 7 fields (includes exit_time)
            if (sscanf(line, "%19[^,],%9[^,],%d,%d,%49[^,],%49[^,],%f",
                      car_number, vehicle_str, &garage_id, &duration_hours,
                      entry_time_str, exit_time_str, &bill_amount) == 7) {
                // Successfully parsed - fill in history structure
                strncpy(cars[*count].car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
                cars[*count].vehicle_type = (strcmp(vehicle_str, "Car") == 0) ? VEHICLE_CAR : VEHICLE_BIKE;
                cars[*count].garage_id = garage_id;
                cars[*count].duration_hours = duration_hours;
                // Parse both timestamps
                cars[*count].entry_time = parse_timestamp(entry_time_str);
                cars[*count].exit_time = parse_timestamp(exit_time_str);
                cars[*count].bill_amount = bill_amount;
                (*count)++;
            }
        }
    }
    
    fclose(fp);
    return 1;
}

/**
 * Save parking history to CSV file (complete overwrite)
 * 
 * @param cars Array of history records to save
 * @param count Number of records in array
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Writes all history records to data/history.csv.
 * Overwrites entire file (complete save).
 * 
 * @note
 * Usually use append_history_car() instead (adds one record).
 * This function saves entire array (useful for manual save).
 */
int save_history_cars(const ParkingHistory *cars, int count) {
    ensure_data_directory();
    FILE *fp = fopen(HISTORY_FILE, "w");
    if (!fp) return 0;
    
    // Write header
    fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount\n");
    
    // Write each history record
    char entry_buf[50], exit_buf[50];
    for (int i = 0; i < count; i++) {
        format_timestamp(cars[i].entry_time, entry_buf, sizeof(entry_buf));
        format_timestamp(cars[i].exit_time, exit_buf, sizeof(exit_buf));
        fprintf(fp, "%s,%s,%d,%d,%s,%s,%.2f\n",
                cars[i].car_number,
                vehicle_type_to_string(cars[i].vehicle_type),
                cars[i].garage_id,
                cars[i].duration_hours,
                entry_buf,
                exit_buf,
                cars[i].bill_amount);
    }
    
    fclose(fp);
    return 1;
}

/**
 * Append a single history record to CSV file
 * 
 * @param car Pointer to ParkingHistory structure to append
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Adds one completed parking transaction to data/history.csv.
 * Appends to end of file (doesn't overwrite).
 * Creates file with header if it doesn't exist.
 * 
 * @usage
 * Called from exit_car() when car exits:
 *   ParkingHistory history = {...}; // Filled with car data
 *   append_history_car(&history);
 * 
 * @note
 * - This is the preferred way to save history (one record at a time)
 * - Creates header automatically if file is new
 * - Appends to existing file (preserves old records)
 */
int append_history_car(const ParkingHistory *car) {
    // Step 1: Ensure directory exists
    ensure_data_directory();
    
    // Step 2: Try to open file in append mode
    FILE *fp = fopen(HISTORY_FILE, "a"); // "a" = append mode
    if (!fp) {
        // File doesn't exist - create it with header
        fp = fopen(HISTORY_FILE, "w"); // "w" = write mode (create new)
        if (!fp) return 0; // Failed to create file
        // Write header
        fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount\n");
    } else {
        // File exists - check if it's empty (needs header)
        fseek(fp, 0, SEEK_END); // Move to end of file
        if (ftell(fp) == 0) {
            // File is empty - write header
            fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount\n");
        }
    }
    
    // Step 3: Format timestamps
    char entry_buf[50], exit_buf[50];
    format_timestamp(car->entry_time, entry_buf, sizeof(entry_buf));
    format_timestamp(car->exit_time, exit_buf, sizeof(exit_buf));
    
    // Step 4: Append CSV row
    fprintf(fp, "%s,%s,%d,%d,%s,%s,%.2f\n",
            car->car_number,
            vehicle_type_to_string(car->vehicle_type),
            car->garage_id,
            car->duration_hours,
            entry_buf,
            exit_buf,
            car->bill_amount);
    
    // Step 5: Close file
    fclose(fp);
    return 1; // Success
}

/**
 * Load garages from CSV file
 * 
 * @param garages Array to store loaded garages
 * @param count Pointer to store number of garages loaded
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Reads data/garages.csv file and loads garage information.
 * Includes capacity, availability, and hourly rates.
 * 
 * @file_format
 * Header: id,name,total_capacity,available_slots,hourly_rate_car,hourly_rate_bike
 * Data:   1,Downtown Garage,50,45,5.00,2.00
 * 
 * @usage
 * Called on application startup (in main.c):
 *   Garage garages[MAX_GARAGES];
 *   int count;
 *   load_garages(garages, &count);
 */
int load_garages(Garage *garages, int *count) {
    FILE *fp = fopen(GARAGES_FILE, "r"); // GARAGES_FILE = "data/garages.csv"
    if (!fp) {
        *count = 0;
        return 0; // File not found
    }
    
    char line[500];
    *count = 0;
    
    // Skip header row
    if (fgets(line, sizeof(line), fp)) {
        // Read each garage record
        while (fgets(line, sizeof(line), fp) && *count < MAX_GARAGES) {
            // Parse CSV: id,name,total_capacity,available_slots,hourly_rate_car,hourly_rate_bike
            if (sscanf(line, "%d,%99[^,],%d,%d,%f,%f",
                      &garages[*count].id,
                      garages[*count].name,
                      &garages[*count].total_capacity,
                      &garages[*count].available_slots,
                      &garages[*count].hourly_rate_car,
                      &garages[*count].hourly_rate_bike) == 6) {
                (*count)++; // Successfully loaded one garage
            }
        }
    }
    
    fclose(fp);
    return 1;
}

/**
 * Save garages to CSV file
 * 
 * @param garages Array of garages to save
 * @param count Number of garages in array
 * @return 1 on success, 0 on failure
 * 
 * @details
 * Writes all garage information to data/garages.csv.
 * Overwrites entire file (complete save).
 * 
 * @usage
 * Called when garage data changes (capacity, rates, etc.):
 *   save_garages(app_state->garages, app_state->garage_count);
 */
int save_garages(const Garage *garages, int count) {
    ensure_data_directory();
    FILE *fp = fopen(GARAGES_FILE, "w");
    if (!fp) return 0;
    
    // Write header
    fprintf(fp, "id,name,total_capacity,available_slots,hourly_rate_car,hourly_rate_bike\n");
    
    // Write each garage
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%d,%d,%.2f,%.2f\n",
                garages[i].id,
                garages[i].name,
                garages[i].total_capacity,
                garages[i].available_slots,
                garages[i].hourly_rate_car,
                garages[i].hourly_rate_bike);
    }
    
    fclose(fp);
    return 1;
}

int export_receipt(const ParkingHistory *car, const Garage *garage) {
    ensure_data_directory();
    
    char filename[200];
    snprintf(filename, sizeof(filename), "%s/receipt_%s_%ld.txt", RECEIPTS_DIR, car->car_number, (long)car->exit_time);
    
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;
    
    char entry_buf[50], exit_buf[50];
    format_timestamp(car->entry_time, entry_buf, sizeof(entry_buf));
    format_timestamp(car->exit_time, exit_buf, sizeof(exit_buf));
    
    fprintf(fp, "========================================\n");
    fprintf(fp, "    SMART PARKING MANAGEMENT SYSTEM\n");
    fprintf(fp, "            PARKING RECEIPT\n");
    fprintf(fp, "========================================\n\n");
    fprintf(fp, "Car Number:      %s\n", car->car_number);
    fprintf(fp, "Vehicle Type:    %s\n", vehicle_type_to_string(car->vehicle_type));
    fprintf(fp, "Garage:          %s (ID: %d)\n", garage->name, garage->id);
    fprintf(fp, "Duration:        %d hour(s)\n", car->duration_hours);
    fprintf(fp, "Entry Time:      %s\n", entry_buf);
    fprintf(fp, "Exit Time:       %s\n", exit_buf);
    fprintf(fp, "----------------------------------------\n");
    fprintf(fp, "Total Amount:    $%.2f\n", car->bill_amount);
    fprintf(fp, "========================================\n");
    fprintf(fp, "Thank you for using our service!\n");
    
    fclose(fp);
    return 1;
}

