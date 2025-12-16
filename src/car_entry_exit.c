#include "car_entry_exit.h"
#include "file_handling.h"
#include <string.h>
#include <time.h>
#include <ctype.h>

/**
 * ============================================================================
 * SUBORNA AKTER - Car Entry/Exit Functions
 * ============================================================================
 * These functions handle adding cars to parking and exiting cars.
 * Bill calculation functions are also included here.
 * ============================================================================
 */

/**
 * Calculate parking bill
 * 
 * @param garage Pointer to Garage structure
 * @param vehicle_type VEHICLE_CAR or VEHICLE_BIKE
 * @param duration_hours Parking duration in hours
 * @return Calculated bill amount
 * 
 * @details
 * Calculates bill by multiplying hourly rate by duration.
 * Different rates for cars and bikes.
 */
float calculate_bill(const Garage *garage, VehicleType vehicle_type, int duration_hours) {
    float rate = (vehicle_type == VEHICLE_CAR) ? garage->hourly_rate_car : garage->hourly_rate_bike;
    return rate * duration_hours;
}

/**
 * Get total revenue from parking history
 * 
 * @param history Array of parking history records
 * @param history_count Number of records
 * @return Total revenue amount
 */
float get_total_revenue(const ParkingHistory *history, int history_count) {
    float revenue = 0.0;
    for (int i = 0; i < history_count; i++) {
        revenue += history[i].bill_amount;
    }
    return revenue;
}

/**
 * Get revenue for a specific garage
 * 
 * @param history Array of parking history records
 * @param history_count Number of records
 * @param garage_id Garage ID to calculate revenue for
 * @return Revenue for the specified garage
 */
float get_garage_revenue(const ParkingHistory *history, int history_count, int garage_id) {
    float revenue = 0.0;
    for (int i = 0; i < history_count; i++) {
        if (history[i].garage_id == garage_id) {
            revenue += history[i].bill_amount;
        }
    }
    return revenue;
}

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

/**
 * ============================================================================
 * SUBORNA AKTER - Car Entry/Exit Functions
 * ============================================================================
 * These functions handle adding cars to parking and exiting cars.
 * ============================================================================
 */

/**
 * Validate car number format
 * 
 * @param car_number Car number string to validate (e.g., "ABC-1234")
 * @return 1 if valid, 0 if invalid
 * 
 * @details
 * Validates that car number:
 * - Is not NULL or empty
 * - Is within maximum length limit
 * - Does not contain commas (would break CSV format)
 * 
 * @usage
 *   if (is_car_number_valid("ABC-1234")) {
 *       // Valid car number
 *   }
 * 
 * @note
 * Commas are not allowed because car numbers are stored in CSV files
 */
int is_car_number_valid(const char *car_number) {
    // Check 1: Must not be NULL or empty
    if (!car_number || strlen(car_number) == 0 || strlen(car_number) >= MAX_CAR_NUMBER_LEN) {
        return 0; // Invalid: empty or too long
    }
    
    // Check 2: Must not contain commas (would break CSV format)
    if (strchr(car_number, ',') != NULL) {
        return 0; // Invalid: contains comma
    }
    
    return 1; // Valid car number
}

/**
 * Find a car in the active parking list by car number
 * 
 * @param state Pointer to application state (contains active_cars array)
 * @param car_number Car number to search for (e.g., "ABC-1234")
 * @return Index in active_cars array if found, -1 if not found
 * 
 * @details
 * - Searches through active_cars array
 * - Compares car_number using strcmp()
 * - Returns first match found
 * 
 * @usage
 *   int idx = find_active_car(&app_state, "ABC-1234");
 *   if (idx >= 0) {
 *       // Car found at index idx
 *   } else {
 *       // Car not found
 *   }
 */
int find_active_car(const AppState *state, const char *car_number) {
    // Loop through all active cars
    for (int i = 0; i < state->active_count; i++) {
        // Compare car numbers (case-sensitive)
        if (strcmp(state->active_cars[i].car_number, car_number) == 0) {
            return i; // Found! Return index
        }
    }
    return -1; // Not found
}

int find_garage(const Garage *garages, int garage_count, int garage_id) {
    for (int i = 0; i < garage_count; i++) {
        if (garages[i].id == garage_id) {
            return i;
        }
    }
    return -1;
}

/**
 * Update garage available slots count
 * 
 * @param garages Array of garages
 * @param garage_count Number of garages in array
 * @param garage_id ID of garage to update
 * @param change Change amount (+1 for exit, -1 for entry)
 * 
 * @details
 * - Finds garage by ID
 * - Updates available_slots by adding/subtracting change
 * - Ensures available_slots stays within valid range (0 to total_capacity)
 * 
 * @usage
 *   // When car enters: decrease available slots
 *   update_garage_availability(garages, count, garage_id, -1);
 *   
 *   // When car exits: increase available slots
 *   update_garage_availability(garages, count, garage_id, +1);
 * 
 * @note
 * - change = -1 when car enters (slot taken)
 * - change = +1 when car exits (slot freed)
 */
void update_garage_availability(Garage *garages, int garage_count, int garage_id, int change) {
    // Step 1: Find garage by ID
    int idx = find_garage(garages, garage_count, garage_id);
    if (idx >= 0) {
        // Step 2: Update available slots
        garages[idx].available_slots += change;
        
        // Step 3: Ensure valid range (cannot be negative or exceed capacity)
        if (garages[idx].available_slots < 0) {
            garages[idx].available_slots = 0; // Minimum: 0
        }
        if (garages[idx].available_slots > garages[idx].total_capacity) {
            garages[idx].available_slots = garages[idx].total_capacity; // Maximum: total capacity
        }
    }
}

/**
 * Add a car to the parking system
 * 
 * @param state Pointer to application state (will be modified)
 * @param car_number Car number string (e.g., "ABC-1234")
 * @param vehicle_type VEHICLE_CAR or VEHICLE_BIKE
 * @param garage_id ID of garage (1, 2, or 3)
 * @param duration_hours Parking duration (1-12 hours)
 * @return 0 on success, negative error code on failure
 * 
 * @details
 * This is YOUR MAIN FUNCTION for car entry. It:
 * 1. Validates all input parameters
 * 2. Checks for duplicate car numbers
 * 3. Verifies garage has available slots
 * 4. Calculates bill automatically
 * 5. Adds car to active_cars array
 * 6. Updates garage availability
 * 
 * @return_codes
 *   0  = Success
 *  -1 = Invalid car number
 *  -2 = Invalid duration (must be 1-12 hours)
 *  -3 = Maximum cars reached (system limit)
 *  -4 = Car already parked (duplicate)
 *  -5 = Garage not found
 *  -6 = Garage is full (no available slots)
 * 
 * @usage
 * Called from ui.c when user clicks "Add Car":
 *   int result = add_car_entry(&app_state, "ABC-1234", VEHICLE_CAR, 1, 2);
 *   if (result == 0) {
 *       // Success! Car added
 *   } else {
 *       // Handle error based on return code
 *   }
 * 
 * @note
 * - Bill is calculated automatically using billing.c
 * - Entry time is set to current time
 * - Garage availability is updated immediately
 */
int add_car_entry(AppState *state, const char *car_number, VehicleType vehicle_type, 
                  int garage_id, int duration_hours) {
    // ========================================================================
    // STEP 1: VALIDATION - Check all inputs are valid
    // ========================================================================
    
    // Validation 1: Check car number format
    if (!is_car_number_valid(car_number)) {
        return -1; // Error: Invalid car number (empty, too long, or contains comma)
    }
    
    // Validation 2: Check duration is within valid range (1-12 hours)
    if (duration_hours < 1 || duration_hours > 12) {
        return -2; // Error: Duration must be between 1 and 12 hours
    }
    
    // Validation 3: Check system capacity limit
    if (state->active_count >= MAX_ACTIVE_CARS) {
        return -3; // Error: System at maximum capacity (1000 cars)
    }
    
    // Validation 4: Check if car is already parked (prevent duplicates)
    if (find_active_car(state, car_number) >= 0) {
        return -4; // Error: This car is already in the parking system
    }
    
    // ========================================================================
    // STEP 2: FIND AND VERIFY GARAGE
    // ========================================================================
    
    // Find garage by ID
    int garage_idx = find_garage(state->garages, state->garage_count, garage_id);
    if (garage_idx < 0) {
        return -5; // Error: Garage ID not found
    }
    
    // Check if garage has available slots
    if (state->garages[garage_idx].available_slots <= 0) {
        return -6; // Error: Garage is full (no available slots)
    }
    
    // ========================================================================
    // STEP 3: ADD CAR TO ACTIVE LIST
    // ========================================================================
    
    // Get pointer to next available slot in active_cars array
    ParkingActive *car = &state->active_cars[state->active_count];
    
    // Fill in car data
    strncpy(car->car_number, car_number, MAX_CAR_NUMBER_LEN - 1);
    car->vehicle_type = vehicle_type;              // Car or Bike
    car->garage_id = garage_id;                     // Which garage
    car->duration_hours = duration_hours;           // How long
    car->entry_time = time(NULL);                   // Current time
    // Calculate bill automatically (using calculate_bill function in this file)
    car->bill_amount = calculate_bill(&state->garages[garage_idx], vehicle_type, duration_hours);
    
    // Increment active car count
    state->active_count++;
    
    // ========================================================================
    // STEP 4: UPDATE GARAGE AVAILABILITY
    // ========================================================================
    
    // Decrease available slots by 1 (car entered)
    update_garage_availability(state->garages, state->garage_count, garage_id, -1);
    
    // Success!
    return 0;
}

/**
 * Exit a car from the parking system
 * 
 * @param state Pointer to application state (will be modified)
 * @param car_number Car number to exit (e.g., "ABC-1234")
 * @return 0 on success, -1 if car not found
 * 
 * @details
 * This is YOUR MAIN FUNCTION for car exit. It:
 * 1. Finds car in active list
 * 2. Creates history record with exit time
 * 3. Exports receipt (calls Nilufa's function)
 * 4. Moves car from active to history
 * 5. Updates garage availability
 * 6. Updates total revenue
 * 
 * @return_codes
 *   0  = Success (car exited)
 *  -1 = Car not found in active list
 * 
 * @usage
 * Called from ui.c when user clicks "Exit Car":
 *   int result = exit_car(&app_state, "ABC-1234");
 *   if (result == 0) {
 *       // Success! Car exited, receipt generated
 *   } else {
 *       // Error: Car not found
 *   }
 * 
 * @note
 * - Receipt is automatically generated (calls Nilufa's export_receipt)
 * - Car is removed from active list and added to history
 * - Garage slot is freed (availability increased)
 * - Revenue is added to total
 */
int exit_car(AppState *state, const char *car_number) {
    // ========================================================================
    // STEP 1: FIND CAR IN ACTIVE LIST
    // ========================================================================
    
    int idx = find_active_car(state, car_number);
    if (idx < 0) {
        return -1; // Error: Car not found in active parking list
    }
    
    // ========================================================================
    // STEP 2: CREATE HISTORY RECORD
    // ========================================================================
    
    // Copy data from active car to history record
    ParkingHistory history;
    strncpy(history.car_number, state->active_cars[idx].car_number, MAX_CAR_NUMBER_LEN - 1);
    history.vehicle_type = state->active_cars[idx].vehicle_type;
    history.garage_id = state->active_cars[idx].garage_id;
    history.duration_hours = state->active_cars[idx].duration_hours;
    history.entry_time = state->active_cars[idx].entry_time;  // Original entry time
    history.exit_time = time(NULL);                          // Current time (exit time)
    history.bill_amount = state->active_cars[idx].bill_amount; // Bill (already calculated)
    
    // ========================================================================
    // STEP 3: EXPORT RECEIPT (Calls Nilufa's function)
    // ========================================================================
    
    // Find garage information for receipt
    int garage_idx = find_garage(state->garages, state->garage_count, history.garage_id);
    
    // Generate receipt file (Nilufa's function - you just call it)
    if (garage_idx >= 0) {
        export_receipt(&history, &state->garages[garage_idx]);
    }
    
    // ========================================================================
    // STEP 4: MOVE TO HISTORY
    // ========================================================================
    
    // Add to history array (if space available)
    if (state->history_count < MAX_HISTORY_CARS) {
        state->history_cars[state->history_count++] = history;
    }
    
    // Also append to history CSV file (your fileio function)
    append_history_car(&history);
    
    // ========================================================================
    // STEP 5: UPDATE GARAGE AVAILABILITY
    // ========================================================================
    
    // Increase available slots by 1 (car exited, slot freed)
    update_garage_availability(state->garages, state->garage_count, history.garage_id, 1);
    
    // ========================================================================
    // STEP 6: REMOVE FROM ACTIVE LIST
    // ========================================================================
    
    // Shift array to remove car (move all cars after this one forward)
    for (int i = idx; i < state->active_count - 1; i++) {
        state->active_cars[i] = state->active_cars[i + 1];
    }
    state->active_count--; // Decrease active count
    
    // ========================================================================
    // STEP 7: UPDATE TOTAL REVENUE
    // ========================================================================
    
    // Add this car's bill to total revenue
    state->total_revenue += history.bill_amount;
    
    // Success!
    return 0;
}

