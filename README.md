# Smart Parking Management System

A complete GUI-based parking management application developed in **C programming language** using **GTK+3** for the user interface. This system automates parking operations, manages vehicle entries/exits, calculates bills automatically, and maintains comprehensive records.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technology Stack](#technology-stack)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [How It Works](#how-it-works)
- [User Guide](#user-guide)
- [File Formats](#file-formats)
- [MySQL Integration (Optional)](#mysql-integration-optional)
- [Troubleshooting](#troubleshooting)
- [Development Team](#development-team)

## 🎯 Overview

The Smart Parking Management System is designed to solve common parking management challenges in modern cities. It provides an automated solution for:

- **Finding available parking spaces** in nearby garages
- **Managing vehicle entries and exits** efficiently
- **Calculating parking bills** automatically based on duration and vehicle type
- **Maintaining records** of all parking transactions
- **Tracking revenue** and garage availability in real-time

The system uses a **file-based storage** approach (CSV files) that works without requiring a database, making it easy to deploy and use. Optional MySQL integration is available for advanced users.

## ✨ Features

### Core Features

#### 1. **User Authentication System**
- Secure login with username and password
- Role-based access control (Admin and User roles)
- User credentials stored in `data/users.txt`
- Default users provided for testing

#### 2. **Garage Management**
- **3 Default Garages** pre-configured:
  - Downtown Garage (50 slots, $5/hr car, $2/hr bike)
  - Mall Parking (100 slots, $4/hr car, $1.50/hr bike)
  - Office Complex (75 slots, $6/hr car, $3/hr bike)
- Real-time availability tracking
- Automatic slot updates when cars enter/exit

#### 3. **Car Entry Management**
- Add vehicles with validation:
  - Car number (unique, no duplicates)
  - Vehicle type (Car or Bike)
  - Garage selection (with availability display)
  - Duration selection (1-12 hours)
- Automatic bill calculation based on:
  - Selected garage rates
  - Vehicle type
  - Parking duration
- Prevents duplicate entries
- Validates garage capacity

#### 4. **Car Exit Management**
- Remove vehicles from active parking
- Automatic receipt generation
- Moves record to history
- Updates garage availability
- Updates total revenue

#### 5. **Real-time Display**
- **Active Cars List**: Shows all currently parked vehicles
  - Car number
  - Vehicle type
  - Garage ID
  - Duration
  - Bill amount
  - Entry time
- **Garage Availability**: Shows available slots for each garage
- **Total Revenue**: Displays cumulative revenue from exited cars

#### 6. **Search and Sort**
- **Search**: Filter cars by car number (real-time)
- **Sort**: Sort by car number or entry time
- Clear search functionality

#### 7. **Data Persistence**
- **Auto-save**: Automatically saves data after each operation
- **Auto-load**: Loads data when application starts
- **Manual Save/Load**: Buttons for manual data operations
- **File-based Storage**: Uses CSV files (no database required)

#### 8. **Receipt Export**
- Automatically generates text receipts when cars exit
- Saved to `data/receipts/` directory
- Includes all transaction details

### Technical Features

- **Clean Architecture**: MVC pattern (Model-View-Controller)
- **Modular Design**: Separated business logic from GUI
- **Error Handling**: Comprehensive validation and error messages
- **Memory Safe**: No memory leaks, proper resource management
- **Cross-platform**: Works on Windows (MSYS2) and Linux
- **Optional MySQL**: Database integration available via compile flag

## 🛠️ Technology Stack

- **Programming Language**: C (C11 standard)
- **GUI Framework**: GTK+ 3.0
- **Compiler**: GNU GCC (via MSYS2/MinGW on Windows)
- **Build System**: Make
- **Data Storage**: CSV files (file-based)
- **Optional Database**: MySQL (compile-time flag)

## 📁 Project Structure

```
Smart-Parking-Management-System/
├── README.md                    # This file
├── Makefile                     # Build configuration
├── build_windows_msys2.md      # Windows build instructions
├── PROJECT_TREE.md              # Detailed project structure
├── schema.sql                   # MySQL database schema (optional)
├── .gitignore                   # Git ignore rules
│
├── src/                         # Source code directory
│   ├── main.c                   # Application entry point
│   ├── models.h/c               # Data structures and models
│   ├── fileio.h/c               # File I/O operations (CSV)
│   ├── auth.h/c                 # Authentication system
│   ├── billing.h/c              # Bill calculation logic
│   ├── parking.h/c               # Core parking operations
│   ├── ui.h/c                   # GTK+3 GUI implementation
│   └── db.h/c                   # MySQL integration (optional)
│
└── data/                        # Data directory
    ├── users.txt                # User credentials
    ├── garages.csv              # Garage information
    ├── active.csv               # Active parking records
    ├── history.csv              # Parking history
    └── receipts/                # Generated receipts
```

## 💻 Installation

### For Windows (MSYS2)

#### Step 1: Install MSYS2
1. Download MSYS2 from: https://www.msys2.org/
2. Run the installer
3. Install to default location: `C:\msys64`

#### Step 2: Install Development Tools
Open **MSYS2 MinGW 64-bit** terminal (NOT regular MSYS2) and run:

```bash
pacman -Syu
# (Close and reopen terminal if prompted)

pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
```

### For Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential libgtk-3-dev pkg-config make
```

## 🔨 Building the Project

### Step 1: Navigate to Project Directory

**Windows (MSYS2 MinGW 64-bit terminal):**
```bash
cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
```

**Linux:**
```bash
cd /path/to/Smart-Parking-Management-System
```

### Step 2: Build

**Standard build (file-based storage only):**
```bash
make
```

**Build with MySQL support (optional):**
```bash
make ENABLE_MYSQL=1
```

### Step 3: Verify Build

After successful build, you should see `smart_parking.exe` (Windows) or `smart_parking` (Linux) in the project directory.

## 🚀 Running the Application

### Windows (MSYS2 MinGW 64-bit terminal):
```bash
./smart_parking.exe
```

### Linux:
```bash
./smart_parking
```

**Important**: The application must be run from the MSYS2 terminal (Windows) or terminal (Linux) to have access to GTK+ libraries.

## 🔑 Login Credentials

Default users are provided in `data/users.txt`:

| Username | Password | Role |
|----------|----------|------|
| admin    | admin123 | Admin |
| user1    | user123  | User  |
| user2    | user123  | User  |

## 📖 How It Works

### Application Flow

1. **Startup**:
   - Application initializes default garages
   - Loads existing data from CSV files
   - Calculates garage availability from active records
   - Displays login window

2. **Authentication**:
   - User enters credentials
   - System validates against `data/users.txt`
   - On success, opens dashboard

3. **Dashboard Operations**:
   - **Add Car**: Validates input, checks availability, calculates bill, saves to `active.csv`
   - **Exit Car**: Moves record to `history.csv`, generates receipt, updates revenue
   - **Search**: Filters active cars list in real-time
   - **Save/Load**: Manual data operations

4. **Data Persistence**:
   - All operations auto-save to CSV files
   - Data loads automatically on startup
   - Receipts saved to `data/receipts/`

### Bill Calculation

Bills are calculated using the formula:
```
Bill = Hourly Rate × Duration (hours)
```

Where:
- **Hourly Rate** depends on:
  - Selected garage
  - Vehicle type (Car or Bike)
- **Duration** is selected by user (1-12 hours)

Example:
- Car parked in Downtown Garage for 2 hours
- Rate: $5.00/hour
- Bill: $5.00 × 2 = $10.00

## 📱 User Guide

### Adding a Car

1. Enter **Car Number** (e.g., "ABC-1234")
2. Select **Vehicle Type** (Car or Bike)
3. Choose **Garage** from dropdown (shows available slots)
4. Select **Duration** (1-12 hours)
5. Click **"Add Car"** button
6. Bill is calculated and displayed automatically
7. Car appears in the active cars list

### Exiting a Car

1. Enter the **Car Number** in the exit section
2. Click **"Exit Car"** button
3. Receipt is automatically generated and saved
4. Car is removed from active list
5. Record moved to history
6. Revenue is updated

### Searching Cars

1. Type car number in the **Search** box
2. List filters automatically as you type
3. Click **"Clear"** to reset search

### Sorting

1. Use the **Sort** dropdown
2. Select "Sort by Car Number" or "Sort by Entry Time"
3. List updates automatically

### Manual Save/Load

- **Save**: Click "Save" to manually save all data
- **Load**: Click "Load" to reload data from files

### Logout

- Click **"Logout"** to return to login screen
- Data is automatically saved before logout

## 📄 File Formats

### users.txt
Format: `username:password:role`
```
admin:admin123:admin
user1:user123:user
user2:user123:user
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
Thank you for using our service!
```

## 🗄️ MySQL Integration (Optional)

### Enabling MySQL Support

1. **Install MySQL Client Library**:
   - Windows: `pacman -S mingw-w64-x86_64-mysql`
   - Linux: `sudo apt-get install libmysqlclient-dev`

2. **Build with MySQL Flag**:
   ```bash
   make ENABLE_MYSQL=1
   ```

3. **Configure Database**:
   - Edit `src/db.c` to set connection parameters:
     ```c
     const char *host = "localhost";
     const char *user = "root";
     const char *password = "your_password";
     const char *database = "smart_parking";
     ```

4. **Create Database and Tables**:
   ```bash
   mysql -u root -p < schema.sql
   ```

### How MySQL Works

- When enabled, all operations (add/exit) also write to MySQL
- File storage still works as primary/backup
- System works fully even if MySQL is unavailable (falls back to files)

## 🔧 Troubleshooting

### Build Issues

**"pkg-config: command not found"**
- Windows: Make sure you're in MSYS2 MinGW 64-bit terminal
- Install: `pacman -S mingw-w64-x86_64-pkg-config`
- Linux: `sudo apt-get install pkg-config`

**"GTK+ not found"**
- Windows: `pacman -S mingw-w64-x86_64-gtk3`
- Linux: `sudo apt-get install libgtk-3-dev`
- Verify: `pkg-config --modversion gtk+-3.0`

**"gcc: command not found"**
- Windows: Install GCC via MSYS2: `pacman -S mingw-w64-x86_64-gcc`
- Linux: `sudo apt-get install build-essential`

### Runtime Issues

**Application doesn't open / DLL errors**
- Windows: Run from MSYS2 MinGW 64-bit terminal (not PowerShell)
- Ensure MSYS2 bin directory is in PATH
- DLLs location: `C:\msys64\mingw64\bin\`

**"Repository not found" errors**
- Check you're in the correct directory
- Verify file paths in code match your system

**Data not saving**
- Check write permissions in `data/` directory
- Ensure directory exists (created automatically on first run)

### MySQL Issues

**Connection failed**
- Verify MySQL server is running
- Check connection parameters in `src/db.c`
- Ensure database exists: `CREATE DATABASE smart_parking;`
- Run schema: `mysql -u root -p smart_parking < schema.sql`

## 👥 Development Team

- **Sabiha Sultana Dipa** - User Login/Registration, Garage Management, Duration Selection, Bill Calculation
- **Suborna Akter** - MySQL Integration, Car Entry/Exit, File Handling, Data Backup System
- **Samiun Nur Saifa** - GUI Development (GTK+), Garage Slot Display, Car List View, Revenue Calculation
- **Nilufa Anjum Tanjim** - Search & Sort, Update/Delete Records, Print Receipt, Exit & Auto-Save Function

## 📚 References

- [GTK+ Documentation](https://www.gtk.org/docs/)
- [MySQL C API Reference](https://dev.mysql.com/doc/c-api/en/)
- [GNU GCC Compiler](https://gcc.gnu.org/)
- [MSYS2 Documentation](https://www.msys2.org/)

## 📝 License

This is a university project developed for educational purposes.

## 🎯 Project Status

✅ **MVP Complete** - All core features implemented and tested  
✅ **File Storage** - CSV-based persistence working  
✅ **GUI** - GTK+3 interface fully functional  
✅ **Documentation** - Complete user and developer guides  
⏳ **MySQL** - Optional module available (compile-time flag)

---

**Built with ❤️ using C, GTK+3, and modern software engineering practices**

For detailed build instructions, see [build_windows_msys2.md](build_windows_msys2.md)  
For project structure details, see [PROJECT_TREE.md](PROJECT_TREE.md)
