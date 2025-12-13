#include "billing.h"

float calculate_bill(const Garage *garage, VehicleType vehicle_type, int duration_hours) {
    float rate = (vehicle_type == VEHICLE_CAR) ? garage->hourly_rate_car : garage->hourly_rate_bike;
    return rate * duration_hours;
}

float get_garage_revenue(const ParkingHistory *history, int history_count, int garage_id) {
    float revenue = 0.0;
    for (int i = 0; i < history_count; i++) {
        if (history[i].garage_id == garage_id) {
            revenue += history[i].bill_amount;
        }
    }
    return revenue;
}

float get_total_revenue(const ParkingHistory *history, int history_count) {
    float revenue = 0.0;
    for (int i = 0; i < history_count; i++) {
        revenue += history[i].bill_amount;
    }
    return revenue;
}

