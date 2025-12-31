# Smart Parking Management System - Suborna's Tasks

## My Task List
1. **MySQL Integration**
2. **Car Entry/Exit**
3. **File Handling**
4. **Data Backup System**

---

## 🚀 How to Setup and Run (Windows)

### Step 1: Install MSYS2
1. Download from https://www.msys2.org/
2. Install to default location: `C:\msys64`

### Step 2: Install Packages
1. Open **MSYS2 MinGW 64-bit** terminal
2. Update packages:
   ```bash
   pacman -Syu
   ```
   (Close terminal if asked, then reopen)
3. Install required packages:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
   ```

### Step 3: Build
1. Open **MSYS2 MinGW 64-bit** terminal
2. Navigate to project:
   ```bash
   cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
   ```
3. Build:
   ```bash
   make clean
   make
   ```

### Step 4: Run
```bash
./smart_parking.exe
```

**Note**: Application starts directly at dashboard (no login required)

---

## 📋 How Features Work

### 1. Car Entry/Exit

#### How It Works:
- **Add Car**: Enter car number, select vehicle type (Car/Bike), choose garage, select duration (1-12 hours), click "Add Car"
- **System Validates**: Checks car number format, garage availability, prevents duplicates
- **Bill Calculation**: Automatically calculates bill based on vehicle type and duration
- **Exit Car**: Enter car number, click "Exit Car" - system removes car from active list, adds to history, updates garage slots

#### Files:
- `src/car_entry_exit.c` - Entry/exit logic and bill calculation
  - `add_car_entry()` - Adds car to parking system
  - `exit_car()` - Removes car from active list and adds to history
  - `calculate_bill()` - Calculates parking bill
  - `find_active_car()` - Finds car in active list
  - `is_car_number_valid()` - Validates car number format
  - `update_garage_availability()` - Updates garage slot availability
- `src/car_entry_exit.h` - Function declarations
- `src/ui.c` - GUI handlers
  - `on_add_car_clicked()` - Handles "Add Car" button click
  - `on_exit_car_clicked()` - Handles "Exit Car" button click

---

### 2. File Handling

#### How It Works:
- **Auto-Load**: On startup, loads data from CSV files (`data/active.csv`, `data/history.csv`, `data/garages.csv`)
- **Auto-Save**: Automatically saves when cars are added/removed
- **Manual Save**: Click "Save" button to save all data immediately
- **Manual Load**: Click "Load" button to reload data from files

#### Files:
- `src/file_handling.c` - All file operations
  - `load_active_cars()` - Loads from `data/active.csv`
  - `save_active_cars()` - Saves to CSV
  - `load_history_cars()` - Loads from `data/history.csv`
  - `save_history_cars()` - Saves history to CSV
  - `append_history_car()` - Appends single record to history
  - `load_garages()` - Loads garage data from CSV
  - `save_garages()` - Saves garage data to CSV
  - `format_timestamp()` - Converts time to string format
  - `parse_timestamp()` - Parses timestamp string to time_t
  - `ensure_data_directory()` - Creates data directory if needed
- `src/file_handling.h` - Function declarations

#### Data Files:
- `data/active.csv` - Currently parked cars (format: car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount)
- `data/history.csv` - Parking history (format: car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount)
- `data/garages.csv` - Garage information (format: id,name,total_capacity,available_slots,hourly_rate_car,hourly_rate_bike)

---

### 3. MySQL Integration

#### How It Works:
- **Optional Module**: Build with `make ENABLE_MYSQL=1` to enable
- **Connection**: Connects to MySQL database on car add/exit operations
- **Table Creation**: Creates tables automatically if they don't exist
- **Data Sync**: When car added/removed, saves to both CSV and MySQL
- **Fallback**: If MySQL unavailable or not enabled, works with file storage only

#### Files:
- `src/mysql_integration.c` - MySQL operations
  - `db_connect()` - Connect to MySQL database
  - `db_disconnect()` - Close database connection
  - `db_init_tables()` - Create database tables if they don't exist
  - `db_add_active_car()` - Add car to MySQL active table
  - `db_remove_active_car()` - Remove car from MySQL active table
  - `db_add_history_car()` - Add car to MySQL history table
- `src/mysql_integration.h` - Function declarations
- `schema.sql` - Database schema (tables: users, garages, parking_active, parking_history)

#### Enable MySQL:
1. Install MySQL Server
2. Create database: `CREATE DATABASE smart_parking;`
3. Update connection parameters in `src/mysql_integration.c`:
   - `host` - MySQL server address (default: "localhost")
   - `user` - MySQL username (default: "root")
   - `password` - MySQL password (default: "")
   - `database` - Database name (default: "smart_parking")
   - `port` - MySQL port (default: 3306)
4. Build with: `make ENABLE_MYSQL=1`

---

### 4. Data Backup System

#### How It Works:
- **File-Based Backup**: CSV files serve as backup mechanism
- **Auto-Save**: Data automatically saved to CSV files on every operation
- **Manual Backup**: Use "Save" button to create backup of current state
- **Restore**: Use "Load" button to restore from CSV files

#### Implementation:
- Data backup is handled through CSV file operations in `src/file_handling.c`
- All data stored in CSV format for easy backup and restore
- CSV files can be manually copied for additional backup

---

## 📁 Project Structure

```
Smart Parking Management System/
├── src/
│   ├── main.c                 # Application entry point
│   ├── models.c/h            # Data structures (Car, Garage, AppState, etc.)
│   ├── car_entry_exit.c/h    # Car entry/exit logic and bill calculation
│   ├── file_handling.c/h     # CSV file operations
│   ├── mysql_integration.c/h # MySQL database operations (optional)
│   └── ui.c/h                # GTK+ GUI implementation
├── data/
│   ├── active.csv            # Currently parked cars
│   ├── history.csv           # Parking history
│   └── garages.csv           # Garage information
├── schema.sql                # MySQL database schema
├── Makefile                  # Build configuration
└── README.md                 # This file
```

---

## 📁 Files for Each Feature

### Car Entry/Exit:
- `src/car_entry_exit.c` / `src/car_entry_exit.h`
- `src/ui.c` (handlers: `on_add_car_clicked`, `on_exit_car_clicked`)

### File Handling:
- `src/file_handling.c` / `src/file_handling.h`
- `data/active.csv`
- `data/history.csv`
- `data/garages.csv`

### MySQL Integration:
- `src/mysql_integration.c` / `src/mysql_integration.h`
- `schema.sql`

### Data Backup:
- `src/file_handling.c` (CSV file operations serve as backup system)

---

## ✅ Quick Test

1. **Run**: `./smart_parking.exe`
2. **Add Car**: 
   - Enter "ABC-1234" in Car Number field
   - Select "Car" from Vehicle Type dropdown
   - Choose a garage from Garage dropdown
   - Select "2" hours from Duration dropdown
   - Click "Add Car"
3. **View**: Car appears in "Active Parked Cars" list with calculated bill
4. **Exit Car**: 
   - Enter "ABC-1234" in Exit Car field
   - Click "Exit Car"
5. **Check**: 
   - Car removed from active list
   - Car added to history
   - Data saved to `data/active.csv` and `data/history.csv`
   - Garage availability updated

---

## 🔧 Build Options

### Standard Build (File Storage Only):
```bash
make clean
make
```

### Build with MySQL Support:
```bash
make clean
make ENABLE_MYSQL=1
```

### Clean Build Files:
```bash
make clean
```

---

## 📝 Notes

- Application starts directly at dashboard (no login screen)
- All data persists in CSV files
- MySQL integration is optional - system works fully without it
- File handling provides automatic backup through CSV storage
- Bill calculation is automatic based on vehicle type and duration
