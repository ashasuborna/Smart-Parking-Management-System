# Smart Parking Management System - Suborna's Tasks

## Suborna Akter's Tasks
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
- **Exit Car**: Enter car number, click "Exit Car" - system removes car, generates receipt, updates garage slots

#### Files:
- `src/parking.c` - Entry/exit logic (`add_car_entry()`, `exit_car()`)
- `src/ui.c` - GUI handlers (`on_add_car_clicked()`, `on_exit_car_clicked()`)

---

### 2. File Handling

#### How It Works:
- **Auto-Load**: On startup, loads data from CSV files (`data/active.csv`, `data/history.csv`, `data/garages.csv`)
- **Auto-Save**: Automatically saves when cars are added/removed
- **Manual Save**: Click "Save" button to save all data immediately
- **Manual Load**: Click "Load" button to reload data from files

#### Files:
- `src/fileio.c` - All file operations
  - `load_active_cars()` - Loads from `data/active.csv`
  - `save_active_cars()` - Saves to CSV
  - `load_history_cars()` - Loads from `data/history.csv`
  - `append_history_car()` - Appends to history
  - `load_garages()` / `save_garages()` - Garage file operations

#### Data Files:
- `data/active.csv` - Currently parked cars
- `data/history.csv` - Parking history
- `data/garages.csv` - Garage information

---

### 3. MySQL Integration

#### How It Works:
- **Connection**: Connects to MySQL on startup (if enabled)
- **Table Creation**: Creates tables automatically if they don't exist
- **Data Sync**: When car added/removed, saves to both CSV and MySQL
- **Fallback**: If MySQL unavailable, works with file storage only

#### Files:
- `src/db.c` - MySQL operations
  - `db_connect()` - Connect to database
  - `db_init_tables()` - Create tables
  - `db_add_active_car()` - Add car to MySQL
  - `db_remove_active_car()` - Remove from MySQL
  - `db_add_history_car()` - Add to history in MySQL
- `schema.sql` - Database schema

#### Enable MySQL:
1. Install MySQL Server
2. Create database: `CREATE DATABASE smart_parking;`
3. Update connection in `src/db.c` (host, user, password)
4. Build with: `make ENABLE_MYSQL=1`

---

### 4. Data Backup System

#### How It Works:
- **Create Backup**: Creates timestamped backup of data files
- **Restore Backup**: Restores data from backup file
- **Auto-Backup**: Automatically creates backup before saving
- **Cleanup**: Removes old backups to save space

#### Files:
- `src/fileio.c` - Backup functions (to be implemented)
  - `create_backup()` - Create backup
  - `restore_backup()` - Restore from backup
  - `cleanup_old_backups()` - Remove old backups

#### Backup Location:
- `data/backups/` - Backup files stored here

---

## 📁 Files for Each Feature

### Car Entry/Exit:
- `src/parking.c` / `src/parking.h`
- `src/ui.c` (handlers: `on_add_car_clicked`, `on_exit_car_clicked`)
- `src/billing.c` (bill calculation)

### File Handling:
- `src/fileio.c` / `src/fileio.h`
- `data/active.csv`
- `data/history.csv`
- `data/garages.csv`

### MySQL Integration:
- `src/db.c` / `src/db.h`
- `schema.sql`

### Data Backup:
- `src/fileio.c` (backup functions - to be implemented)

---

## ✅ Quick Test

1. **Run**: `./smart_parking.exe`
2. **Add Car**: Enter "ABC-1234", select Car, choose garage, duration 2 hours, click "Add Car"
3. **View**: Car appears in "Active Parked Cars" list
4. **Exit Car**: Enter "ABC-1234", click "Exit Car"
5. **Check**: Receipt generated in `data/receipts/`, data saved to CSV files

---

**Ready for Suborna's presentation! 🎉**
