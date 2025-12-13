#ifndef MODELS_H
#define MODELS_H

#include <time.h>

#define MAX_CAR_NUMBER_LEN 20
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_GARAGE_NAME_LEN 100
#define MAX_GARAGES 10
#define MAX_ACTIVE_CARS 1000
#define MAX_HISTORY_CARS 10000

// User roles
typedef enum {
    ROLE_USER = 0,
    ROLE_ADMIN = 1
} UserRole;

// Vehicle types
typedef enum {
    VEHICLE_CAR = 0,
    VEHICLE_BIKE = 1
} VehicleType;

// User structure
typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
} User;

// Garage structure
typedef struct {
    int id;
    char name[MAX_GARAGE_NAME_LEN];
    int total_capacity;
    int available_slots;
    float hourly_rate_car;
    float hourly_rate_bike;
} Garage;

// Parking record (active)
typedef struct {
    char car_number[MAX_CAR_NUMBER_LEN];
    VehicleType vehicle_type;
    int garage_id;
    int duration_hours;
    time_t entry_time;
    float bill_amount;
} ParkingActive;

// Parking record (history)
typedef struct {
    char car_number[MAX_CAR_NUMBER_LEN];
    VehicleType vehicle_type;
    int garage_id;
    int duration_hours;
    time_t entry_time;
    time_t exit_time;
    float bill_amount;
} ParkingHistory;

// Application state
typedef struct {
    User current_user;
    int is_logged_in;
    Garage garages[MAX_GARAGES];
    int garage_count;
    ParkingActive active_cars[MAX_ACTIVE_CARS];
    int active_count;
    ParkingHistory history_cars[MAX_HISTORY_CARS];
    int history_count;
    float total_revenue;
} AppState;

// Function to initialize app state
void init_app_state(AppState *state);

// Function to get vehicle type string
const char* vehicle_type_to_string(VehicleType type);

// Function to get role string
const char* role_to_string(UserRole role);

#endif // MODELS_H

