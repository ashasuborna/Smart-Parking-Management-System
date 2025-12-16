# Smart Parking Management System

## Quick Start

### Build
```bash
make clean
make
```

### Run
```bash
./smart_parking.exe
```

## Features
- Car Entry/Exit
- File Handling (CSV)
- MySQL Integration (optional)
- Data Backup System

## MySQL Setup (Optional)
```bash
make ENABLE_MYSQL=1
```

## Branch: add-exit-car-file-handling-by-suborna
This branch contains Suborna's tasks: Car Entry/Exit, File Handling, MySQL Integration.

---

## 📋 Step-by-Step Instructions for Windows

### Prerequisites
1. **MSYS2** - Download and install from https://www.msys2.org/
2. **GTK+3** - Install via MSYS2 package manager
3. **GCC Compiler** - Included with MSYS2

### Installation Steps

#### Step 1: Install MSYS2
1. Download MSYS2 installer from https://www.msys2.org/
2. Run the installer and follow the installation wizard
3. Install to default location: `C:\msys64`

#### Step 2: Install Required Packages
1. Open **MSYS2 MinGW 64-bit** terminal (NOT MSYS2 terminal)
2. Update package database:
   ```bash
   pacman -Syu
   ```
   (Close terminal if asked, then reopen)
3. Install GCC, GTK+3, and build tools:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
   ```

#### Step 3: Navigate to Project
1. Open **MSYS2 MinGW 64-bit** terminal
2. Navigate to project directory:
   ```bash
   cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
   ```

#### Step 4: Build the Project
```bash
make clean
make
```

#### Step 5: Run the Application
```bash
./smart_parking.exe
```

---

## 🚀 How Features Work

### 1. Car Entry/Exit

#### How It Works:
1. **Add Car**: Enter car number, select vehicle type (Car/Bike), choose garage, select duration (1-12 hours)
2. **System Validates**: Checks if car number is valid, garage has space, no duplicate entries
3. **Bill Calculation**: Automatically calculates bill based on vehicle type and duration
4. **Exit Car**: Enter car number, system removes car, generates receipt, updates garage availability

#### Files Used:
- `src/parking.c` - Core entry/exit logic
  - `add_car_entry()` - Adds car to parking system
  - `exit_car()` - Removes car and generates receipt
  - `find_active_car()` - Searches for car in active list
  - `is_car_number_valid()` - Validates car number format
  - `update_garage_availability()` - Updates garage slots
- `src/parking.h` - Function declarations
- `src/ui.c` - GUI handlers
  - `on_add_car_clicked()` - Handles "Add Car" button
  - `on_exit_car_clicked()` - Handles "Exit Car" button
- `src/billing.c` - Bill calculation
  - `calculate_bill()` - Calculates parking bill

#### Data Storage:
- Active cars: `data/active.csv`
- History: `data/history.csv`
- Receipts: `data/receipts/receipt_*.txt`

---

### 2. File Handling

#### How It Works:
1. **Auto-Load**: On startup, loads active cars, history, and garages from CSV files
2. **Auto-Save**: Automatically saves data when cars are added/removed
3. **Manual Save**: Click "Save" button to save all data immediately
4. **Manual Load**: Click "Load" button to reload data from files

#### Files Used:
- `src/fileio.c` - All file operations
  - `load_active_cars()` - Loads active cars from `data/active.csv`
  - `save_active_cars()` - Saves active cars to CSV
  - `load_history_cars()` - Loads history from `data/history.csv`
  - `save_history_cars()` - Saves history to CSV
  - `append_history_car()` - Appends one record to history
  - `load_garages()` - Loads garages from `data/garages.csv`
  - `save_garages()` - Saves garages to CSV
  - `format_timestamp()` - Formats time for CSV
  - `parse_timestamp()` - Parses time from CSV
- `src/fileio.h` - Function declarations
- `src/ui.c` - Save/Load button handlers
  - `on_save_clicked()` - Manual save
  - `on_load_clicked()` - Manual load

#### Data Files:
- `data/active.csv` - Currently parked cars
- `data/history.csv` - Parking transaction history
- `data/garages.csv` - Garage information
- `data/users.txt` - User accounts (not used in this branch)

#### CSV Format:
**active.csv:**
```
car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
ABC-1234,Car,1,2,2024-12-13 14:30:00,10.00
```

**history.csv:**
```
car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount
ABC-1234,Car,1,2,2024-12-13 14:30:00,2024-12-13 16:30:00,10.00
```

---

### 3. MySQL Integration

#### How It Works:
1. **Connection**: Connects to MySQL database on startup (if enabled)
2. **Table Creation**: Creates tables automatically if they don't exist
3. **Data Sync**: When car is added/removed, data is saved to both CSV and MySQL
4. **Fallback**: If MySQL unavailable, system works with file storage only

#### Files Used:
- `src/db.c` - MySQL implementation
  - `db_connect()` - Connects to MySQL database
  - `db_disconnect()` - Closes connection
  - `db_init_tables()` - Creates database tables
  - `db_add_active_car()` - Adds car to MySQL active table
  - `db_remove_active_car()` - Removes car from MySQL
  - `db_add_history_car()` - Adds to MySQL history table
- `src/db.h` - Function declarations
- `schema.sql` - Database schema (table definitions)

#### MySQL Setup:
1. Install MySQL Server
2. Create database:
   ```sql
   CREATE DATABASE smart_parking;
   ```
3. Update connection in `src/db.c`:
   ```c
   const char *host = "localhost";
   const char *user = "root";
   const char *password = "your_password";
   const char *database = "smart_parking";
   ```
4. Build with MySQL enabled:
   ```bash
   make clean
   make ENABLE_MYSQL=1
   ```

#### Database Tables:
- `users` - User accounts
- `garages` - Garage information
- `parking_active` - Currently parked cars
- `parking_history` - Parking transaction history

---

### 4. Data Backup System

#### How It Works:
1. **Create Backup**: Creates timestamped backup of data files
2. **Restore Backup**: Restores data from backup file
3. **Auto-Backup**: Automatically creates backup before saving
4. **Cleanup**: Removes old backups to save space

#### Files Used:
- `src/fileio.c` - Backup functions (to be implemented)
  - `create_backup()` - Creates backup of data files
  - `restore_backup()` - Restores from backup
  - `cleanup_old_backups()` - Removes old backups
- `src/ui.c` - Backup UI buttons (to be implemented)

#### Backup Location:
- `data/backups/` - Backup files stored here

---

## 📁 Project Structure

```
Smart Parking Management System/
├── README.md              # This file
├── Makefile              # Build configuration
├── schema.sql            # MySQL database schema
├── data/                 # Data files
│   ├── active.csv        # Active parked cars
│   ├── history.csv       # Parking history
│   ├── garages.csv       # Garage information
│   ├── users.txt         # User accounts
│   └── receipts/         # Generated receipts
└── src/                  # Source code
    ├── main.c            # Application entry point
    ├── models.h/c        # Data structures
    ├── parking.h/c       # Car entry/exit logic (Suborna)
    ├── fileio.h/c        # File operations (Suborna)
    ├── db.h/c            # MySQL integration (Suborna)
    ├── billing.h/c       # Bill calculation
    ├── auth.h/c          # Authentication (not used)
    └── ui.h/c            # GUI interface
```

---

## 🎯 Feature Files Summary

### Suborna's Tasks:

**Car Entry/Exit:**
- `src/parking.c` - Main logic
- `src/parking.h` - Declarations
- `src/ui.c` - UI handlers (on_add_car_clicked, on_exit_car_clicked)

**File Handling:**
- `src/fileio.c` - All CSV operations
- `src/fileio.h` - Declarations
- `data/*.csv` - Data files

**MySQL Integration:**
- `src/db.c` - MySQL operations
- `src/db.h` - Declarations
- `schema.sql` - Database schema

**Data Backup:**
- `src/fileio.c` - Backup functions (to be implemented)

---

## 🔧 Troubleshooting

### Build Errors:
- **Error: pkg-config not found**
  - Make sure you're using **MSYS2 MinGW 64-bit** terminal (not MSYS2)
  - Install: `pacman -S mingw-w64-x86_64-pkg-config`

- **Error: GTK+ not found**
  - Install: `pacman -S mingw-w64-x86_64-gtk3`

- **Error: gcc not found**
  - Install: `pacman -S mingw-w64-x86_64-gcc`

### Runtime Errors:
- **Application won't start**
  - Check GTK+ is installed correctly
  - Verify you're running from MSYS2 MinGW 64-bit terminal

- **MySQL connection fails**
  - Check MySQL server is running
  - Verify connection settings in `src/db.c`
  - Make sure database exists

---

## 📝 Usage Example

1. **Start Application**: Run `./smart_parking.exe`
2. **Add Car**: 
   - Enter car number: "ABC-1234"
   - Select vehicle: Car
   - Select garage: Downtown Garage
   - Select duration: 2 hours
   - Click "Add Car"
3. **View Car**: Car appears in "Active Parked Cars" list
4. **Exit Car**:
   - Enter car number: "ABC-1234"
   - Click "Exit Car"
   - Receipt generated in `data/receipts/`
5. **Save Data**: Click "Save" button to save all data
6. **Load Data**: Click "Load" button to reload from files

---

## ✅ Testing Checklist

- [ ] Build succeeds without errors
- [ ] Application starts and shows dashboard
- [ ] Can add car successfully
- [ ] Can exit car successfully
- [ ] Receipt is generated on exit
- [ ] Data saves to CSV files
- [ ] Data loads from CSV files on restart
- [ ] MySQL sync works (if enabled)
- [ ] Garage availability updates correctly

---

**Ready for Suborna's presentation! 🎉**
