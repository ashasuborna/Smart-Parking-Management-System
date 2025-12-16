# Suborna Akter - Presentation Branch

## 🎯 Purpose
This branch is dedicated to **Suborna Akter's completed work** for presentation purposes.

## 📋 Assigned Tasks
1. **MySQL Integration**
2. **Car Entry/Exit**
3. **File Handling**
4. **Data Backup System**

---

## ✅ What to Include in This Branch

### Task 1: MySQL Integration
- [ ] Enhanced `src/db.c` with proper error handling
- [ ] Tested MySQL connection and table creation
- [ ] Verified data sync between file and MySQL
- [ ] Documentation of MySQL setup steps

### Task 2: Car Entry/Exit
- [ ] Tested `add_car_entry()` function thoroughly
- [ ] Tested `exit_car()` function thoroughly
- [ ] Verified all validation cases
- [ ] Tested edge cases (duplicates, full garage, etc.)

### Task 3: File Handling
- [ ] Verified CSV file operations work correctly
- [ ] Tested file loading on startup
- [ ] Tested file saving after operations
- [ ] Verified timestamp parsing works on Windows/Linux

### Task 4: Data Backup System
- [ ] Implemented `create_backup()` function
- [ ] Implemented `restore_backup()` function
- [ ] Implemented `cleanup_old_backups()` function
- [ ] Added backup UI buttons
- [ ] Tested backup/restore functionality

---

## 📁 Files You Worked On

### MySQL Integration
- `src/db.h` - MySQL function declarations
- `src/db.c` - MySQL implementation
- `schema.sql` - Database schema

### Car Entry/Exit
- `src/parking.c` - Your functions:
  - `add_car_entry()`
  - `exit_car()`
  - `find_active_car()`
  - `is_car_number_valid()`
  - `update_garage_availability()`
- `src/ui.c` - Your handlers:
  - `on_add_car_clicked()`
  - `on_exit_car_clicked()`

### File Handling
- `src/fileio.c` - Your functions:
  - `load_active_cars()`
  - `save_active_cars()`
  - `load_history_cars()`
  - `save_history_cars()`
  - `append_history_car()`
  - `load_garages()`
  - `save_garages()`
  - `format_timestamp()`
  - `parse_timestamp()`

### Data Backup System
- `src/fileio.c` - New backup functions (to be implemented)
- `src/ui.c` - Backup UI buttons (to be implemented)

---

## 🚀 How to Present Your Work

### 1. Build and Run
```bash
# Standard build
make clean
make

# With MySQL enabled
make clean
make ENABLE_MYSQL=1

# Run application
./smart_parking.exe
```

### 2. Demonstrate Features

#### MySQL Integration
- Show MySQL connection working
- Demonstrate data sync between file and MySQL
- Show database tables created

#### Car Entry/Exit
- Add a car (show validation)
- Exit a car (show receipt generation)
- Show error handling (duplicate, full garage, etc.)

#### File Handling
- Show CSV files being created/updated
- Demonstrate data persistence (restart app, data still there)
- Show timestamp formatting

#### Data Backup System
- Create a backup
- Restore from backup
- Show backup files in data/backups/

### 3. Code Walkthrough
- Explain your functions
- Show how MySQL sync works
- Explain file operations
- Demonstrate backup system

---

## 📝 Presentation Checklist

- [ ] All code is committed to this branch
- [ ] Code compiles without errors
- [ ] All features are tested and working
- [ ] Documentation is complete
- [ ] Screenshots/demos are ready
- [ ] Ready to present!

---

## 🔗 Related Documentation

- See `SUBORNA_FILES_CORRECT.md` for detailed file breakdown
- See `README.md` for project overview
- See `build_windows_msys2.md` for build instructions

---

**Good luck with your presentation! 🎉**

