#include "parking.h"
#include "billing.h"
#include "fileio.h"
#include <string.h>
#include <time.h>
#include <ctype.h>

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

int is_car_number_valid(const char *car_number) {
    if (!car_number || strlen(car_number) == 0 || strlen(car_number) >= MAX_CAR_NUMBER_LEN) {
        return 0;
    }
    
    // Check for commas (not allowed in CSV)
    if (strchr(car_number, ',') != NULL) {
        return 0;
    }
    
    return 1;
}

int find_active_car(const AppState *state, const char *car_number) {
    for (int i = 0; i < state->active_count; i++) {
        if (strcmp(state->active_cars[i].car_number, car_number) == 0) {
            return i;
        }
    }
    return -1;
}

int find_garage(const Garage *garages, int garage_count, int garage_id) {
    for (int i = 0; i < garage_count; i++) {
        if (garages[i].id == garage_id) {
            return i;
        }
    }
    return -1;
}

void update_garage_availability(Garage *garages, int garage_count, int garage_id, int change) {
    int idx = find_garage(garages, garage_count, garage_id);
    if (idx >= 0) {
        garages[idx].available_slots += change;
        if (garages[idx].available_slots < 0) garages[idx].available_slots = 0;
        if (garages[idx].available_slots > garages[idx].total_capacity) {
            garages[idx].available_slots = garages[idx].total_capacity;
        }
    }
}

int add_car_entry(AppState *state, const char *car_number, VehicleType vehicle_type, 
                  int garage_id, int duration_hours) {
    // Validation
    if (!is_car_number_valid(car_number)) {
        return -1; // Invalid car number
    }
    
    if (duration_hours < 1 || duration_hours > 12) {
        return -2; // Invalid duration
    }
    
    if (state->active_count >= MAX_ACTIVE_CARS) {
        return -3; // Maximum cars reached
    }
    
    // Check if car already parked
    if (find_active_car(state, car_number) >= 0) {
        return -4; // Car already parked
    }
    
    // Find garage
    int garage_idx = find_garage(state->garages, state->garage_count, garage_id);
    if (garage_idx < 0) {
        return -5; // Garage not found
    }
    
    // Check garage availability
    if (state->garages[garage_idx].available_slots <= 0) {
        return -6; // Garage full
    }
    
    // Add car
    ParkingActive *car = &state->active_cars[state->active_count];
    strncpy(car->car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
    car->vehicle_type = vehicle_type;
    car->garage_id = garage_id;
    car->duration_hours = duration_hours;
    car->entry_time = time(NULL);
    car->bill_amount = calculate_bill(&state->garages[garage_idx], vehicle_type, duration_hours);
    
    state->active_count++;
    update_garage_availability(state->garages, state->garage_count, garage_id, -1);
    
    return 0; // Success
}

int exit_car(AppState *state, const char *car_number) {
    int idx = find_active_car(state, car_number);
    if (idx < 0) {
        return -1; // Car not found
    }
    
    // Move to history
    ParkingHistory history;
    strncpy(history.car_number, state->active_cars[idx].car_number, MAX_CAR_NUMBER_LEN - 1);
    history.vehicle_type = state->active_cars[idx].vehicle_type;
    history.garage_id = state->active_cars[idx].garage_id;
    history.duration_hours = state->active_cars[idx].duration_hours;
    history.entry_time = state->active_cars[idx].entry_time;
    history.exit_time = time(NULL);
    history.bill_amount = state->active_cars[idx].bill_amount;
    
    // Find garage for receipt
    int garage_idx = find_garage(state->garages, state->garage_count, history.garage_id);
    
    // Export receipt
    if (garage_idx >= 0) {
        export_receipt(&history, &state->garages[garage_idx]);
    }
    
    // Add to history
    if (state->history_count < MAX_HISTORY_CARS) {
        state->history_cars[state->history_count++] = history;
    }
    append_history_car(&history);
    
    // Update garage availability
    update_garage_availability(state->garages, state->garage_count, history.garage_id, 1);
    
    // Remove from active (shift array)
    for (int i = idx; i < state->active_count - 1; i++) {
        state->active_cars[i] = state->active_cars[i + 1];
    }
    state->active_count--;
    
    // Update revenue
    state->total_revenue += history.bill_amount;
    
    return 0; // Success
}

