#ifndef PARKING_H
#define PARKING_H

#include "models.h"

// Parking management functions
int add_car_entry(AppState *state, const char *car_number, VehicleType vehicle_type, 
                  int garage_id, int duration_hours);
int exit_car(AppState *state, const char *car_number);
int find_active_car(const AppState *state, const char *car_number);
int is_car_number_valid(const char *car_number);
void initialize_default_garages(Garage *garages, int *count);
void update_garage_availability(Garage *garages, int garage_count, int garage_id, int change);

#endif // PARKING_H

