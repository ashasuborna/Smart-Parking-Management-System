# Suborna Akter - Your Specific Files and Code Sections

## 🎯 Your Assigned Tasks
1. **MySQL Integration**
2. **Car Entry/Exit**
3. **File Handling**
4. **Data Backup System**

---

## 📁 Files You Own (100% Your Responsibility)

### 1. MySQL Integration - COMPLETELY YOURS

#### `src/db.h` - **YOUR FILE**
- **Status**: Entire file is yours
- **Purpose**: All MySQL function declarations
- **Functions**:
  - `db_connect()` - Connect to MySQL
  - `db_disconnect()` - Close connection
  - `db_init_tables()` - Create tables
  - `db_add_active_car()` - Add car to MySQL
  - `db_remove_active_car()` - Remove car from MySQL
  - `db_add_history_car()` - Add to history in MySQL
  - `db_load_users()` - Load users from MySQL
  - `db_save_user()` - Save user to MySQL

#### `src/db.c` - **YOUR FILE**
- **Status**: Entire file is yours
- **Purpose**: All MySQL implementation
- **What You Need to Do**:
  - Test and enhance connection handling
  - Add error handling
  - Ensure proper sync with file operations
  - Test with `ENABLE_MYSQL=1` flag

#### `schema.sql` - **YOUR FILE**
- **Status**: Entire file is yours
- **Purpose**: MySQL database schema
- **What You Need to Do**:
  - Verify all tables are correct
  - Test schema creation
  - Ensure compatibility with your db.c code

---

### 2. Car Entry/Exit - PARTIALLY YOURS

#### `src/parking.h` - **PARTIALLY YOURS**
- **Your Functions** (Lines to focus on):
  - `add_car_entry()` - **YOURS** ✅
  - `exit_car()` - **YOURS** ✅
  - `find_active_car()` - **YOURS** ✅
  - `is_car_number_valid()` - **YOURS** ✅
  - `update_garage_availability()` - **YOURS** ✅
  
- **NOT YOURS** (Sabiha's work):
  - `initialize_default_garages()` - Sabiha's (garage management)

#### `src/parking.c` - **PARTIALLY YOURS**
- **Your Functions** (Focus on these):

  **`add_car_entry()`** (Approx. Lines 80-130) - **YOURS** ✅
  - Validates car number
  - Checks for duplicates
  - Validates duration
  - Checks garage availability
  - Adds car to active list
  - Updates garage slots
  - **What You Need to Do**: Test, enhance validation, add error handling

  **`exit_car()`** (Approx. Lines 135-180) - **YOURS** ✅
  - Finds car in active list
  - Creates history record
  - Exports receipt (calls Nilufa's function)
  - Moves to history
  - Updates garage availability
  - Updates revenue
  - **What You Need to Do**: Test, ensure proper cleanup

  **`find_active_car()`** (Approx. Lines 46-52) - **YOURS** ✅
  - Searches active cars by number
  - Returns index or -1 if not found

  **`is_car_number_valid()`** (Approx. Lines 33-44) - **YOURS** ✅
  - Validates car number format
  - Checks for commas (CSV safety)

  **`update_garage_availability()`** (Approx. Lines 70-80) - **YOURS** ✅
  - Updates available slots when cars enter/exit

- **NOT YOURS**:
  - `initialize_default_garages()` - Sabiha's (garage setup)

#### `src/ui.c` - **PARTIALLY YOURS** (Specific Functions Only)

**`on_add_car_clicked()`** (Approx. Lines 101-160) - **YOURS** ✅
- Gets input from GUI
- Calls YOUR `add_car_entry()` function
- Displays results
- **What You Need to Add**: MySQL sync call after file save
- **Current Code Location**: Around line 101-160

**`on_exit_car_clicked()`** (Approx. Lines 165-215) - **YOURS** ✅
- Gets car number from GUI
- Calls YOUR `exit_car()` function
- Updates displays
- **What You Need to Add**: MySQL sync call after file save
- **Current Code Location**: Around line 165-215

**NOT YOURS** (Other team members):
- `on_login_clicked()` - Sabiha's (login)
- `ui_show_login()` - Sabiha's (login UI)
- `ui_show_dashboard()` - Samiun's (GUI layout)
- `update_car_list_display()` - Samiun's (car list view)
- `update_garage_availability()` - Samiun's (display function)
- `update_total_revenue_display()` - Samiun's (revenue display)
- `on_search_changed()` - Nilufa's (search)
- `on_sort_changed()` - Nilufa's (sort)
- `on_save_clicked()` - Nilufa's (save button)
- `on_load_clicked()` - Nilufa's (load button)
- `on_exit_app_clicked()` - Nilufa's (exit app)

---

### 3. File Handling - PARTIALLY YOURS

#### `src/fileio.h` - **PARTIALLY YOURS**
- **Your Functions**:
  - `load_active_cars()` - **YOURS** ✅
  - `save_active_cars()` - **YOURS** ✅
  - `load_history_cars()` - **YOURS** ✅
  - `save_history_cars()` - **YOURS** ✅
  - `append_history_car()` - **YOURS** ✅
  - `load_garages()` - **YOURS** ✅
  - `save_garages()` - **YOURS** ✅
  - `format_timestamp()` - **YOURS** ✅
  - `parse_timestamp()` - **YOURS** ✅
  
- **NOT YOURS**:
  - `load_users()` - Sabiha's (user management)
  - `save_user()` - Sabiha's (user management)
  - `export_receipt()` - Nilufa's (receipt printing)

#### `src/fileio.c` - **PARTIALLY YOURS**
- **Your Functions** (Focus on these):

  **CSV File Operations** - **YOURS** ✅
  - `load_active_cars()` (Approx. Lines 60-90) - **YOURS**
  - `save_active_cars()` (Approx. Lines 92-115) - **YOURS**
  - `load_history_cars()` (Approx. Lines 117-150) - **YOURS**
  - `save_history_cars()` (Approx. Lines 152-175) - **YOURS**
  - `append_history_car()` (Approx. Lines 177-200) - **YOURS**
  - `load_garages()` (Approx. Lines 202-225) - **YOURS**
  - `save_garages()` (Approx. Lines 227-245) - **YOURS**

  **Timestamp Functions** - **YOURS** ✅
  - `format_timestamp()` (Approx. Lines 19-23) - **YOURS**
  - `parse_timestamp()` (Approx. Lines 25-40) - **YOURS**

  **Directory Management** - **YOURS** ✅
  - `ensure_data_directory()` (Approx. Lines 14-17) - **YOURS**

- **NOT YOURS**:
  - `load_users()` - Sabiha's
  - `save_user()` - Sabiha's
  - `export_receipt()` - Nilufa's (but you call it from exit_car)

#### `src/main.c` - **PARTIALLY YOURS** (File Loading Only)
- **Your Code Sections**:
  - Lines 26-38: File loading on startup - **YOURS** ✅
    - `load_active_cars()` call
    - `load_history_cars()` call
    - `load_garages()` call
    - Garage availability recalculation

- **NOT YOURS**:
  - Lines 14-55: Application initialization - Samiun's
  - Lines 57-63: Shutdown handler - Nilufa's

---

### 4. Data Backup System - COMPLETELY YOURS (NEW FEATURE)

#### `src/fileio.h` - **ADD THESE FUNCTIONS** (NEW)
- **Functions to Add**:
  ```c
  int create_backup(const char *filename);
  int restore_backup(const char *backup_filename);
  void cleanup_old_backups(int keep_count);
  int list_backups(char backup_list[][200], int *count);
  int verify_backup(const char *backup_filename);
  ```

#### `src/fileio.c` - **IMPLEMENT THESE FUNCTIONS** (NEW)
- **Functions to Implement**:
  - `create_backup()` - **YOURS** (NEW - Create timestamped backup)
  - `restore_backup()` - **YOURS** (NEW - Restore from backup)
  - `cleanup_old_backups()` - **YOURS** (NEW - Remove old backups)
  - `list_backups()` - **YOURS** (NEW - List available backups)
  - `verify_backup()` - **YOURS** (NEW - Verify backup integrity)

#### `src/ui.h` - **ADD THESE FUNCTIONS** (NEW)
- **Functions to Add**:
  ```c
  void on_backup_clicked(GtkWidget *widget, gpointer data);
  void on_restore_clicked(GtkWidget *widget, gpointer data);
  ```

#### `src/ui.c` - **ADD UI ELEMENTS** (NEW)
- **What to Add**:
  - "Backup Now" button in dashboard
  - "Restore Backup" dropdown/button
  - Backup status display
  - Restore backup selection dialog

#### `src/main.c` - **MODIFY THIS FUNCTION** (NEW)
- **Modify**: `on_shutdown()` function (Line 57)
- **Add**: Call `create_backup()` before saving files

---

## 📊 Summary: What's Yours vs Others

### ✅ 100% YOURS:
1. **`src/db.h`** - Entire file
2. **`src/db.c`** - Entire file
3. **`schema.sql`** - Entire file
4. **Data Backup System** - All new code

### ✅ PARTIALLY YOURS (Specific Functions):
1. **`src/parking.c`**:
   - ✅ `add_car_entry()` - YOURS
   - ✅ `exit_car()` - YOURS
   - ✅ `find_active_car()` - YOURS
   - ✅ `is_car_number_valid()` - YOURS
   - ✅ `update_garage_availability()` - YOURS
   - ❌ `initialize_default_garages()` - Sabiha's

2. **`src/fileio.c`**:
   - ✅ `load_active_cars()` - YOURS
   - ✅ `save_active_cars()` - YOURS
   - ✅ `load_history_cars()` - YOURS
   - ✅ `save_history_cars()` - YOURS
   - ✅ `append_history_car()` - YOURS
   - ✅ `load_garages()` - YOURS
   - ✅ `save_garages()` - YOURS
   - ✅ `format_timestamp()` - YOURS
   - ✅ `parse_timestamp()` - YOURS
   - ✅ `ensure_data_directory()` - YOURS
   - ❌ `load_users()` - Sabiha's
   - ❌ `save_user()` - Sabiha's
   - ❌ `export_receipt()` - Nilufa's (but you call it)

3. **`src/ui.c`**:
   - ✅ `on_add_car_clicked()` - YOURS (add MySQL sync)
   - ✅ `on_exit_car_clicked()` - YOURS (add MySQL sync)
   - ❌ Everything else - Other team members

4. **`src/main.c`**:
   - ✅ Lines 26-38 (file loading) - YOURS
   - ✅ Lines 43-50 (MySQL init) - YOURS
   - ❌ Rest - Other team members

---

## 🎯 Your Implementation Tasks

### Task 1: MySQL Integration
**Files**: `src/db.c`, `src/db.h`, `schema.sql`
- Enhance connection handling
- Add sync calls in `ui.c` (on_add_car_clicked, on_exit_car_clicked)
- Test with `make ENABLE_MYSQL=1`

### Task 2: Car Entry/Exit
**Files**: `src/parking.c` (your functions), `src/ui.c` (your handlers)
- Test `add_car_entry()` thoroughly
- Test `exit_car()` thoroughly
- Enhance error handling
- Add MySQL sync in UI handlers

### Task 3: File Handling
**Files**: `src/fileio.c` (your CSV functions)
- Add file locking
- Add backup before save
- Add error recovery
- Enhance validation

### Task 4: Data Backup System
**Files**: `src/fileio.c` (new functions), `src/ui.c` (new UI)
- Implement all backup functions
- Add UI buttons
- Add auto-backup on save
- Test backup/restore

---

## ⚠️ Important Notes

1. **Don't Modify Others' Code**: 
   - Don't change Sabiha's garage initialization
   - Don't change Samiun's GUI layout functions
   - Don't change Nilufa's search/sort/save functions

2. **Integration Points**:
   - You CALL `export_receipt()` (Nilufa's function) from your `exit_car()`
   - You USE `calculate_bill()` (Sabiha's function) in your `add_car_entry()`
   - You DISPLAY using Samiun's UI update functions

3. **Your Code Flow**:
   ```
   User Action → UI Handler (YOURS) → Parking Function (YOURS) 
   → File Save (YOURS) → MySQL Sync (YOURS) → UI Update (Samiun's)
   ```

---

## 📝 Quick Reference

### Files You Modify:
1. `src/db.c` - MySQL implementation
2. `src/db.h` - MySQL declarations
3. `src/parking.c` - Your functions only
4. `src/fileio.c` - Your CSV functions + NEW backup functions
5. `src/ui.c` - Only YOUR handler functions
6. `src/main.c` - Only file loading section
7. `schema.sql` - MySQL schema

### Files You DON'T Touch:
- `src/auth.c/h` - Sabiha's (login)
- `src/billing.c/h` - Sabiha's (bill calculation)
- `src/models.c/h` - Shared (data structures)
- Most of `src/ui.c` - Other team members' GUI code
- Most of `src/main.c` - Other initialization code

---

**Focus on YOUR functions and YOUR tasks! Good luck! 🚀**




