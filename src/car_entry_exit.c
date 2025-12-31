#include "car_entry_exit.h"
#include "file_handling.h"
#include <string.h>
#include <time.h>
#include <ctype.h>

// Calculate parking bill based on vehicle type and duration
float calculate_bill(const Garage *garage, VehicleType vehicle_type, int duration_hours) {
    float rate = (vehicle_type == VEHICLE_CAR) ? garage->hourly_rate_car : garage->hourly_rate_bike;
    return rate * duration_hours;
}

// Calculate total revenue from all parking history
float get_total_revenue(const ParkingHistory *history, int history_count) {
    float revenue = 0.0;
    for (int i = 0; i < history_count; i++) {
        revenue += history[i].bill_amount;
    }
    return revenue;
}

// Initialize default garage data (3 garages)
void initialize_default_garages(Garage *garages, int *count) {
    *count = 3;
    
    garages[0].id = 1;
    strncpy(garages[0].name, "Downtown Garage", MAX_GARAGE_NAME_LEN - 1);
    garages[0].total_capacity = 50;
    garages[0].available_slots = 50;
    garages[0].hourly_rate_car = 5.0;
    garages[0].hourly_rate_bike = 2.0;
    
    garages[1].id = 2;
    strncpy(garages[1].name, "Mall Parking", MAX_GARAGE_NAME_LEN - 1);
    garages[1].total_capacity = 100;
    garages[1].available_slots = 100;
    garages[1].hourly_rate_car = 4.0;
    garages[1].hourly_rate_bike = 1.5;
    
    garages[2].id = 3;
    strncpy(garages[2].name, "Office Complex", MAX_GARAGE_NAME_LEN - 1);
    garages[2].total_capacity = 75;
    garages[2].available_slots = 75;
    garages[2].hourly_rate_car = 6.0;
    garages[2].hourly_rate_bike = 3.0;
}

// Check if car number is valid (not empty, not too long, no commas)
int is_car_number_valid(const char *car_number) {
    if (!car_number || strlen(car_number) == 0 || strlen(car_number) >= MAX_CAR_NUMBER_LEN) {
        return 0;
    }
    if (strchr(car_number, ',') != NULL) {
        return 0;
    }
    return 1;
}

// Find car in active list by car number, returns index or -1 if not found
int find_active_car(const AppState *state, const char *car_number) {
    for (int i = 0; i < state->active_count; i++) {
        if (strcmp(state->active_cars[i].car_number, car_number) == 0) {
            return i;
        }
    }
    return -1;
}

// Find garage by ID, returns index or -1 if not found
int find_garage(const Garage *garages, int garage_count, int garage_id) {
    for (int i = 0; i < garage_count; i++) {
        if (garages[i].id == garage_id) {
            return i;
        }
    }
    return -1;
}

// Update garage available slots (change: -1 for entry, +1 for exit)
void update_garage_availability(Garage *garages, int garage_count, int garage_id, int change) {
    int idx = find_garage(garages, garage_count, garage_id);
    if (idx >= 0) {
        garages[idx].available_slots += change;
        if (garages[idx].available_slots < 0) {
            garages[idx].available_slots = 0;
        }
        if (garages[idx].available_slots > garages[idx].total_capacity) {
            garages[idx].available_slots = garages[idx].total_capacity;
        }
    }
}

// Add car to parking system, returns 0 on success or error code on failure
int add_car_entry(AppState *state, const char *car_number, VehicleType vehicle_type, 
                  int garage_id, int duration_hours) {
    if (!is_car_number_valid(car_number)) {
        return -1;
    }
    if (duration_hours < 1 || duration_hours > 12) {
        return -2;
    }
    if (state->active_count >= MAX_ACTIVE_CARS) {
        return -3;
    }
    if (find_active_car(state, car_number) >= 0) {
        return -4;
    }
    
    int garage_idx = find_garage(state->garages, state->garage_count, garage_id);
    if (garage_idx < 0) {
        return -5;
    }
    if (state->garages[garage_idx].available_slots <= 0) {
        return -6;
    }
    
    ParkingActive *car = &state->active_cars[state->active_count];
    strncpy(car->car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
    car->vehicle_type = vehicle_type;
    car->garage_id = garage_id;
    car->duration_hours = duration_hours;
    car->entry_time = time(NULL);
    car->bill_amount = calculate_bill(&state->garages[garage_idx], vehicle_type, duration_hours);
    state->active_count++;
    
    update_garage_availability(state->garages, state->garage_count, garage_id, -1);
    
    return 0;
}

// Exit car from parking system, returns 0 on success or -1 if car not found
int exit_car(AppState *state, const char *car_number) {
    int idx = find_active_car(state, car_number);
    if (idx < 0) {
        return -1;
    }
    
    ParkingHistory history;
    strncpy(history.car_number, state->active_cars[idx].car_number, MAX_CAR_NUMBER_LEN - 1);
    history.vehicle_type = state->active_cars[idx].vehicle_type;
    history.garage_id = state->active_cars[idx].garage_id;
    history.duration_hours = state->active_cars[idx].duration_hours;
    history.entry_time = state->active_cars[idx].entry_time;
    history.exit_time = time(NULL);
    history.bill_amount = state->active_cars[idx].bill_amount;
    
    if (state->history_count < MAX_HISTORY_CARS) {
        state->history_cars[state->history_count++] = history;
    }
    append_history_car(&history);
    
    update_garage_availability(state->garages, state->garage_count, history.garage_id, 1);
    
    for (int i = idx; i < state->active_count - 1; i++) {
        state->active_cars[i] = state->active_cars[i + 1];
    }
    state->active_count--;
    
    state->total_revenue += history.bill_amount;
    
    return 0;
}

