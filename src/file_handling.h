#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "models.h"

#define DATA_DIR "data"
#define ACTIVE_FILE "data/active.csv"
#define HISTORY_FILE "data/history.csv"
#define GARAGES_FILE "data/garages.csv"
#define RECEIPTS_DIR "data/receipts"

// File I/O functions - Suborna's tasks only
// REMOVED: load_users() and save_user() - not part of Suborna's tasks
int load_active_cars(ParkingActive *cars, int *count);
int save_active_cars(const ParkingActive *cars, int count);
int load_history_cars(ParkingHistory *cars, int *count);
int save_history_cars(const ParkingHistory *cars, int count);
int append_history_car(const ParkingHistory *car);
int load_garages(Garage *garages, int *count);
int save_garages(const Garage *garages, int count);
int export_receipt(const ParkingHistory *car, const Garage *garage);

// Utility functions
void ensure_data_directory(void);
char* format_timestamp(time_t t, char *buffer, size_t size);
time_t parse_timestamp(const char *str);

#endif // FILE_HANDLING_H

