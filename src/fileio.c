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

void ensure_data_directory(void) {
    mkdir(DATA_DIR, 0755);
    mkdir(RECEIPTS_DIR, 0755);
}

char* format_timestamp(time_t t, char *buffer, size_t size) {
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

time_t parse_timestamp(const char *str) {
    struct tm tm = {0};
#ifdef _WIN32
    // Windows alternative: use sscanf
    if (sscanf(str, "%d-%d-%d %d:%d:%d", 
               &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        return mktime(&tm);
    }
#else
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) != NULL) {
        return mktime(&tm);
    }
#endif
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

int load_active_cars(ParkingActive *cars, int *count) {
    FILE *fp = fopen(ACTIVE_FILE, "r");
    if (!fp) {
        *count = 0;
        return 0;
    }
    
    char line[500];
    *count = 0;
    
    // Skip header
    if (fgets(line, sizeof(line), fp)) {
        while (fgets(line, sizeof(line), fp) && *count < MAX_ACTIVE_CARS) {
            char car_number[MAX_CAR_NUMBER_LEN];
            char vehicle_str[10];
            int garage_id, duration_hours;
            char entry_time_str[50];
            float bill_amount;
            
            if (sscanf(line, "%19[^,],%9[^,],%d,%d,%49[^,],%f",
                      car_number, vehicle_str, &garage_id, &duration_hours, entry_time_str, &bill_amount) == 6) {
                strncpy(cars[*count].car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
                cars[*count].vehicle_type = (strcmp(vehicle_str, "Car") == 0) ? VEHICLE_CAR : VEHICLE_BIKE;
                cars[*count].garage_id = garage_id;
                cars[*count].duration_hours = duration_hours;
                cars[*count].entry_time = parse_timestamp(entry_time_str);
                cars[*count].bill_amount = bill_amount;
                (*count)++;
            }
        }
    }
    
    fclose(fp);
    return 1;
}

int save_active_cars(const ParkingActive *cars, int count) {
    ensure_data_directory();
    FILE *fp = fopen(ACTIVE_FILE, "w");
    if (!fp) return 0;
    
    fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount\n");
    
    char time_buf[50];
    for (int i = 0; i < count; i++) {
        format_timestamp(cars[i].entry_time, time_buf, sizeof(time_buf));
        fprintf(fp, "%s,%s,%d,%d,%s,%.2f\n",
                cars[i].car_number,
                vehicle_type_to_string(cars[i].vehicle_type),
                cars[i].garage_id,
                cars[i].duration_hours,
                time_buf,
                cars[i].bill_amount);
    }
    
    fclose(fp);
    return 1;
}

int load_history_cars(ParkingHistory *cars, int *count) {
    FILE *fp = fopen(HISTORY_FILE, "r");
    if (!fp) {
        *count = 0;
        return 0;
    }
    
    char line[500];
    *count = 0;
    
    // Skip header
    if (fgets(line, sizeof(line), fp)) {
        while (fgets(line, sizeof(line), fp) && *count < MAX_HISTORY_CARS) {
            char car_number[MAX_CAR_NUMBER_LEN];
            char vehicle_str[10];
            int garage_id, duration_hours;
            char entry_time_str[50], exit_time_str[50];
            float bill_amount;
            
            if (sscanf(line, "%19[^,],%9[^,],%d,%d,%49[^,],%49[^,],%f",
                      car_number, vehicle_str, &garage_id, &duration_hours,
                      entry_time_str, exit_time_str, &bill_amount) == 7) {
                strncpy(cars[*count].car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
                cars[*count].vehicle_type = (strcmp(vehicle_str, "Car") == 0) ? VEHICLE_CAR : VEHICLE_BIKE;
                cars[*count].garage_id = garage_id;
                cars[*count].duration_hours = duration_hours;
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

int save_history_cars(const ParkingHistory *cars, int count) {
    ensure_data_directory();
    FILE *fp = fopen(HISTORY_FILE, "w");
    if (!fp) return 0;
    
    fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount\n");
    
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

int append_history_car(const ParkingHistory *car) {
    ensure_data_directory();
    FILE *fp = fopen(HISTORY_FILE, "a");
    if (!fp) {
        // Create file with header if doesn't exist
        fp = fopen(HISTORY_FILE, "w");
        if (!fp) return 0;
        fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount\n");
    } else {
        fseek(fp, 0, SEEK_END);
        if (ftell(fp) == 0) {
            fprintf(fp, "car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount\n");
        }
    }
    
    char entry_buf[50], exit_buf[50];
    format_timestamp(car->entry_time, entry_buf, sizeof(entry_buf));
    format_timestamp(car->exit_time, exit_buf, sizeof(exit_buf));
    
    fprintf(fp, "%s,%s,%d,%d,%s,%s,%.2f\n",
            car->car_number,
            vehicle_type_to_string(car->vehicle_type),
            car->garage_id,
            car->duration_hours,
            entry_buf,
            exit_buf,
            car->bill_amount);
    
    fclose(fp);
    return 1;
}

int load_garages(Garage *garages, int *count) {
    FILE *fp = fopen(GARAGES_FILE, "r");
    if (!fp) {
        *count = 0;
        return 0;
    }
    
    char line[500];
    *count = 0;
    
    // Skip header
    if (fgets(line, sizeof(line), fp)) {
        while (fgets(line, sizeof(line), fp) && *count < MAX_GARAGES) {
            if (sscanf(line, "%d,%99[^,],%d,%d,%f,%f",
                      &garages[*count].id,
                      garages[*count].name,
                      &garages[*count].total_capacity,
                      &garages[*count].available_slots,
                      &garages[*count].hourly_rate_car,
                      &garages[*count].hourly_rate_bike) == 6) {
                (*count)++;
            }
        }
    }
    
    fclose(fp);
    return 1;
}

int save_garages(const Garage *garages, int count) {
    ensure_data_directory();
    FILE *fp = fopen(GARAGES_FILE, "w");
    if (!fp) return 0;
    
    fprintf(fp, "id,name,total_capacity,available_slots,hourly_rate_car,hourly_rate_bike\n");
    
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

