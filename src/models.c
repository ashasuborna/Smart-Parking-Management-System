#include "models.h"
#include <string.h>

void init_app_state(AppState *state) {
    memset(state, 0, sizeof(AppState));
    state->is_logged_in = 0;
    state->garage_count = 0;
    state->active_count = 0;
    state->history_count = 0;
    state->total_revenue = 0.0;
}

const char* vehicle_type_to_string(VehicleType type) {
    return (type == VEHICLE_CAR) ? "Car" : "Bike";
}

const char* role_to_string(UserRole role) {
    return (role == ROLE_ADMIN) ? "admin" : "user";
}

