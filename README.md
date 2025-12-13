# Smart Parking Management System

A complete university-level GUI application for managing parking operations, developed in C using GTK+3.

## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Building on Linux](#building-on-linux)
- [Building on Windows (MSYS2)](#building-on-windows-msys2)
- [Running the Application](#running-the-application)
- [Usage Guide](#usage-guide)
- [Demo Credentials](#demo-credentials)
- [MySQL Support (Optional)](#mysql-support-optional)
- [File Formats](#file-formats)
- [Troubleshooting](#troubleshooting)
- [Development Team](#development-team)

## ✨ Features

### Core Features (MVP)
- ✅ **User Authentication**: Login system with role-based access (admin/user)
- ✅ **Car Entry Management**: Add cars with validation (car number, vehicle type, garage, duration)
- ✅ **Car Exit Management**: Remove cars and finalize billing
- ✅ **Automatic Bill Calculation**: Based on garage rates and vehicle type
- ✅ **Garage Management**: 3 default garages with capacity tracking
- ✅ **Real-time Availability**: Display available slots per garage
- ✅ **Car List Display**: View active parked cars in a table
- ✅ **Search Functionality**: Search cars by car number
- ✅ **Sort Functionality**: Sort by car number or entry time
- ✅ **Data Persistence**: Auto-save/load using CSV files
- ✅ **Receipt Export**: Generate text receipts when cars exit
- ✅ **Revenue Tracking**: Total revenue calculation and display
- ✅ **Manual Save/Load**: Buttons for manual data operations

### Technical Features
- Clean MVC architecture (business logic separated from GUI)
- File-based storage (works without database)
- Optional MySQL integration (compile-time flag)
- Cross-platform support (Linux and Windows)
- Error handling and validation
- Memory-safe implementation

## 📁 Project Structure

```
smart_parking/
├── README.md                 # This file
├── Makefile                  # Build configuration (Linux)
├── build_windows_msys2.md   # Windows build instructions
├── schema.sql                # MySQL database schema (optional)
├── src/                      # Source code directory
│   ├── main.c               # Application entry point
│   ├── models.h/c           # Data structures and models
│   ├── fileio.h/c           # File I/O operations (CSV)
│   ├── auth.h/c             # Authentication system
│   ├── billing.h/c          # Bill calculation logic
│   ├── parking.h/c          # Core parking operations
│   ├── ui.h/c               # GTK+ GUI implementation
│   └── db.h/c               # MySQL integration (optional)
└── data/                     # Data directory
    ├── users.txt            # User credentials (username:password:role)
    ├── active.csv           # Active parking records
    ├── history.csv          # Parking history
    ├── garages.csv          # Garage information
    └── receipts/            # Exported receipts directory
```

## 🔧 Requirements

### Linux (Ubuntu/Debian)
- GCC compiler
- GTK+ 3.0 development libraries
- pkg-config
- make

### Windows (MSYS2)
- MSYS2 with MinGW-w64
- GTK+ 3.0 (via MSYS2)
- pkg-config (via MSYS2)

### Optional (MySQL Support)
- MySQL client library
- MySQL server (for testing)

## 🐧 Building on Linux

### Step 1: Install Dependencies
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libgtk-3-dev
sudo apt-get install pkg-config
```

### Step 2: Build the Project
```bash
cd smart_parking
make
```

### Step 3: Build with MySQL (Optional)
```bash
make ENABLE_MYSQL=1
```

### Step 4: Run
```bash
./smart_parking
```

## 🪟 Building on Windows (MSYS2)

See detailed instructions in [build_windows_msys2.md](build_windows_msys2.md)

### Quick Steps:
1. Install MSYS2 from https://www.msys2.org/
2. Open **MSYS2 MinGW 64-bit** terminal
3. Install packages:
   ```bash
   pacman -Syu
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
   ```
4. Navigate to project and build:
   ```bash
   cd "/c/path/to/smart_parking"
   make
   ```
5. Run:
   ```bash
   ./smart_parking.exe
   ```

## 🚀 Running the Application

### First Run
1. The application will create necessary data directories
2. Default users are loaded from `data/users.txt`
3. Default garages are initialized

### Login
- Use demo credentials (see below)
- After successful login, the dashboard opens

### Dashboard Features
- **Left Panel**: Car entry/exit forms, garage availability
- **Right Panel**: Active car list, search, revenue display
- **Buttons**: Save, Load, Logout, Exit App

## 📖 Usage Guide

### Adding a Car
1. Enter car number (e.g., "ABC-1234")
2. Select vehicle type (Car or Bike)
3. Choose garage from dropdown
4. Select duration (1-12 hours)
5. Click "Add Car"
6. Bill is calculated and displayed automatically

### Exiting a Car
1. Enter car number in exit section
2. Click "Exit Car"
3. Receipt is automatically exported to `data/receipts/`
4. Car is moved to history
5. Revenue is updated

### Searching Cars
- Type car number in search box
- List filters automatically
- Click "Clear" to reset

### Sorting
- Use sort dropdown to change sort order
- Options: By Car Number, By Entry Time

### Manual Save/Load
- **Save**: Click "Save" to manually save all data
- **Load**: Click "Load" to reload data from files
- Auto-save occurs on exit and after operations

## 🔑 Demo Credentials

### Admin Account
- **Username**: `admin`
- **Password**: `admin123`
- **Role**: Admin

### User Accounts
- **Username**: `user1`
- **Password**: `user123`
- **Role**: User

- **Username**: `user2`
- **Password**: `user123`
- **Role**: User

## 🗄️ MySQL Support (Optional)

### Enabling MySQL

1. **Install MySQL Client Library**
   - Linux: `sudo apt-get install libmysqlclient-dev`
   - Windows (MSYS2): `pacman -S mingw-w64-x86_64-mysql`

2. **Build with MySQL Flag**
   ```bash
   make ENABLE_MYSQL=1
   ```

3. **Configure Database**
   - Edit `src/db.c` to set MySQL connection parameters:
     ```c
     const char *host = "localhost";
     const char *user = "root";
     const char *password = "your_password";
     const char *database = "smart_parking";
     ```

4. **Create Database and Tables**
   ```bash
   mysql -u root -p < schema.sql
   ```

### How It Works
- When MySQL is enabled, all operations (add/exit) also write to MySQL
- File storage still works as primary/backup
- System works fully even if MySQL is unavailable (falls back to files)

## 📄 File Formats

### users.txt
Format: `username:password:role`
```
admin:admin123:admin
user1:user123:user
```

### active.csv
CSV format with header:
```csv
car_number,vehicle_type,garage_id,duration_hours,entry_time,bill_amount
ABC-1234,Car,1,2,2024-12-13 10:30:00,10.00
```

### history.csv
CSV format with header:
```csv
car_number,vehicle_type,garage_id,duration_hours,entry_time,exit_time,bill_amount
ABC-1234,Car,1,2,2024-12-13 10:30:00,2024-12-13 12:30:00,10.00
```

### garages.csv
CSV format with header:
```csv
id,name,total_capacity,available_slots,hourly_rate_car,hourly_rate_bike
1,Downtown Garage,50,45,5.00,2.00
```

### Receipts
Text files in `data/receipts/`:
```
========================================
    SMART PARKING MANAGEMENT SYSTEM
            PARKING RECEIPT
========================================

Car Number:      ABC-1234
Vehicle Type:    Car
Garage:          Downtown Garage (ID: 1)
Duration:        2 hour(s)
Entry Time:      2024-12-13 10:30:00
Exit Time:       2024-12-13 12:30:00
----------------------------------------
Total Amount:    $10.00
========================================
```

## 🔍 Troubleshooting

### GTK+ Not Found
**Linux:**
```bash
sudo apt-get install libgtk-3-dev pkg-config
pkg-config --modversion gtk+-3.0  # Verify installation
```

**Windows:**
- Ensure MSYS2 MinGW 64-bit terminal is used
- Install: `pacman -S mingw-w64-x86_64-gtk3`

### pkg-config Not Found
**Linux:**
```bash
sudo apt-get install pkg-config
```

**Windows:**
```bash
pacman -S mingw-w64-x86_64-pkg-config
```

### Compilation Errors
1. Check all dependencies are installed
2. Verify GTK+ version: `pkg-config --modversion gtk+-3.0`
3. Clean and rebuild: `make clean && make`

### Runtime Errors (DLL/Shared Library)
**Windows:**
- Run from MSYS2 MinGW 64-bit terminal
- Or add `C:\msys64\mingw64\bin` to PATH

**Linux:**
- Install runtime libraries: `sudo apt-get install libgtk-3-0`

### Data Not Saving
- Check write permissions in `data/` directory
- Ensure directory exists (created automatically on first run)

### MySQL Connection Failed
- Verify MySQL server is running
- Check connection parameters in `src/db.c`
- Ensure database exists: `CREATE DATABASE smart_parking;`
- Run schema: `mysql -u root -p smart_parking < schema.sql`

## 👥 Development Team

- **Sabiha Sultana Dipa** - User Login/Registration, Garage Management, Duration Selection, Bill Calculation
- **Suborna Akter** - MySQL Integration, Car Entry/Exit, File Handling, Data Backup System
- **Samiun Nur Saifa** - GUI Development (GTK+), Garage Slot Display, Car List View, Revenue Calculation
- **Nilufa Anjum Tanjim** - Search & Sort, Update/Delete Records, Print Receipt, Exit & Auto-Save Function

## 📝 License

This is a university project for educational purposes.

## 🎯 Project Status

✅ **MVP Complete** - All core features implemented and tested
✅ **File Storage** - CSV-based persistence working
✅ **GUI** - GTK+3 interface fully functional
⏳ **MySQL** - Optional module available (compile-time flag)

## 📚 References

- GTK+ Documentation: https://www.gtk.org/docs/
- MySQL C API: https://dev.mysql.com/doc/c-api/en/
- GNU GCC Compiler: https://gcc.gnu.org/

---

**Built with ❤️ using C, GTK+3, and modern software engineering practices**
