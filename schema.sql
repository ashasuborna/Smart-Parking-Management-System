-- Smart Parking Management System - MySQL Database Schema
-- This schema is used when ENABLE_MYSQL is defined during compilation

-- Create database (run this manually if needed)
-- CREATE DATABASE IF NOT EXISTS smart_parking;
-- USE smart_parking;

-- Users table
CREATE TABLE IF NOT EXISTS users (
    username VARCHAR(50) PRIMARY KEY,
    password VARCHAR(50) NOT NULL,
    role VARCHAR(10) NOT NULL
);

-- Garages table
CREATE TABLE IF NOT EXISTS garages (
    id INT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    total_capacity INT NOT NULL,
    available_slots INT NOT NULL,
    hourly_rate_car FLOAT NOT NULL,
    hourly_rate_bike FLOAT NOT NULL
);

-- Active parking records
CREATE TABLE IF NOT EXISTS parking_active (
    car_number VARCHAR(20) PRIMARY KEY,
    vehicle_type VARCHAR(10) NOT NULL,
    garage_id INT NOT NULL,
    duration_hours INT NOT NULL,
    entry_time DATETIME NOT NULL,
    bill_amount FLOAT NOT NULL,
    FOREIGN KEY (garage_id) REFERENCES garages(id)
);

-- Parking history
CREATE TABLE IF NOT EXISTS parking_history (
    id INT AUTO_INCREMENT PRIMARY KEY,
    car_number VARCHAR(20) NOT NULL,
    vehicle_type VARCHAR(10) NOT NULL,
    garage_id INT NOT NULL,
    duration_hours INT NOT NULL,
    entry_time DATETIME NOT NULL,
    exit_time DATETIME NOT NULL,
    bill_amount FLOAT NOT NULL,
    FOREIGN KEY (garage_id) REFERENCES garages(id)
);

-- Insert default garages
INSERT INTO garages (id, name, total_capacity, available_slots, hourly_rate_car, hourly_rate_bike)
VALUES
    (1, 'Downtown Garage', 50, 50, 5.0, 2.0),
    (2, 'Mall Parking', 100, 100, 4.0, 1.5),
    (3, 'Office Complex', 75, 75, 6.0, 3.0)
ON DUPLICATE KEY UPDATE name=VALUES(name);

-- Insert sample users (password is plaintext for demo - use hashing in production)
INSERT INTO users (username, password, role)
VALUES
    ('admin', 'admin123', 'admin'),
    ('user1', 'user123', 'user'),
    ('user2', 'user123', 'user')
ON DUPLICATE KEY UPDATE password=VALUES(password);

