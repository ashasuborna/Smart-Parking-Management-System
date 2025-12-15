# Suborna Akter - Files to Work On

## Overview
This document lists all files you need to understand and work on for your assigned tasks:
1. **MySQL Integration**
2. **Car Entry/Exit**
3. **File Handling**
4. **Data Backup System**

---

## 📁 File List by Task

### Task 1: MySQL Integration

#### Files to Work On:

**1. `src/db.h`** - MySQL Header File
- **Purpose**: Declares all MySQL-related functions
- **Key Functions**:
  - `db_connect()` - Connect to MySQL database
  - `db_disconnect()` - Close MySQL connection
  - `db_init_tables()` - Create database tables
  - `db_add_active_car()` - Add car to MySQL active table
  - `db_remove_active_car()` - Remove car from MySQL
  - `db_add_history_car()` - Add car to MySQL history table
  - `db_load_users()` - Load users from MySQL
  - `db_save_user()` - Save user to MySQL

**2. `src/db.c`** - MySQL Implementation
- **Purpose**: Implements all MySQL database operations
- **Current State**: Basic structure exists, needs enhancement
- **What to Do**:
  - Test connection parameters
  - Add error handling
  - Ensure proper synchronization with file storage
  - Add connection retry logic

**3. `src/main.c`** - Application Entry Point
- **Lines to Check**: Lines 43-50 (MySQL initialization)
- **What It Does**: 
  - Initializes MySQL connection on startup (if ENABLE_MYSQL is defined)
  - Creates database tables if they don't exist

**4. `src/ui.c`** - GUI Integration
- **Lines to Check**: 
  - `on_add_car_clicked()` function (around line 101)
  - `on_exit_car_clicked()` function (around line 165)
- **What to Do**:
  - Add MySQL sync calls after file operations
  - Ensure data is written to both file and MySQL

**5. `schema.sql`** - Database Schema
- **Purpose**: SQL script to create database tables
- **Tables**:
  - `users` - User accounts
  - `garages` - Garage information
  - `parking_active` - Currently parked cars
  - `parking_history` - Parking history

---

### Task 2: Car Entry/Exit

#### Files to Work On:

**1. `src/parking.h`** - Parking Operations Header
- **Purpose**: Declares parking management functions
- **Key Functions**:
  - `add_car_entry()` - Add a car to parking
  - `exit_car()` - Remove car from parking
  - `find_active_car()` - Find car by number
  - `is_car_number_valid()` - Validate car number
  - `initialize_default_garages()` - Set up default garages
  - `update_garage_availability()` - Update garage slots

**2. `src/parking.c`** - Parking Operations Implementation
- **Purpose**: Core logic for car entry and exit
- **Key Functions Explained**:

  **`add_car_entry()`** (Lines ~80-130):
  - Validates car number (no duplicates, no commas)
  - Validates duration (1-12 hours)
  - Checks garage availability
  - Calculates bill using billing.c
  - Adds to active cars array
  - Updates garage availability
  - Returns error codes for different failures

  **`exit_car()`** (Lines ~135-180):
  - Finds car in active list
  - Creates history record
  - Exports receipt
  - Moves to history array
  - Updates garage availability
  - Updates total revenue

**3. `src/billing.h`** - Billing Header
- **Purpose**: Bill calculation functions
- **Key Functions**:
  - `calculate_bill()` - Calculate parking bill
  - `get_garage_revenue()` - Get revenue per garage
  - `get_total_revenue()` - Get total revenue

**4. `src/billing.c`** - Billing Implementation
- **Purpose**: Bill calculation logic
- **How It Works**:
  ```c
  float calculate_bill(const Garage *garage, VehicleType vehicle_type, int duration_hours) {
      float rate = (vehicle_type == VEHICLE_CAR) ? 
                   garage->hourly_rate_car : 
                   garage->hourly_rate_bike;
      return rate * duration_hours;
  }
  ```

**5. `src/ui.c`** - GUI Handlers
- **Functions to Understand**:
  - `on_add_car_clicked()` - Handles "Add Car" button click
  - `on_exit_car_clicked()` - Handles "Exit Car" button click
- **Flow**:
  1. Get user input from GUI
  2. Call parking.c functions
  3. Display results/errors
  4. Update GUI displays
  5. Save to files

**6. `src/models.h`** - Data Structures
- **Purpose**: Defines data structures used
- **Key Structures**:
  - `ParkingActive` - Active parking record
  - `ParkingHistory` - History record
  - `Garage` - Garage information
  - `AppState` - Application state

---

### Task 3: File Handling

#### Files to Work On:

**1. `src/fileio.h`** - File I/O Header
- **Purpose**: Declares all file operations
- **Key Functions**:
  - `load_users()` - Load users from users.txt
  - `save_user()` - Save user to users.txt
  - `load_active_cars()` - Load from active.csv
  - `save_active_cars()` - Save to active.csv
  - `load_history_cars()` - Load from history.csv
  - `save_history_cars()` - Save to history.csv
  - `append_history_car()` - Append to history.csv
  - `load_garages()` - Load garages from CSV
  - `save_garages()` - Save garages to CSV
  - `export_receipt()` - Generate receipt file
  - `format_timestamp()` - Format time for display
  - `parse_timestamp()` - Parse time from string

**2. `src/fileio.c`** - File I/O Implementation
- **Purpose**: All file reading/writing operations
- **Current Functions**:

  **CSV File Operations** (Lines ~60-200):
  - Reads/writes CSV files with headers
  - Handles comma-separated values
  - Parses timestamps
  - Validates data format

  **User File Operations** (Lines ~40-60):
  - Reads users.txt (format: username:password:role)
  - Appends new users

  **Receipt Export** (Lines ~250-280):
  - Creates text receipt files
  - Saves to data/receipts/ directory
  - Formats receipt with all details

**3. `src/main.c`** - File Loading on Startup
- **Lines to Check**: Lines 26-38
- **What It Does**:
  - Calls `load_active_cars()` on startup
  - Calls `load_history_cars()` on startup
  - Calls `load_garages()` on startup
  - Recalculates garage availability

**4. `src/ui.c`** - File Save Operations
- **Functions**:
  - `on_save_clicked()` - Manual save button
  - `on_load_clicked()` - Manual load button
  - Auto-save calls in add/exit functions

**5. Data Files** (in `data/` directory):
- `users.txt` - User credentials
- `active.csv` - Active parking records
- `history.csv` - Parking history
- `garages.csv` - Garage information
- `receipts/` - Generated receipts directory

---

### Task 4: Data Backup System

#### Files to Create/Modify:

**1. `src/fileio.h`** - Add Backup Function Declarations
- **New Functions to Add**:
  ```c
  int create_backup(const char *filename);
  int restore_backup(const char *backup_filename);
  void cleanup_old_backups(int keep_count);
  int list_backups(char backup_list[][200], int *count);
  int verify_backup(const char *backup_filename);
  ```

**2. `src/fileio.c`** - Implement Backup Functions
- **New Functions to Implement**:

  **`create_backup()`**:
  - Generate timestamp
  - Create `data/backups/` directory if needed
  - Copy file to backup location with timestamp
  - Return success/failure

  **`restore_backup()`**:
  - Copy backup file back to original location
  - Reload data
  - Return success/failure

  **`cleanup_old_backups()`**:
  - List all backup files
  - Sort by date
  - Delete oldest ones, keep last N
  - Prevent disk space issues

**3. `src/ui.h`** - Add UI Function Declarations
- **New Functions**:
  ```c
  void on_backup_clicked(GtkWidget *widget, gpointer data);
  void on_restore_clicked(GtkWidget *widget, gpointer data);
  ```

**4. `src/ui.c`** - Add Backup UI
- **What to Add**:
  - "Backup Now" button in dashboard
  - "Restore Backup" dropdown/button
  - Backup status display
  - Restore backup selection dialog

**5. `src/main.c`** - Auto-backup on Exit
- **Modify**: `on_shutdown()` function
- **Add**: Create backup before saving files

---

## 🔄 How Everything Works Together

### Car Entry Flow:
```
User clicks "Add Car" 
  → ui.c:on_add_car_clicked()
    → parking.c:add_car_entry()
      → billing.c:calculate_bill()
      → Updates AppState
    → fileio.c:save_active_cars() [Save to CSV]
    → db.c:db_add_active_car() [Save to MySQL if enabled]
    → ui.c:update_car_list_display()
```

### Car Exit Flow:
```
User clicks "Exit Car"
  → ui.c:on_exit_car_clicked()
    → parking.c:exit_car()
      → fileio.c:export_receipt() [Generate receipt]
      → Moves to history
      → Updates garage availability
    → fileio.c:append_history_car() [Save to CSV]
    → db.c:db_add_history_car() [Save to MySQL if enabled]
    → db.c:db_remove_active_car() [Remove from MySQL]
    → ui.c:update_car_list_display()
    → ui.c:update_revenue()
```

### File Operations Flow:
```
Application Startup
  → main.c:on_activate()
    → fileio.c:load_active_cars()
    → fileio.c:load_history_cars()
    → fileio.c:load_garages()
    → Recalculate garage availability

Save Operation
  → fileio.c:save_active_cars()
  → fileio.c:save_history_cars()
  → fileio.c:save_garages()
  → [NEW] fileio.c:create_backup() [Before saving]
```

---

## 📝 Implementation Checklist

### MySQL Integration:
- [ ] Test `src/db.c` connection
- [ ] Add error handling in `db_connect()`
- [ ] Add sync calls in `src/ui.c` (on_add_car_clicked, on_exit_car_clicked)
- [ ] Test with MySQL enabled: `make ENABLE_MYSQL=1`
- [ ] Verify data appears in MySQL tables
- [ ] Test fallback when MySQL unavailable

### Car Entry/Exit:
- [ ] Review `src/parking.c` functions
- [ ] Test all validation cases
- [ ] Enhance error messages
- [ ] Test edge cases (duplicate, full garage, etc.)
- [ ] Verify bill calculations
- [ ] Test receipt generation

### File Handling:
- [ ] Review `src/fileio.c` functions
- [ ] Add file locking mechanism
- [ ] Add transaction support (all-or-nothing)
- [ ] Add data validation on load
- [ ] Add error recovery
- [ ] Test with corrupted files

### Data Backup System:
- [ ] Create `create_backup()` function
- [ ] Create `restore_backup()` function
- [ ] Create `cleanup_old_backups()` function
- [ ] Add backup UI buttons
- [ ] Add auto-backup on save
- [ ] Test backup/restore functionality

---

## 🧪 Testing Your Changes

### Build Commands:
```bash
# Standard build
make clean
make

# With MySQL
make clean
make ENABLE_MYSQL=1

# Run
./smart_parking.exe
```

### Test Scenarios:
1. **MySQL Test**: Enable MySQL, add car, check database
2. **Entry Test**: Add car with various inputs, check validation
3. **Exit Test**: Exit car, verify receipt, check history
4. **File Test**: Save, load, check file contents
5. **Backup Test**: Create backup, restore, verify data

---

## 📚 Key Concepts to Understand

### 1. Data Flow:
- **GUI (ui.c)** → **Business Logic (parking.c)** → **Storage (fileio.c/db.c)**

### 2. Error Handling:
- Functions return error codes (0 = success, negative = error)
- UI displays error messages to user

### 3. Data Structures:
- `ParkingActive` - Cars currently parked
- `ParkingHistory` - Cars that exited
- `Garage` - Garage information
- `AppState` - Holds all application data

### 4. File Format:
- CSV files have headers
- Timestamps in format: "YYYY-MM-DD HH:MM:SS"
- Users file: "username:password:role"

---

## 🎯 Quick Reference

### Most Important Files:
1. **`src/parking.c`** - Car entry/exit logic (YOUR MAIN FILE)
2. **`src/fileio.c`** - File operations (YOUR MAIN FILE)
3. **`src/db.c`** - MySQL operations (YOUR MAIN FILE)
4. **`src/ui.c`** - GUI integration (needs MySQL sync calls)
5. **`src/billing.c`** - Bill calculation (used by parking.c)

### Functions You'll Modify Most:
- `add_car_entry()` in parking.c
- `exit_car()` in parking.c
- `save_active_cars()` in fileio.c
- `create_backup()` in fileio.c (NEW)
- `db_add_active_car()` in db.c
- `on_add_car_clicked()` in ui.c
- `on_exit_car_clicked()` in ui.c

---

## 💡 Tips

1. **Start Small**: Test one feature at a time
2. **Read Existing Code**: Understand patterns before modifying
3. **Test Frequently**: Build and test after each change
4. **Use Git**: Commit often with descriptive messages
5. **Ask Questions**: If something is unclear, ask!

---

**Good luck with your implementation! 🚀**

