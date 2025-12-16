#ifndef CAR_ENTRY_EXIT_H
#define CAR_ENTRY_EXIT_H

#include "models.h"

// Bill calculation function (moved from billing.c)
float calculate_bill(const Garage *garage, VehicleType vehicle_type, int duration_hours);

// Parking management functions
int add_car_entry(AppState *state, const char *car_number, VehicleType vehicle_type, 
                  int garage_id, int duration_hours);
int exit_car(AppState *state, const char *car_number);
int find_active_car(const AppState *state, const char *car_number);
int is_car_number_valid(const char *car_number);
void initialize_default_garages(Garage *garages, int *count);
void update_garage_availability(Garage *garages, int garage_count, int garage_id, int change);

#endif // CAR_ENTRY_EXIT_H

