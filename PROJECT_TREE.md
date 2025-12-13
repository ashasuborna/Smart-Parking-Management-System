# Smart Parking Management System - Project Tree

```
smart_parking/
├── README.md                    # Comprehensive documentation
├── Makefile                      # Build configuration (Linux/Windows)
├── build_windows_msys2.md       # Detailed Windows build instructions
├── schema.sql                    # MySQL database schema (optional)
├── PROJECT_TREE.md              # This file
│
├── src/                         # Source code directory
│   ├── main.c                   # Application entry point
│   ├── models.h                 # Data structures and type definitions
│   ├── models.c                 # Model implementations
│   ├── fileio.h                 # File I/O function declarations
│   ├── fileio.c                 # CSV file operations
│   ├── auth.h                   # Authentication declarations
│   ├── auth.c                   # Login/registration logic
│   ├── billing.h                # Billing function declarations
│   ├── billing.c                # Bill calculation logic
│   ├── parking.h                # Parking operations declarations
│   ├── parking.c                # Core parking management
│   ├── ui.h                     # GUI function declarations
│   ├── ui.c                     # GTK+3 GUI implementation
│   ├── db.h                     # MySQL integration (optional)
│   └── db.c                     # MySQL operations (optional)
│
└── data/                        # Data directory
    ├── users.txt                # User credentials (username:password:role)
    ├── active.csv               # Active parking records (CSV)
    ├── history.csv              # Parking history (CSV)
    ├── garages.csv              # Garage information (CSV)
    └── receipts/                # Exported receipt files
        └── (receipts generated on car exit)
```

## File Descriptions

### Core Application Files
- **main.c**: Entry point, initializes application and GTK
- **models.h/c**: Data structures (User, Garage, ParkingActive, ParkingHistory, AppState)
- **fileio.h/c**: CSV file reading/writing, receipt export
- **auth.h/c**: User authentication and registration
- **billing.h/c**: Bill calculation based on rates and duration
- **parking.h/c**: Add/exit car operations, validation
- **ui.h/c**: GTK+3 GUI (login window, dashboard window)

### Optional Module
- **db.h/c**: MySQL integration (compiled only with ENABLE_MYSQL flag)

### Data Files
- **users.txt**: Plain text user database (username:password:role)
- **active.csv**: Currently parked cars
- **history.csv**: Completed parking sessions
- **garages.csv**: Garage definitions and availability
- **receipts/**: Individual receipt files (text format)

### Build & Documentation
- **Makefile**: Build system for Linux/Windows
- **README.md**: Complete user and developer documentation
- **build_windows_msys2.md**: Step-by-step Windows setup
- **schema.sql**: MySQL table definitions

## Module Dependencies

```
main.c
  ├── models.h
  ├── parking.h → billing.h, fileio.h
  ├── fileio.h → models.h
  └── ui.h → models.h, parking.h, fileio.h, auth.h
      └── (GTK+3)
```

## Compilation Units

**Without MySQL:**
- main.c, models.c, fileio.c, auth.c, billing.c, parking.c, ui.c

**With MySQL (ENABLE_MYSQL=1):**
- All above + db.c

