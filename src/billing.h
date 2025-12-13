#ifndef BILLING_H
#define BILLING_H

#include "models.h"

// Billing functions
float calculate_bill(const Garage *garage, VehicleType vehicle_type, int duration_hours);
float get_garage_revenue(const ParkingHistory *history, int history_count, int garage_id);
float get_total_revenue(const ParkingHistory *history, int history_count);

#endif // BILLING_H

